#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
RGBTRIPLE blur_pixel(RGBTRIPLE arr[], int num_rgbt);

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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // loop through each pixel in image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // get the red, green, and blue for this pixel
            BYTE blue = image[i][j].rgbtBlue;
            BYTE green = image[i][j].rgbtGreen;
            BYTE red = image[i][j].rgbtRed;

            //sepia algorithm
            if (round(0.272 * red + 0.534 * green + 0.131 * blue) > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = round(0.272 * red + 0.534 * green + 0.131 * blue);
            }

            if (round(0.349 * red + 0.686 * green + 0.168 * blue) > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = round(0.349 * red + 0.686 * green + 0.168 * blue);
            }
            if (round(0.393 * red + 0.769 * green + 0.189 * blue) > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = round(0.393 * red + 0.769 * green + 0.189 * blue);
            }

            // assign the sepia color to red, green, and blue for this pixel


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