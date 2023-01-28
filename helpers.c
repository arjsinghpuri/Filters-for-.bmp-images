#include "helpers.h"
#include <math.h>
#include<stdio.h>
#include<stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE new_triplet;
            float avg = (image[i][j].rgbtBlue +  image[i][j].rgbtGreen +  image[i][j].rgbtRed) / 3.0;
            new_triplet.rgbtBlue = (int)round(avg);
            new_triplet.rgbtGreen = (int)round(avg);
            new_triplet.rgbtRed = (int)round(avg);
            image[i][j] = new_triplet;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp_triplet;
            temp_triplet.rgbtBlue = image[i][j].rgbtBlue;
            temp_triplet.rgbtGreen = image[i][j].rgbtGreen;
            temp_triplet.rgbtRed = image[i][j].rgbtRed;

            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;

            image[i][width - 1 - j].rgbtBlue = temp_triplet.rgbtBlue;
            image[i][width - 1 - j].rgbtGreen = temp_triplet.rgbtGreen;
            image[i][width  - 1 - j].rgbtRed = temp_triplet.rgbtRed;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE new_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int bvals[9] = {image[i][j].rgbtBlue, 0, 0, 0, 0, 0, 0, 0, 0};
            int gvals[9] = {image[i][j].rgbtGreen, 0, 0, 0, 0, 0, 0, 0, 0};
            int rvals[9] = {image[i][j].rgbtRed, 0, 0, 0, 0, 0, 0, 0, 0};
            int index = 1;

            if (j - 1 >= 0)
            {
                bvals[index] = image[i][j - 1].rgbtBlue;
                gvals[index] = image[i][j - 1].rgbtGreen;
                rvals[index] = image[i][j - 1].rgbtRed;
                index++;
            }
            if (j + 1 < width)
            {
                bvals[index] = image[i][j + 1].rgbtBlue;
                gvals[index] = image[i][j + 1].rgbtGreen;
                rvals[index] = image[i][j + 1].rgbtRed;
                index++;
            }
            if (i - 1 >= 0)
            {
                bvals[index] = image[i - 1][j].rgbtBlue;
                gvals[index] = image[i - 1][j].rgbtGreen;
                rvals[index] = image[i - 1][j].rgbtRed;
                index++;
            }
            if (i + 1 < height)
            {
                bvals[index] = image[i + 1][j].rgbtBlue;
                gvals[index] = image[i + 1][j].rgbtGreen;
                rvals[index] = image[i + 1][j].rgbtRed;
                index++;
            }
            if ((i - 1 >= 0) && (j - 1 >= 0))
            {
                bvals[index] = image[i - 1][j - 1].rgbtBlue;
                gvals[index] = image[i - 1][j - 1].rgbtGreen;
                rvals[index] = image[i - 1][j - 1].rgbtRed;
                index++;
            }
            if ((i - 1 >= 0) && (j + 1 < width))
            {
                bvals[index] = image[i - 1][j + 1].rgbtBlue;
                gvals[index] = image[i - 1][j + 1].rgbtGreen;
                rvals[index] = image[i - 1][j + 1].rgbtRed;
                index++;
            }
            if ((i + 1 < height) && (j + 1 < width))
            {
                bvals[index] = image[i + 1][j + 1].rgbtBlue;
                gvals[index] = image[i + 1][j + 1].rgbtGreen;
                rvals[index] = image[i + 1][j + 1].rgbtRed;
                index++;
            }
            if ((i + 1 < height) && (j - 1 >= 0))
            {
                bvals[index] = image[i + 1][j - 1].rgbtBlue;
                gvals[index] = image[i + 1][j - 1].rgbtGreen;
                rvals[index] = image[i + 1][j - 1].rgbtRed;
                index++;
            }

            int bsum = 0;
            int gsum = 0;
            int rsum = 0;

            for (int val = 0; val < 9; val++)
            {
                bsum += bvals[val];
                gsum += gvals[val];
                rsum += rvals[val];
            }

            new_image[i][j].rgbtBlue = (int)round(bsum / (float)index);
            new_image[i][j].rgbtGreen = (int)round(gsum / (float)index);
            new_image[i][j].rgbtRed = (int)round(rsum / (float)index);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = new_image[i][j].rgbtBlue;
            image[i][j].rgbtGreen = new_image[i][j].rgbtGreen;
            image[i][j].rgbtRed = new_image[i][j].rgbtRed;
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE edge_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int bvals[9] = {image[i][j].rgbtBlue, 0, 0, 0, 0, 0, 0, 0, 0};
            int gvals[9] = {image[i][j].rgbtGreen, 0, 0, 0, 0, 0, 0, 0, 0};
            int rvals[9] = {image[i][j].rgbtRed, 0, 0, 0, 0, 0, 0, 0, 0};
            int index = 1;

            int x[9] = {0, -1, 0, 1, -2, 2, -1, 0, 1};
            int y[9] = {0, -1, -2, -1, 0, 0, 1, 2, 1};

            if ((i - 1 >= 0) && (j - 1 >= 0))
            {
                bvals[index] = image[i - 1][j - 1].rgbtBlue;
                gvals[index] = image[i - 1][j - 1].rgbtGreen;
                rvals[index] = image[i - 1][j - 1].rgbtRed;
            }
            index++;
            if (i - 1 >= 0)
            {
                bvals[index] = image[i - 1][j].rgbtBlue;
                gvals[index] = image[i - 1][j].rgbtGreen;
                rvals[index] = image[i - 1][j].rgbtRed;
            }
            index++;
            if ((i - 1 >= 0) && (j + 1 < width))
            {
                bvals[index] = image[i - 1][j + 1].rgbtBlue;
                gvals[index] = image[i - 1][j + 1].rgbtGreen;
                rvals[index] = image[i - 1][j + 1].rgbtRed;
            }
            index++;
            if (j - 1 >= 0)
            {
                bvals[index] = image[i][j - 1].rgbtBlue;
                gvals[index] = image[i][j - 1].rgbtGreen;
                rvals[index] = image[i][j - 1].rgbtRed;
            }
            index++;
            if (j + 1 < width)
            {
                bvals[index] = image[i][j + 1].rgbtBlue;
                gvals[index] = image[i][j + 1].rgbtGreen;
                rvals[index] = image[i][j + 1].rgbtRed;
            }
            index++;
            if ((i + 1 < height) && (j - 1 >= 0))
            {
                bvals[index] = image[i + 1][j - 1].rgbtBlue;
                gvals[index] = image[i + 1][j - 1].rgbtGreen;
                rvals[index] = image[i + 1][j - 1].rgbtRed;
            }
            index++;
            if (i + 1 < height)
            {
                bvals[index] = image[i + 1][j].rgbtBlue;
                gvals[index] = image[i + 1][j].rgbtGreen;
                rvals[index] = image[i + 1][j].rgbtRed;
            }
            index++;
            if ((i + 1 < height) && (j + 1 < width))
            {
                bvals[index] = image[i + 1][j + 1].rgbtBlue;
                gvals[index] = image[i + 1][j + 1].rgbtGreen;
                rvals[index] = image[i + 1][j + 1].rgbtRed;
            }

            int bx = 0, gx = 0, rx = 0, by = 0, gy = 0, ry = 0;

            for (int xyz = 0; xyz < 9; xyz++)
            {
                bx += bvals[xyz] * x[xyz];
                by += bvals[xyz] * y[xyz];

                gx += gvals[xyz] * x[xyz];
                gy += gvals[xyz] * y[xyz];

                rx += rvals[xyz] * x[xyz];
                ry += rvals[xyz] * y[xyz];
            }

            if (sqrt((bx * bx) + (by * by)) > 255)
            {
                edge_image[i][j].rgbtBlue = 255;
            }
            else
            {
                edge_image[i][j].rgbtBlue = round(sqrt((bx * bx) + (by * by)));
            }
            if (sqrt((gx * gx) + (gy * gy)) > 255)
            {
                edge_image[i][j].rgbtGreen = 255;
            }
            else
            {
                edge_image[i][j].rgbtGreen = round(sqrt((gx * gx) + (gy * gy)));
            }
            if (sqrt((rx * rx) + (ry * ry)) > 255)
            {
                edge_image[i][j].rgbtRed = 255;
            }
            else
            {
                edge_image[i][j].rgbtRed = round(sqrt((rx * rx) + (ry * ry)));
            }
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = edge_image[i][j].rgbtBlue;
            image[i][j].rgbtGreen = edge_image[i][j].rgbtGreen;
            image[i][j].rgbtRed = edge_image[i][j].rgbtRed;
        }
    }
    return;
}
