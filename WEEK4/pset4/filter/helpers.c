#include "helpers.h"
#include <math.h>
#include <stdio.h>


typedef struct {
    float frgbtBlue;
    float frgbtGreen;
    float frgbtRed;
}
FRGBTRIPLE;

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float shade;
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            shade = (float) (image[row][col].rgbtBlue + image[row][col].rgbtGreen + image[row][col].rgbtRed)/3;
            image[row][col].rgbtBlue = image[row][col].rgbtGreen = image[row][col].rgbtRed = (int) round(shade);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width/2; col++) {
            tmp = image[row][col];
            image[row][col] = image[row][width-col-1];
            image[row][width-col-1] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    FRGBTRIPLE blurred;
    int divider = 9;
    RGBTRIPLE original[height+2][width+2];
    for (int row = 0; row < height + 2; row++) {
        for (int col = 0; col < width + 2; col++) {
            if (row == 0 || row == height + 1 || col == 0 || col == width + 1) {
               original[row][col].rgbtBlue = original[row][col].rgbtGreen = original[row][col].rgbtRed = 0;
            }
            else {
                original[row][col] = image[row-1][col-1];
            }
        }
    }

    for (int row = 1; row < height + 1; row++) {
        for (int col = 1; col < width + 1; col++) {
            blurred.frgbtBlue = blurred.frgbtGreen = blurred.frgbtRed = 0;
            for (int i = row - 1; i < row + 2; i++) {
                for (int j = col -1; j < col + 2; j++) {
                    blurred.frgbtBlue += original[i][j].rgbtBlue;
                    blurred.frgbtGreen += original[i][j].rgbtGreen;
                    blurred.frgbtRed += original[i][j].rgbtRed;
                }
            }

            if (row == 1 || row == height) {
                if (col == 1 || col == width) {
                    divider = 4;
                }
                else {
                    divider = 6;
                }
            }
            else {
                if (col == 1 || col == width) {
                    divider = 6;
                }
                else {
                    divider = 9;
                }
            }

            image[row-1][col-1].rgbtBlue = (int) round(blurred.frgbtBlue/divider);
            image[row-1][col-1].rgbtGreen = (int) round(blurred.frgbtGreen/divider);
            image[row-1][col-1].rgbtRed = (int) round(blurred.frgbtRed/divider);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    FRGBTRIPLE edgex, edgey, edge;
    RGBTRIPLE original[height+2][width+2];
    for (int row = 0; row < height + 2; row++) {
        for (int col = 0; col < width + 2; col++) {
            if (row == 0 || row == height + 1 || col == 0 || col == width + 1) {
               original[row][col].rgbtBlue = original[row][col].rgbtGreen = original[row][col].rgbtRed = 0;
            }
            else {
                original[row][col] = image[row-1][col-1];
            }
        }
    }

    for (int row = 1; row < height + 1; row++) {
        for (int col = 1; col < width + 1; col++) {
            edgex.frgbtBlue = edgex.frgbtGreen = edgex.frgbtRed = 0;
            edgey.frgbtBlue = edgey.frgbtGreen = edgey.frgbtRed = 0;
            for (int i = row - 1; i < row + 2; i++) {
                for (int j = col -1; j < col + 2; j++) {
                    edgex.frgbtBlue += original[i][j].rgbtBlue*Gx[row-i+1][col-j+1];
                    edgex.frgbtGreen += original[i][j].rgbtGreen*Gx[row-i+1][col-j+1];
                    edgex.frgbtRed += original[i][j].rgbtRed*Gx[row-i+1][col-j+1];

                    edgey.frgbtBlue += original[i][j].rgbtBlue*Gy[row-i+1][col-j+1];
                    edgey.frgbtGreen += original[i][j].rgbtGreen*Gy[row-i+1][col-j+1];
                    edgey.frgbtRed += original[i][j].rgbtRed*Gy[row-i+1][col-j+1];

                }
            }
            edge.frgbtBlue = (int) round(sqrt(edgex.frgbtBlue*edgex.frgbtBlue + edgey.frgbtBlue*edgey.frgbtBlue));
            edge.frgbtGreen = (int) round(sqrt(edgex.frgbtGreen*edgex.frgbtGreen + edgey.frgbtGreen*edgey.frgbtGreen));
            edge.frgbtRed = (int) round(sqrt(edgex.frgbtRed*edgex.frgbtRed + edgey.frgbtRed*edgey.frgbtRed));

            image[row-1][col-1].rgbtBlue = edge.frgbtBlue > 255 ? 255 : edge.frgbtBlue;
            image[row-1][col-1].rgbtGreen = edge.frgbtGreen > 255 ? 255 : edge.frgbtGreen;
            image[row-1][col-1].rgbtRed = edge.frgbtRed > 255 ? 255 : edge.frgbtRed;

        }
    }

    return;
}

