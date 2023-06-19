#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "include/ascii_numbers.h"

#ifdef _WIN32
#define CLEAR "cls"
#include <windows.h>
#else
#define CLEAR "clear"
#include <unistd.h>
#endif

typedef struct s_stopWatch {
    int h;
    int m;
    int s;
} stopWatch;

char *get_time(char *raw_time, size_t len) {
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

void updateStopWatch(stopWatch *watch) {
    watch->s++;
    if(watch->s >= 60) {
        watch->s -= 60;
        watch->m++;
    }
    if(watch->m >= 60) {
        watch->m -= 60;
        watch->h++;
    }
}

void stopWatchToString(char *str,stopWatch *watch) {
    char h[3];
    char m[3];
    char s[3];
    sprintf(h,watch->h < 10 ?"0%d" : "%d",watch->h);
    sprintf(m,watch->m < 10 ?"0%d" : "%d",watch->m);
    sprintf(s,watch->s < 10 ?"0%d" : "%d",watch->s);
    sprintf(str,"%s:%s:%s",h,m,s);
}

int main (int argv, char** argc) {
    int mode = argv <= 1 ? 0 : 1;
    stopWatch watch = {
            .h = 0,
            .m = 0,
            .s = 0
    };
    time_t rawtime;
    struct tm * timeinfo;
    system(CLEAR);
    for(;;) {
        int idx = 0;
        int needed_ascii_nums[8];
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        char *time = argv <= 1 ? "" : calloc(9,sizeof(char));
        switch(mode) {
            case 0:
                time = get_time(asctime(timeinfo), strlen(asctime(timeinfo)));

                break;
            case 1:
                updateStopWatch(&watch);
                stopWatchToString(time,&watch);
                break;
        }

        //get the indices of the ascii numbers
        while (*time) {
            for(int i = 0; i < ASCII_NUMS_COUNT; ++i) {
                if(ascii_nums[i].number == *time) {
                    needed_ascii_nums[idx++] = i;
                }
            }
            time++;
        }

        //render line by line
        for(int i = 0; i < 6; ++i) {
            for(int j = 0; j < 8; ++j) {
                printf("%s",ascii_nums[needed_ascii_nums[j]].ascii[i]);
            }
            printf("\n");
        }
        //flush, because buffered and sleep
        fflush(stdout);
#ifdef _WIN32
        Sleep(1000)
#else
        sleep(1);
#endif
        system(CLEAR);
    }
    return 0;
}