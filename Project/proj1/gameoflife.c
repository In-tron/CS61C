/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//This function takes a Color pointer and returns a 32-bit RGB value.
uint32_t getRGB(Color *color) {
	uint32_t R = color->R;
	uint32_t G = color->G;
	uint32_t B = color->B;
	uint32_t RGB = R+ (G<< 8) + (B << 16);
	return RGB;
}

//This function takes a 32-bit RGB value and returns an array of three uint8_t values, one for each color channel (R, G, B).
uint8_t *getRnGnB(uint32_t RGB) {
	uint8_t *array = (uint8_t*) malloc(sizeof(uint8_t) * 3);
	int i;
	for (i = 0; i < 3; i++) {
		RGB = RGB >> (8 * i);
		array[i] = RGB & 255;
	}
	return array;
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule) {
    uint32_t RGB = getRGB(&image->image[row][col]);
    uint32_t nextRGB = 0;

    for (int bit = 0; bit < 24; ++bit) {
        uint8_t cell_bit = (RGB >> bit) & 1;
        uint32_t label = cell_bit << 9;

        for (int dr = -1; dr <= 1; ++dr) {
            int r = (row + dr + image->rows) % image->rows;
            for (int dc = -1; dc <= 1; ++dc) {
                int c = (col + dc + image->cols) % image->cols;
                if (dr == 0 && dc == 0) continue;

                uint32_t neighborRGB = getRGB(&image->image[r][c]);
                uint8_t neighbor_bit = (neighborRGB >> bit) & 1;
                label = (label << 1) | neighbor_bit;
            }
        }

        if (rule & (1 << label)) {
            nextRGB |= (1 << bit);
        }
    }

    uint8_t *colors = getRnGnB(nextRGB);
    Color *result = malloc(sizeof(Color));
    result->R = colors[0];
    result->G = colors[1];
    result->B = colors[2];
    free(colors);
    return result;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	Image* newImage = (Image*) malloc(sizeof(Image));
	newImage->rows = image->rows;
	newImage->cols = image->cols;
	newImage->image = (Color**) malloc(sizeof(Color*) * newImage->rows);
	int i, j;
	for (i = 0; i < newImage->rows; i++) {

		newImage->image[i] = (Color*) malloc(sizeof(Color) * newImage->cols);

		for (j = 0; j < newImage->cols; j++) {
			Color* newColor = evaluateOneCell(image, i, j, rule); // Evaluate the color of the cell at (i, j) using the given rule.
			newImage->image[i][j] = *newColor;
			free(newColor);	
		}
	}
	return newImage;
}

void processCLI(int argc, char **argv, char **filename, uint32_t *rule) 
{
	if (argc != 3 ) {
		printf("usage: %s filename\n",argv[0]);
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		printf("rule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}
	*filename = argv[1]; // The name of the file to read.
	char *endptr;
	*rule = strtol(argv[2], &endptr, 16);  // Convert the string to a long integer in base 16.
	if (*rule > 0x3ffff) { // The maximum value for a rule is 0x3ffff.
		printf("usage: %s filename\n",argv[0]);
		printf("filename is an ASCII PPM file (type P3) with maximum value 255.\n");
		printf("rule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	Image *image, *newImage;
	char* fileName;
	uint32_t rule;
	processCLI(argc,argv,&fileName, &rule); // Process the command line arguments to get the filename and rule.
	image = readData(fileName);
	newImage = life(image, rule);
	writeData(newImage);
	freeImage(image);
	freeImage(newImage);
	return 0;
}