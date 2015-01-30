/* *********************************************************************************
   EE211A Image Processing Test Program (10 October 1999)
 
   Filename:
   fft_example.c

   Description:
   This program creates a 2D rect and writes it out to a file called rect.gray.
   It is run using the command:

       fft_example output_image_name.gray rect_size image_size output_scale
       
   where filename extension .gray must be included and the image_size must be a
   power of 2.
   
   ****************************************************************************** */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image_lib.h"

void main(int argc, char* argv[])
{
	unsigned char** image_byte;
	float** image_float;
	float** image_temp;
	float** image_median_temp;
	Complex** image_complex;
	unsigned char hdr[HEADER_LEN];
	char input_file[MAX_NAME];
	char output_file[MAX_NAME];
	int rect_size;
	int image_size;
	int output_scale;	
	
	float max_value;
	float avg_value;	
	int i;
	int j;

	int k;	
	int m;
	
    /* Absorb command line arguments */
    if ((argc > 6) || (argc < 6))
    {
		fprintf(stderr,
                "Usage: %s [output image name] [rect size] [image size] [output scale]", 
                argv[0]);
        exit(1);
    }
    
    strcpy(input_file, argv[1]);
    strcpy(output_file, argv[2]);
    rect_size = atoi(argv[3]);
    image_size = atoi(argv[4]);
    output_scale = atoi(argv[5]);    
	
	/* Dynamically allocate the 2D arrays */
	image_byte = (unsigned char **) malloc(image_size * sizeof(unsigned char *));
	image_float = (float **) malloc(image_size * sizeof(float *));
	image_complex = (Complex **) malloc(image_size * sizeof(Complex *));
	image_temp = (float **) malloc(image_size * sizeof(float *));
	image_median_temp = (float **) malloc(image_size * sizeof(float *));

	for (i = 0; i < image_size; i++) 
	{
		image_byte[i] = (unsigned char *) malloc(image_size * sizeof(unsigned char));
		image_float[i] = (float *) malloc(image_size * sizeof(float));
		image_complex[i] = (Complex *) malloc(image_size * sizeof(Complex));
		image_temp[i] = (float *) malloc(image_size * sizeof(float));
		image_median_temp[i] = (float *) malloc(image_size * sizeof(float));
	}

	/* Read in the image and convert to a float */
	read_image(hdr, image_byte, input_file, image_size);
	byte2float(image_byte, image_float, image_size);
	byte2float(image_byte, image_temp, image_size);
/***********************ca3_IV_1*****************/
	
	add_impulse_noise(image_float, 30.0, image_size);
	//float nMse=nmse(image_float,image_temp, image_size);
	//printf("the nmse is: %f\n", nMse);

/***********median filter*******************************************/
	median_filter(image_float, image_median_temp, 3, image_size);
	
/**********************************************************/


	
	float nMse=nmse(image_float,image_temp, image_size);
	printf("the nmse between orig and noise added is: %f\n", nMse);

	float nMse2=nmse(image_median_temp,image_temp, image_size);
	printf("the nmse between orig and median filtered is: %f\n", nMse2);
	
	float nMse3=nmse(image_float,image_median_temp, image_size);
	printf("the nmse between noise added and median filtered is: %f\n", nMse3);

	float2byte(image_median_temp, image_byte, 1.0, image_size);

	
	/* Output to a file */
	generate_header(hdr, image_size, image_size);
	write_image(hdr, image_byte, output_file, image_size);
}
