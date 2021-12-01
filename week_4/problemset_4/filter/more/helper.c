#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
RGBTRIPLE blur_pixel(RGBTRIPLE arr[], int num_rgbt);
RGBTRIPLE weighted_sum(RGBTRIPLE pixel_box[9] , int Gx[9], int Gy[9]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // loop through each pixel in image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // average the red, green, and blue for each pixel
            BYTE blue = image[i][j].rgbtBlue;
            BYTE green = image[i][j].rgbtGreen;
            BYTE red = image[i][j].rgbtRed;
            BYTE average = round((blue + green + red) / 3.0);

            // assign the average to red, green, and blue for each pixel
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // buffer for swapping
    RGBTRIPLE buffer;
    // loop through each pixel in the image to store it into temp
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            buffer = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = buffer;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // allocate temp memory for blurring
    RGBTRIPLE(*temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    // loop through the image to calculate blurred pixels
    // and store them in temp

    // the 3x3 pixel box around the target pixel
    RGBTRIPLE pixel_box[9];

    // the middle part
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            pixel_box[0] = image[i - 1][j - 1];
            pixel_box[1] = image[i - 1][j];
            pixel_box[2] = image[i - 1][j + 1];
            pixel_box[3] = image[i][j - 1];
            pixel_box[4] = image[i][j];
            pixel_box[5] = image[i][j + 1];
            pixel_box[6] = image[i + 1][j - 1];
            pixel_box[7] = image[i + 1][j];
            pixel_box[8] = image[i + 1][j + 1];
            temp[i][j] = blur_pixel(pixel_box, 9);
        }
    }

    // top left corner
    pixel_box[0] = image[0][0];
    pixel_box[1] = image[0][1];
    pixel_box[2] = image[1][0];
    pixel_box[3] = image[1][1];
    temp[0][0] = blur_pixel(pixel_box, 4);

    // top right corner
    pixel_box[0] = image[0][width - 2];
    pixel_box[1] = image[0][width - 1];
    pixel_box[2] = image[1][width - 2];
    pixel_box[3] = image[1][width - 1];
    temp[0][width - 1] = blur_pixel(pixel_box, 4);

    // bottom left corner
    pixel_box[0] = image[height - 2][0];
    pixel_box[1] = image[height - 2][1];
    pixel_box[2] = image[height - 1][0];
    pixel_box[3] = image[height - 1][1];
    temp[height - 1][0] = blur_pixel(pixel_box, 4);

    //bottom right corner
    pixel_box[0] = image[height - 2][width - 2];
    pixel_box[1] = image[height - 2][width - 1];
    pixel_box[2] = image[height - 1][width - 2];
    pixel_box[3] = image[height - 1][width - 1];
    temp[height - 1][width - 1] = blur_pixel(pixel_box, 4);


    // top/bottom row excluding corners
    for (int j = 1; j < width - 1; j++)
    {
        // top row
        pixel_box[0] = image[0][j - 1];
        pixel_box[1] = image[0][j];
        pixel_box[2] = image[0][j + 1];
        pixel_box[3] = image[1][j - 1];
        pixel_box[4] = image[1][j];
        pixel_box[5] = image[1][j + 1];
        temp[0][j] = blur_pixel(pixel_box, 6);

        // bottom row
        pixel_box[0] = image[height - 2][j - 1];
        pixel_box[1] = image[height - 2][j];
        pixel_box[2] = image[height - 2][j + 1];
        pixel_box[3] = image[height - 1][j - 1];
        pixel_box[4] = image[height - 1][j];
        pixel_box[5] = image[height - 1][j + 1];
        temp[height - 1][j] = blur_pixel(pixel_box, 6);
    }

    // left/right most colomn
    for (int i = 1; i < height - 1; i++)
    {
        // left column
        pixel_box[0] = image[i - 1][0];
        pixel_box[1] = image[i - 1][1];
        pixel_box[2] = image[i][0];
        pixel_box[3] = image[i][1];
        pixel_box[4] = image[i + 1][0];
        pixel_box[5] = image[i + 1][1];
        temp[i][0] = blur_pixel(pixel_box, 6);

        // right column
        pixel_box[0] = image[i - 1][width - 2];
        pixel_box[1] = image[i - 1][width - 1];
        pixel_box[2] = image[i][width - 2];
        pixel_box[3] = image[i][width - 1];
        pixel_box[4] = image[i + 1][width - 2];
        pixel_box[5] = image[i + 1][width - 1];
        temp[i][width - 1] = blur_pixel(pixel_box, 6);
    }



    // save the pixels from temp into image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    // free the temp memory
    free(temp);
    return;
}

RGBTRIPLE blur_pixel(RGBTRIPLE arr[], int num_rgbt)
{
    int red_sum = 0;
    int green_sum = 0;
    int blue_sum = 0;

    for (int i = 0; i < num_rgbt; i++)
    {
        red_sum += arr[i].rgbtRed;
        green_sum += arr[i].rgbtGreen;
        blue_sum += arr[i].rgbtBlue;
    }
    RGBTRIPLE blurred;
    blurred.rgbtRed = (BYTE) round((float)red_sum / num_rgbt);
    blurred.rgbtGreen = (BYTE) round((float) green_sum / num_rgbt);
    blurred.rgbtBlue = (BYTE) round((float) blue_sum / num_rgbt);
    return blurred;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // create the kernels
    int Gx[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int Gy[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    // allocate temp memory for blurring
    RGBTRIPLE(*temp)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    // declare a 3x3 pixel box around the current pixel
    RGBTRIPLE pixel_box[9];
    // assign the black border
    RGBTRIPLE black_border;
    black_border.rgbtRed = 0;
    black_border.rgbtGreen = 0;
    black_border.rgbtBlue = 0;

    // go through each pixel to get the weighted pixels in temp

    // top left corner
    pixel_box[0] = black_border;
    pixel_box[1] = black_border;
    pixel_box[2] = black_border;
    pixel_box[3] = black_border;
    pixel_box[4] = image[0][0];
    pixel_box[5] = image[0][1];
    pixel_box[6] = black_border;
    pixel_box[7] = image[1][0];
    pixel_box[8] = image[1][1];
    temp[0][0] = weighted_sum(pixel_box, Gx, Gy);

    // top right corner
    pixel_box[0] = black_border;
    pixel_box[1] = black_border;
    pixel_box[2] = black_border;
    pixel_box[3] = image[0][width - 2];
    pixel_box[4] = image[0][width - 1];
    pixel_box[5] = black_border;
    pixel_box[6] = image[1][width - 2];
    pixel_box[7] = image[1][width - 1];
    pixel_box[8] = black_border;
    temp[0][width - 1] = weighted_sum(pixel_box, Gx, Gy);

    // bottom left corner
    pixel_box[0] = black_border;
    pixel_box[1] = image[height - 2][0];
    pixel_box[2] = image[height - 2][1];
    pixel_box[3] = black_border;
    pixel_box[4] = image[height - 1][0];
    pixel_box[5] = image[height - 1][1];
    pixel_box[6] = black_border;
    pixel_box[7] = black_border;
    pixel_box[8] = black_border;
    temp[height - 1][0] = weighted_sum(pixel_box, Gx, Gy);

    // bottom right corder
    pixel_box[0] = image[height - 2][width - 2];
    pixel_box[1] = image[height - 2][width - 1];
    pixel_box[2] = black_border;
    pixel_box[3] = image[height - 1][width - 2];
    pixel_box[4] = image[height - 1][width - 1];
    pixel_box[5] = black_border;
    pixel_box[6] = black_border;
    pixel_box[7] = black_border;
    pixel_box[8] = black_border;
    temp[height - 1][width - 1] = weighted_sum(pixel_box, Gx, Gy);

    // top row
    for (int j = 1; j < width - 1; j++)
    {
        pixel_box[0] = black_border;
        pixel_box[1] = black_border;
        pixel_box[2] = black_border;
        pixel_box[3] = image[0][j - 1];
        pixel_box[4] = image[0][j];
        pixel_box[5] = image[0][j + 1];
        pixel_box[6] = image[1][j - 1];
        pixel_box[7] = image[1][j];
        pixel_box[8] = image[1][j + 1];
        temp[0][j] = weighted_sum(pixel_box, Gx, Gy);
    }

    // bottom row
    for (int j = 1; j < width - 1; j++)
    {
        pixel_box[0] = image[height - 2][j - 1];
        pixel_box[1] = image[height - 2][j];
        pixel_box[2] = image[height - 2][j + 1];
        pixel_box[3] = image[height - 1][j - 1];
        pixel_box[4] = image[height - 1][j];
        pixel_box[5] = image[height - 1][j + 1];
        pixel_box[6] = black_border;
        pixel_box[7] = black_border;
        pixel_box[8] = black_border;
        temp[height - 1][j] = weighted_sum(pixel_box, Gx, Gy);
    }

    // left column
    for (int i = 1; i < height - 1; i++)
    {
        pixel_box[0] = black_border;
        pixel_box[1] = image[i - 1][0];
        pixel_box[2] = image[i - 1][1];
        pixel_box[3] = black_border;
        pixel_box[4] = image[i][0];
        pixel_box[5] = image[i][1];
        pixel_box[6] = black_border;
        pixel_box[7] = image[i + 1][0];
        pixel_box[8] = image[i + 1][1];
        temp[i][0] = weighted_sum(pixel_box, Gx, Gy);
    }

    // right column
    for (int i = 1; i < height - 1; i++)
    {
        pixel_box[0] = image[i - 1][width - 2];
        pixel_box[1] = image[i - 1][width - 1];
        pixel_box[2] = black_border;
        pixel_box[3] = image[i][width - 2];
        pixel_box[4] = image[i][width - 1];
        pixel_box[5] = black_border;
        pixel_box[6] = image[i + 1][width - 2];
        pixel_box[7] = image[i + 1][width - 1];
        pixel_box[8] = black_border;
        temp[i][width - 1] = weighted_sum(pixel_box, Gx, Gy);
    }

    // middle pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixel_box[0] = image[i - 1][j - 1];
            pixel_box[1] = image[i - 1][j];
            pixel_box[2] = image[i - 1][j + 1];
            pixel_box[3] = image[i][j - 1];
            pixel_box[4] = image[i][j];
            pixel_box[5] = image[i][j + 1];
            pixel_box[6] = image[i + 1][j - 1];
            pixel_box[7] = image[i + 1][j];
            pixel_box[8] = image[i + 1][j + 1];
            temp[i][j] = weighted_sum(pixel_box, Gx, Gy);
        }
    }

    // loop through each pixel again to get the temp value saved into image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
            printf("%i, %i, %i\n", temp[i][j].rgbtRed, temp[i][j].rgbtGreen, temp[i][j].rgbtBlue);
        }
    }
    free(temp);
    return;
}

RGBTRIPLE weighted_sum(RGBTRIPLE pixel_box[9] , int Gx[9], int Gy[9])
{
    // print out the pixel box
    /*
    for (int i = 0; i < 9; i++)
    {
        printf("pixel_box[%i] Red: %i\n", i, pixel_box[i].rgbtRed);
    }
    */

    // declare the variables to keep track of the sums
    int Gx_sum_red = 0;
    int Gy_sum_red = 0;
    int Gx_sum_green = 0;
    int Gy_sum_green = 0;
    int Gx_sum_blue = 0;
    int Gy_sum_blue = 0;
    // loop through the pixel box
    for (int m = 0; m < 9; m++)
    {
        // get the sum by multiplying the pixel box with the kernels
        Gx_sum_red += pixel_box[m].rgbtRed * Gx[m];
        //printf("%i\n", pixel_box[m].rgbtRed);
        //printf("%i\n", Gx[m]);
        //printf("%i\n", Gx_sum_red);
        Gy_sum_red += pixel_box[m].rgbtRed * Gy[m];
        Gx_sum_green += pixel_box[m].rgbtGreen * Gx[m];
        Gy_sum_green += pixel_box[m].rgbtGreen * Gy[m];
        Gx_sum_blue += pixel_box[m].rgbtBlue * Gx[m];
        Gy_sum_blue += pixel_box[m].rgbtBlue * Gy[m];
    }
    // print
    printf("%i\n", Gx_sum_red);
    printf("%i\n", Gy_sum_red);
    // get the weighted sum from Gx and Gy sum
    int weighted_red = round(sqrt(pow(Gx_sum_red, 2) + pow(Gy_sum_red, 2)));
    int weighted_green = round(sqrt(pow(Gx_sum_green, 2) + pow(Gy_sum_green, 2)));
    int weighted_blue = round(sqrt(pow(Gx_sum_blue, 2) + pow(Gy_sum_blue, 2)));
    printf("%f\n", sqrt(pow(Gx_sum_red, 2) + pow(Gy_sum_red, 2)));
    printf("%i\n", weighted_red);

    // construct the weighted pixel
    RGBTRIPLE weighted_pixel;
    weighted_pixel.rgbtRed = weighted_red;
    weighted_pixel.rgbtGreen = weighted_green;
    weighted_pixel.rgbtBlue = weighted_blue;
    // return the weighted pixel
    return weighted_pixel;
}