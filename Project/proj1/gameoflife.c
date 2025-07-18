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

//This function returns the previous position in a circular array.
//Note that we treat the top row as adjacent to the bottom row, and the left column as adjacent to the right column.
int prevPosition(int current, int size) {
	if (current != 0) { // If the current position is not the first element, we can just return the previous element.
		return current - 1;
	} else { // If the current position is the first element, we wrap around to the last element.
		return size - 1;
	}
}

//This function returns the next position in a circular array.
int nextPosition(int current, int size) {
	if (current != size - 1) {
		return current + 1;
	} else {
		return 0;
	}
}

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
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
	uint32_t RGB = getRGB(&image->image[row][col]);
	int time = 0;
	uint32_t sum = 0;

	uint32_t r, c; // Will be used for iterating through all neighbors.

	for (time = 0; time < 24; time++) {  // Iterate through the 24 bits of RGB.
		uint8_t last = RGB & 1; // Get the last bit. If 1, this pixel is alive.
		RGB = RGB >> 1; // Shift the RGB value to the right by 1 to get the next bit.
		uint32_t label = 1;
		label = label << (9 * last); // If the last bit is 1, we shift the label by 9 bits to the left, otherwise it stays 1.

		r = prevPosition(row, image->rows); // Get the previous row, wrapping around if necessary.
		c = prevPosition(col, image->cols); // Get the previous column, wrapping around if necessary.

		uint8_t i, j;
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				if ( r == row && c == col) { // If we are at the cell itself, we skip it.
					c = nextPosition(c, image->cols); // Skip to the next column.
					continue;
				} else {
					Color* current = &image->image[r][c]; // Get the current pixel color.
					uint32_t currentRGB = getRGB(current); // Get the RGB value of the current pixel.
					uint8_t target = (currentRGB >> time) & 1; // Get the target (last) bit for the current pixel at the current time step.

					if (target) { // If the target bit is 1, this pixel is alive.
						label = label << 1; // Shift the label to the left by 1 to indicate that this pixel is alive.
					} // If this pixel alive, add 1 to the label.

					c = nextPosition(c, image->cols); // Move to the next column.
				}				
			}
			r = nextPosition(r, image->rows); // Move to the next row.
		}
		if (label & rule) { // If the label is set in the rule, we add 1 << time to the sum.
			sum += (1 << time);
		}
	}
	uint8_t *numbers = getRnGnB(sum); // Convert the sum to an array of three uint8_t values (R, G, B).
	Color *tmp = (Color*) malloc(sizeof(Color)); // Allocate space for a new Color.
	tmp->R = numbers[0];
	tmp->G = numbers[1];
	tmp->B = numbers[2];
	free(numbers);
	return tmp;
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