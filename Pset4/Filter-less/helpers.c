#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0, n = height; i < n; i++)
    {
        for (int j = 0, m = width; j < m; j++)
        {
            // get rounded average of triplet
            float avg = round((float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);

            // set all colors to rounded average
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0, n = height; i < n; i++)
    {
        for (int j = 0, m = width; j < m; j++)
        {
            // apply sepia formula to color values
            RGBTRIPLE pixel = image[i][j];
            int sepiaRed = round((float).393 * pixel.rgbtRed + .769 * pixel.rgbtGreen + .189 * pixel.rgbtBlue);
            int sepiaGreen = round((float).349 * pixel.rgbtRed + .686 * pixel.rgbtGreen + .168 * pixel.rgbtBlue);
            int sepiaBlue = round((float).272 * pixel.rgbtRed + .534 * pixel.rgbtGreen + .131 * pixel.rgbtBlue);

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            // set colors to new values
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;

        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    void swap(RGBTRIPLE * a, RGBTRIPLE * b); // prototype for swap function

    // for each pixel, swap with pixel on opposite side
    for (int i = 0, n = height; i < n; i++)
    {
        for (int j = 0, m = width; j < m / 2; j++)
        {
            swap(&image[i][j], &image[i][width - j - 1]);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // temporary image
    RGBTRIPLE temp[height][width];

    // loop through each pixel
    for (int i = 0, n = height; i < n; i++)
    {
        for (int j = 0, m = width; j < m; j++)
        {
            int total_red = 0;
            int total_green = 0;
            int total_blue = 0;
            int pixels = 0;

            // for each pixel, loop through pixels around it
            for (int k = i - 1, o = i + 1; k <= o; k++)
            {
                // if pixel on left or right
                if (k < 0 || k > height - 1)
                {
                    continue;
                }

                // if pixel on top or bottom
                for (int l = j - 1, p = j + 1; l <= p; l++)
                {
                    if (l < 0 || l > width - 1)
                    {
                        continue;
                    }

                    // add up totals of colors around pixel and number of pixels
                    total_red += image[k][l].rgbtRed;
                    total_green += image[k][l].rgbtGreen;
                    total_blue += image[k][l].rgbtBlue;
                    pixels++;
                }
            }

            // calculate values for each color
            int avg_red = round((float) total_red / pixels);
            int avg_green = round((float) total_green / pixels);
            int avg_blue = round((float) total_blue / pixels);

            // set temporary image pixels to correct colors
            temp[i][j].rgbtRed = avg_red;
            temp[i][j].rgbtGreen = avg_green;
            temp[i][j].rgbtBlue = avg_blue;
        }
    }

    // loop through pixels and set temporary color values to image values
    for (int i = 0, n = height; i < n; i++)
    {
        for (int j = 0, m = width; j < m; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }
    return;
}

// goes to adress of triples and sqaps them
void swap(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
