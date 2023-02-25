#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include "include/ascii_numbers.h"

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

char *get_time(char *raw_time, size_t len) {
    char *t_str;
    t_str = strtok(raw_time, " ");
    while (*t_str) {
        t_str = strtok(NULL," ");
        if(strchr(t_str,':') != NULL) {
            return t_str;
        }
    }

}
int main (void) {
    time_t rawtime;
    struct tm * timeinfo;
    system(CLEAR);
    for(;;) {
        int idx = 0;
        int needed_ascii_nums[8];
        time ( &rawtime );
        timeinfo = localtime ( &rawtime );
        char *time = get_time(asctime(timeinfo), strlen(asctime(timeinfo)));

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
        sleep(1);
        system(CLEAR);
    }
    return 0;
}