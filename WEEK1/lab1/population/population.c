#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void) {
    int start, end, year=0;
    do {
        start = get_int("Start size : ");
    }
    while (start<9);

    do {
        end = get_int("End size : ");
    }
    while (end<start);

    while (start<end) {
        start = start + start/3 - start/4;
        year++;
    }
    printf("Years: %i",year);
 }