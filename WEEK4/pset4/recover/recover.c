#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
const int HEADER_SIZE = 4;
const int BLOCK_SIZE = 512;
int isjpg(BYTE header[HEADER_SIZE]);

int main(int argc, char *argv[])
{
    if (argv[1] == NULL) {
        printf("Usage: ./recover image\n");
        return 1;
    }

    FILE *memc = fopen(argv[1], "r");
    if (memc == NULL) {
        return 1;
    }

    BYTE header[HEADER_SIZE];
    BYTE buffer[BLOCK_SIZE];
    int firstjpg = 1;
    int number_img = 0;

    char *imgname = malloc(7);
    if (imgname == NULL) {
        return 1;
    }

    sprintf(imgname, "%03i.jpg", number_img);
    FILE *jpg = fopen(imgname, "w");

    while(fread(&buffer, sizeof(BYTE), BLOCK_SIZE, memc)) {
        for (int i = 0; i < HEADER_SIZE; i++) {
            header[i] = buffer[i];
        }
        if (isjpg(header)) {

            if (firstjpg) {
                fwrite(&buffer, sizeof(BYTE), BLOCK_SIZE, jpg);
                firstjpg = 0;
            }
            else {
                fclose(jpg);
                number_img++;
                sprintf(imgname, "%03i.jpg", number_img);
                jpg = fopen(imgname, "w");
                fwrite(&buffer, sizeof(BYTE), BLOCK_SIZE, jpg);
            }

        }
        else {
            if (!firstjpg) {
                fwrite(&buffer, sizeof(BYTE), BLOCK_SIZE, jpg);
            }
        }
    }

    fclose(memc);
    fclose(jpg);
    free(imgname);
}

int isjpg(BYTE header[HEADER_SIZE]) {
    if (header[0] == 0xff && header[1] == 0xd8 && header[2] == 0xff && (header[3] & 0xf0) == 0xe0) {
        return 1;
    }
    return 0;
}