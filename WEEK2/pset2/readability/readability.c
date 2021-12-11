#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
int main(void) {

    string text = get_string("Text : ");
    float letter = 0, word = 0, sentence = 0;

    for(int i = 0, n = strlen(text); i < n; i++) {
        int ascii = (int) toupper(text[i]);
        if (ascii >= 65 && ascii <= 90) {
            letter += 1;
        }
        else if (ascii == 32) {
            word += 1;
        }
        else if (ascii == 46 || ascii == 33 || ascii == 63) {
            sentence += 1;
        }
    }
    word += 1;

    float index = 0.0588 * (letter/(word/100)) - 0.296 * (sentence/(word/100)) - 15.8;

    if (index < 1) {
        printf("Before Grade 1\n");
    }
    else if (index > 16) {
        printf("Grade 16+\n");
    }
    else {
        printf("Grade %d\n", (int) round(index));
    }
}

