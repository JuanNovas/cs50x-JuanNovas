#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int x = 0; x < width; x++)
        {
            int num = round((image[i][x].rgbtBlue + image[i][x].rgbtGreen + image[i][x].rgbtRed) / 3.0);
            image[i][x].rgbtBlue = num;
            image[i][x].rgbtGreen = num;
            image[i][x].rgbtRed = num;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer;
    for (int i = 0; i < height; i++)
    {
        for (int x = 0; x < width / 2; x++)
        {
            buffer.rgbtBlue = image[i][x].rgbtBlue;
            buffer.rgbtGreen = image[i][x].rgbtGreen;
            buffer.rgbtRed = image[i][x].rgbtRed;
            image[i][x].rgbtBlue = image[i][width - 1 - x].rgbtBlue;
            image[i][x].rgbtGreen = image[i][width - 1 - x].rgbtGreen;
            image[i][x].rgbtRed = image[i][width - 1 - x].rgbtRed;
            image[i][width - 1 - x].rgbtBlue = buffer.rgbtBlue;
            image[i][width - 1 - x].rgbtGreen = buffer.rgbtGreen;
            image[i][width - 1 - x].rgbtRed = buffer.rgbtRed;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    float bufferBlue = 0;
    float bufferGreen = 0;
    float bufferRed = 0;
    int n = 0;
    int ultrabufferBlue[height][width];
    int ultrabufferGreen[height][width];
    int ultrabufferRed[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int x = 0; x < width; x++)
        {
            bufferBlue = 0;
            bufferGreen = 0;
            bufferRed = 0;
            n = 0;
            if (i > 0)
            {
                bufferBlue += image[i - 1][x].rgbtBlue;
                bufferGreen += image[i - 1][x].rgbtGreen;
                bufferRed += image[i - 1][x].rgbtRed;
                n++;
                if (x > 0)
                {
                    bufferBlue += image[i - 1][x - 1].rgbtBlue;
                    bufferGreen += image[i - 1][x - 1].rgbtGreen;
                    bufferRed += image[i - 1][x - 1].rgbtRed;
                    n++;
                }
                if (x < width - 1)
                {
                    bufferBlue += image[i - 1][x + 1].rgbtBlue;
                    bufferGreen += image[i - 1][x + 1].rgbtGreen;
                    bufferRed += image[i - 1][x + 1].rgbtRed;
                    n++;
                }
            }
            if (i < height - 1)
            {
                bufferBlue += image[i + 1][x].rgbtBlue;
                bufferGreen += image[i + 1][x].rgbtGreen;
                bufferRed += image[i + 1][x].rgbtRed;
                n++;
                if (x > 0)
                {
                    bufferBlue += image[i + 1][x - 1].rgbtBlue;
                    bufferGreen += image[i + 1][x - 1].rgbtGreen;
                    bufferRed += image[i + 1][x - 1].rgbtRed;
                    n++;
                }
                if (x < width - 1)
                {
                    bufferBlue += image[i + 1][x + 1].rgbtBlue;
                    bufferGreen += image[i + 1][x + 1].rgbtGreen;
                    bufferRed += image[i + 1][x + 1].rgbtRed;
                    n++;
                }
            }
            if (x > 0)
            {
                bufferBlue += image[i][x - 1].rgbtBlue;
                bufferGreen += image[i][x - 1].rgbtGreen;
                bufferRed += image[i][x - 1].rgbtRed;
                n++;
            }
            if (x < width - 1)
            {
                bufferBlue += image[i][x + 1].rgbtBlue;
                bufferGreen += image[i][x + 1].rgbtGreen;
                bufferRed += image[i][x + 1].rgbtRed;
                n++;
            }
            ultrabufferBlue[i][x] = round((bufferBlue + image[i][x].rgbtBlue) / (n + 1));
            ultrabufferGreen[i][x] = round((bufferGreen + image[i][x].rgbtGreen) / (n + 1));
            ultrabufferRed[i][x] = round((bufferRed + image[i][x].rgbtRed) / (n + 1));
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int x = 0; x < width; x++)
        {
            image[i][x].rgbtBlue = ultrabufferBlue[i][x];
            image[i][x].rgbtGreen = ultrabufferGreen[i][x];
            image[i][x].rgbtRed = ultrabufferRed[i][x];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int gxBlue[height][width];
    int gxGreen[height][width];
    int gxRed[height][width];
    int gyBlue[height][width];
    int gyGreen[height][width];
    int gyRed[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int x = 0; x < width; x++)
        {
            gxBlue[i][x] = 0;
            gxGreen[i][x] = 0;
            gxRed[i][x] = 0;
            gyBlue[i][x] = 0;
            gyGreen[i][x] = 0;
            gyRed[i][x] = 0;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int x = 0; x < width; x++)
        {
            if (i > 0)
            {
                gyBlue[i][x] += image[i - 1][x].rgbtBlue * -2;
                gyGreen[i][x] += image[i - 1][x].rgbtGreen * -2;
                gyRed[i][x] += image[i - 1][x].rgbtRed * -2;

                if (x > 0)
                {
                    gyBlue[i][x] += image[i - 1][x - 1].rgbtBlue * -1;
                    gyGreen[i][x] += image[i - 1][x - 1].rgbtGreen * -1;
                    gyRed[i][x] += image[i - 1][x - 1].rgbtRed * -1;

                    gxBlue[i][x] += image[i - 1][x - 1].rgbtBlue * -1;
                    gxGreen[i][x] += image[i - 1][x - 1].rgbtGreen * -1;
                    gxRed[i][x] += image[i - 1][x - 1].rgbtRed * -1;
                }
                if (x < width - 1)
                {
                    gyBlue[i][x] += image[i - 1][x + 1].rgbtBlue * -1;
                    gyGreen[i][x] += image[i - 1][x + 1].rgbtGreen * -1;
                    gyRed[i][x] += image[i - 1][x + 1].rgbtRed * -1;

                    gxBlue[i][x] += image[i - 1][x + 1].rgbtBlue;
                    gxGreen[i][x] += image[i - 1][x + 1].rgbtGreen;
                    gxRed[i][x] += image[i - 1][x + 1].rgbtRed;
                }
            }
            if (i < height - 1)
            {
                gyBlue[i][x] += image[i + 1][x].rgbtBlue * 2;
                gyGreen[i][x] += image[i + 1][x].rgbtGreen * 2;
                gyRed[i][x] += image[i + 1][x].rgbtRed * 2;

                if (x > 0)
                {
                    gyBlue[i][x] += image[i + 1][x - 1].rgbtBlue;
                    gyGreen[i][x] += image[i + 1][x - 1].rgbtGreen;
                    gyRed[i][x] += image[i + 1][x - 1].rgbtRed;

                    gxBlue[i][x] += image[i + 1][x - 1].rgbtBlue * -1;
                    gxGreen[i][x] += image[i + 1][x - 1].rgbtGreen * -1;
                    gxRed[i][x] += image[i + 1][x - 1].rgbtRed * -1;
                }
                if (x < width - 1)
                {
                    gyBlue[i][x] += image[i + 1][x + 1].rgbtBlue;
                    gyGreen[i][x] += image[i + 1][x + 1].rgbtGreen;
                    gyRed[i][x] += image[i + 1][x + 1].rgbtRed;

                    gxBlue[i][x] += image[i + 1][x + 1].rgbtBlue;
                    gxGreen[i][x] += image[i + 1][x + 1].rgbtGreen;
                    gxRed[i][x] += image[i + 1][x + 1].rgbtRed;
                }
            }
            if (x > 0)
            {
                gxBlue[i][x] += image[i][x - 1].rgbtBlue * -2;
                gxGreen[i][x] += image[i][x - 1].rgbtGreen * -2;
                gxRed[i][x] += image[i][x - 1].rgbtRed * -2;
            }
            if (x < width - 1)
            {
                gxBlue[i][x] += image[i][x + 1].rgbtBlue * 2;
                gxGreen[i][x] += image[i][x + 1].rgbtGreen * 2;
                gxRed[i][x] += image[i][x + 1].rgbtRed * 2;
            }
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int x = 0; x < width; x++)
        {
            int blue = round(sqrt((gxBlue[i][x] * gxBlue[i][x]) + (gyBlue[i][x] * gyBlue[i][x])));
            if (blue > 255)
            {
                blue = 255;
            }
            int green = round(sqrt((gxGreen[i][x] * gxGreen[i][x]) + (gyGreen[i][x] * gyGreen[i][x])));
            if (green > 255)
            {
                green = 255;
            }
            int red = round(sqrt((gxRed[i][x] * gxRed[i][x]) + (gyRed[i][x] * gyRed[i][x])));
            if (red > 255)
            {
                red = 255;
            }
            image[i][x].rgbtBlue = blue;
            image[i][x].rgbtGreen = green;
            image[i][x].rgbtRed = red;
        }
    }
    return;
}
