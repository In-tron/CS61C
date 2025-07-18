/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
	FILE *fp = fopen(filename, "r");
	char buff[20];  // Buffer to hold the first line of the file
	uint32_t column, row;  // Number of columns and rows in the image  
	uint8_t pixel;  // Maximum pixel value (usually 255)
	fscanf(fp, "%s", buff);  // Read the first line (should be "P3")
	fscanf(fp, "%u %u", &column, &row);  // Read the second line (width and height)
	fscanf(fp, "%hhu", &pixel);  // Read the third line (maximum pixel value)
	
	// Now we're going to get the image data.
	Image* image = (Image*) malloc(sizeof(Image)); // Allocate memory for the Image structure
	image->rows = row;  // Set the number of rows in the image
	image->cols = column;  // Set the number of columns in the image
	image->image = (Color**) malloc(sizeof(Color*) * image->rows);  // Allocate memory for the rows of the image
	int i, j;  // Loop variables for iterating through the image pixels
	uint32_t R, G, B;  // Temporary variables to hold the RGB values
	for (i = 0; i < image->rows; i++) {

		// Forget this line at first, which causes Segmentation Fault.
		image->image[i] = (Color*) malloc(sizeof(Color) * image->cols);  // Allocate memory for each row of pixels
		
		for (j = 0; j < image->cols; j++) {
			fscanf(fp, "%u %u %u", &R, &G, &B);
			Color* tmp = &image->image[i][j]; // Allocate memory for each pixel's color
			tmp->R = (uint8_t)R;
			tmp->G = (uint8_t)G;
			tmp->B = (uint8_t)B;
		}
	}
	// Close the file.
	fclose(fp);

	return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	printf("P3\n");
	printf("%d %d\n", image->cols, image->rows);
	printf("%d\n", 255);
	int i, j;
	for (i = 0; i < image->rows; i++) {
		for (j = 0; j <image->cols; j++) {
			Color* tmp = &(image->image[i][j]);
			printf("%3d %3d %3d", tmp->R, tmp->G, tmp->B);
			if (j != image->cols - 1) {  // If not the last pixel in the row, print three spaces
				printf("   ");  // Use three spaces for better formatting
			}
		}
		printf("\n");  // Print a newline after each row
	}
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	int i;
	for (i = 0; i < image->rows; i++) {
		free(image->image[i]);
	}
	free(image->image);
	free(image);
}