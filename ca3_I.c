/* *********************************************************************************
   EE211A Image Processing Introduction Program (23 September 1999)
 
   Filename:
   intro.c

   Description:
   This program demonstrates some of the fundamental EE211A routines. It halves 
   the image pixel values of an image. It is run using the command:

       ca1 input_image.gray output_image.gray image_size

   where filename extension .gray must be included and the image_size must be a
   power of 2.

   ****************************************************************************** */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image_lib.h"


/* 
 * The size of the image array is assumed to be n by n, where the value of n 
 * is absorbed from the command line. Most of the images we will use are 256 by 256.
 */
void main(int argc, char* argv[])
{
	unsigned char** image_byte;
	float** image_float;
	float** image_float_temp;
	unsigned char hdr[HEADER_LEN];
	char input_file[MAX_NAME];
	char output_file[MAX_NAME];
	int image_size;
	
	float mean;
	float std_dev;
	int i;
	int row;
	int column;

	int* hist;
	int h;


    /* Absorb command line arguments */
    if ((argc > 4) || (argc < 4))
    {
		fprintf(stderr, "Usage: %s [input image] [output image] [image size]\n", argv[0]);
        exit(1);
    }
    
    strcpy(input_file, argv[1]);
    strcpy(output_file, argv[2]);
    image_size = atoi(argv[3]);
	
	/* Dynamically allocate the 2D arrays */
	image_byte = (unsigned char **) malloc(image_size * sizeof(unsigned char *));
	image_float = (float **) malloc(image_size * sizeof(float *));
	image_float_temp = (float **) malloc(image_size * sizeof(float *));
	hist = (int*) malloc(256*sizeof(int));
	
	for (i = 0; i < image_size; i++) 
	{
		image_byte[i] = (unsigned char *) malloc(image_size * sizeof(unsigned char));
		image_float[i] = (float *) malloc(image_size * sizeof(float));
		image_float_temp[i] = (float *) malloc(image_size * sizeof(float));
	}

    /* Read in the image and convert to a float */
	read_image(hdr, image_byte, input_file, image_size);
	byte2float(image_byte, image_float, image_size);

    /* Get and print the mean and standard deviation of the image */
	image_statistics(image_float, &mean, &std_dev, image_size);
	printf("Before Processing: Signal Mean = %f\n", mean); 
	printf("                   Standard Deviation = %f\n", std_dev);


	image_statistics(image_float, &mean, &std_dev, image_size);
	printf("After Processing: Signal Mean = %f\n", mean); 
	printf("                  Standard Deviation = %f\n", std_dev);

/***********ca3_I*******************************************/
	histogram(image_byte, hist, image_size);
	for(h=0;h<256;h++){
		printf("%d\n", hist[h]);
	}
	
/***********************************************************/

    /* Convert the float image to a byte image */
	/* We must scale by 1.0 to get the correct output */
	float2byte(image_float, image_byte, 1.0, image_size);

/*************************find non zero pixels*********************************/
	int k,m;
	int count=0;	
	for(k=0;k<image_size;k++){
		for(m=0;m<image_size;m++){
			if(image_byte[k][m]>1) {count++;}
		}
	}
	printf("non zero pixels: %d \n", count);
/***********************************************************/

	/* Output to a file */
	generate_header(hdr, image_size, image_size);
	write_image(hdr, image_byte, output_file, image_size);	
}
