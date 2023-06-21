#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "include/ascii_numbers.h"
#define MAX_ROUNDS 10
#ifdef _WIN32
#define CLEAR "cls"
#include <windows.h>
#else
#define CLEAR "clear"
#include <unistd.h>
#include <pthread.h>
#endif

int running = 1;
int paused = 0;
typedef struct s_stopWatch {
    int h;
    int m;
    int s;
} stopWatch;

typedef struct s_stopWatchRounds {
    int currentLength;
    stopWatch rounds[MAX_ROUNDS];
}stopWatchRounds;

stopWatch watch = {
        .h = 0,
        .m = 0,
        .s = 0
};

stopWatchRounds rounds;

char *get_time(char *raw_time) {
    char *t_str;
    t_str = strtok(raw_time, " ");
    while (*t_str) {
        t_str = strtok(NULL," ");
        if(strchr(t_str,':') != NULL) {
            return t_str;
        }
    }
    return NULL;
}

void updateStopWatch() {
    watch.s++;
    if(watch.s >= 60) {
        watch.s -= 60;
        watch.m++;
    }
    if(watch.m >= 60) {
        watch.m -= 60;
        watch.h++;
    }
}

void stopWatchToString(char *str, stopWatch *w) {
    char h[3];
    char m[3];
    char s[3];
    sprintf(h,w->h < 10 ?"0%d" : "%d",w->h);
    sprintf(m,w->m < 10 ?"0%d" : "%d",w->m);
    sprintf(s,w->s < 10 ?"0%d" : "%d",w->s);
    sprintf(str,"%s:%s:%s",h,m,s);
}



#ifndef _WIN32
void *worker() {
    while (running) {
        int c = getchar();
        switch(c) {
            case 'q':
                running = 0;
                break;
            case 'p':
                paused = 1;
                break;
            case 'r':
                if(rounds.currentLength < MAX_ROUNDS) {
                    rounds.rounds[rounds.currentLength].s = watch.s;
                    rounds.rounds[rounds.currentLength].m = watch.m;
                    rounds.rounds[rounds.currentLength++].h = watch.h;
                }
                break;
            case 'c':
                paused = 0;
                break;
            default:
                continue;
        }

    }
    return NULL;
}
#endif

int main (int argv, char** argc) {
    if(argv == 2 && strncmp(argc[1],"h",strlen(argc[1])) == 0) {
        printf("Syntax: AsciiClock <mode> <duration>\n");
        printf("Modes:\nNone - Clock\n1 - Stopwatch\n");
#ifndef _WIN32
        printf("Controls:\nStopwatch (Linux only)\np - pause\nc - continue\nr - stop round\n\n");
#endif
        printf("When choosing mode 1 the second paramter <duration> can be used to set a limit\n");
        printf("Example usage for the stopwatch: Asciiclock 1 10 - This will display a stopwatch, which is counting to 10\n");
        return 0;
    }
    int mode = argv <= 1 ? 0 : 1;
    long limit = 1, iter = 1;
    char *overflow;
    if(argv >= 3) {
        limit = strtol(argc[2],&overflow,10);
    }

    time_t rawtime;
    struct tm * timeinfo;
    system(CLEAR);
#ifndef _WIN32
    pthread_t th1;
    pthread_create(&th1, NULL, worker, "");
    pthread_detach(th1);
    char *str = calloc(9,sizeof(char));
#endif
    char *time_ptr = argv == 1 ? NULL: calloc(9,sizeof(char));
    while(iter <= limit) {
        if(!running) {
            free(str);
            exit(0);
        }
        unsigned long start = time(NULL);
        int idx = 0;
        int needed_ascii_nums[8];
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );

        if(mode == 1) {
            if(!paused) {
                updateStopWatch();
                stopWatchToString(time_ptr, &watch);
            }
        }else{
            time_ptr = get_time(asctime(timeinfo));
        }

        //get the indices of the ascii numbers
        while (*time_ptr) {
            for(int i = 0; i < ASCII_NUMS_COUNT; ++i) {
                if(ascii_nums[i].number == *time_ptr) {
                    needed_ascii_nums[idx++] = i;
                }
            }
            time_ptr++;
        }

        //render line by line
        for(int i = 0; i < 6; ++i) {
            for(int j = 0; j < 8; ++j) {
                printf("%s",ascii_nums[needed_ascii_nums[j]].ascii[i]);
            }
            printf("\n");
        }
        if(argv >= 3) {
            iter++;
        }
        for(int i = 0; i < rounds.currentLength; ++i) {
            stopWatchToString(str, &rounds.rounds[i]);
            printf("Round %d: %s\n",i+1,str);
        }
        //flush, because buffered and sleep
        fflush(stdout);
#ifdef _WIN32
        Sleep(1000 - (time(NULL) - start))
#else
        sleep(1 - (time(NULL) - start));
#endif
        system(CLEAR);
    }
    return 0;
}