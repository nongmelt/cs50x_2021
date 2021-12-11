#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int isalphabet (char c);
int isduplicate (string text);

int main(int argc, string argv[]) {

    if (argc != 2 || (int) toupper(argv[1][0]) < 65 || (int) toupper(argv[1][0]) > 90){
        printf("Usage: ./substitution key\n");
        return 1;
    }
    int m = strlen(argv[1]);
    if (m != 26) {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    if (isduplicate(argv[1])) {
        return 1;
    }
    string plain = get_string("plaintext: ");

    printf("ciphertext: ");
    for (int i = 0, n = strlen(plain); i < n; i++) {
        if (isalphabet(plain[i])) {
            if (islower(plain[i])) {
                printf("%c",tolower(argv[1][(int) plain[i] - 97]));
            }
            else {
                printf("%c",toupper(argv[1][(int) plain[i] - 65]));
            }
        }
        else {
            printf("%c",plain[i]);
        }
    }
    printf("\n");
}

int isalphabet(char c) {
    if ((int) toupper(c) >= 65 && (int) toupper(c) <= 90) {
        return 1;
    }
    return 0;
}

int isduplicate(string text) {
    int check = 2015;
    for (int i = 0, n = strlen(text); i < n; i++) {
        check -= (int) toupper(text[i]);
    }
    if (check != 0) {
        return 1;
    }
    return 0;
}