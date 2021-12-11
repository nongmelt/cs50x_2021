#include <stdio.h>
#include <cs50.h>

int main(void) {
    long id;
    id = get_long("Number: ");
    int even=0, odd=0, count=1, check=0;

    do {
        if (count%2==1) {
            odd += id%10;
        }
        else {
            if ((id%10)*2 > 9) {
                even += 1 + ((id%10)*2)%10 ;
            }
            else {
                even += (id%10)*2;
            }
        }

        if (id%100 !=0 && id > 10) {
            check = id;
        }
        count++;
        id /= 10;
    } while(id!=0);

    if (count < 14 || count > 17 || (even+odd)%10!=0) {
        printf("INVALID\n");
        return 0;
    }


    if (check/10==4) {
        printf("VISA\n");
    }
    else if (check==34 || check==37) {
        printf("AMEX\n");
    }
    else if (check>=51 && check<=55) {
        printf("MASTERCARD\n");
    }
    else {
        printf("INVALID\n");
    }



}