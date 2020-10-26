#include "helpers.h"
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // temporary image
    RGBTRIPLE temp[height][width];

    // create arrays for sobel's x and y values
    int sobel_x[3][3] = {
        { -1, 0, 1 },
        { -2, 0, 2 },
        { -1, 0, 1 },
    };

    int sobel_y[3][3] = {
        { -1, -2, -1 },
        { 0, 0, 0 },
        { 1, 2, 1 },
    };

    // loop through pixels
    for (int i = 0, n = height; i < n; i++)
    {
        for (int j = 0, m = width; j < m; j++)
        {
            int red_x = 0;
            int green_x = 0;
            int blue_x = 0;
            int red_y = 0;
            int green_y = 0;
            int blue_y = 0;


            for (int k = -1; k <= 1; k++)
            {
                if (i + k < 0 || i + k > height - 1)
                {
                    continue;
                }

                for (int l = -1; l <= 1; l++)
                {
                    if (j + l < 0 || j + l > width - 1)
                    {
                        continue;
                    }

                    red_x += image[i + k][j + l].rgbtRed * sobel_x[k + 1][l + 1];
                    green_x += image[i + k][j + l].rgbtGreen * sobel_x[k + 1][l + 1];
                    blue_x += image[i + k][j + l].rgbtBlue * sobel_x[k + 1][l + 1];

                    red_y += image[i + k][j + l].rgbtRed * sobel_y[k + 1][l + 1];
                    green_y += image[i + k][j + l].rgbtGreen * sobel_y[k + 1][l + 1];
                    blue_y += image[i + k][j + l].rgbtBlue * sobel_y[k + 1][l + 1];

                    int final_red = round((float) sqrt(red_x * red_x + red_y * red_y));
                    int final_green = round((float) sqrt(green_x * green_x + green_y * green_y));
                    int final_blue = round((float) sqrt(blue_x * blue_x + blue_y * blue_y));

                    if (final_red > 255)
                    {
                        final_red = 255;
                    }
                    if (final_green > 255)
                    {
                        final_green = 255;
                    }
                    if (final_blue > 255)
                    {
                        final_blue = 255;
                    }

                    temp[i][j].rgbtRed = final_red;
                    temp[i][j].rgbtGreen = final_green;
                    temp[i][j].rgbtBlue = final_blue;
                }
            }

        }
    }

    for (int i = 1, n = height - 1; i < n; i++)
    {
        for (int j = 1, m = width - 1; j < m; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }

    return;
}

// goes to adress of triples and swaps them
void swap(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
