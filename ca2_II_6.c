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
	
	for (i = 0; i < image_size; i++) 
	{
		image_byte[i] = (unsigned char *) malloc(image_size * sizeof(unsigned char));
		image_float[i] = (float *) malloc(image_size * sizeof(float));
		image_complex[i] = (Complex *) malloc(image_size * sizeof(Complex));
	}

	/* Read in the image and convert to a float */
	read_image(hdr, image_byte, input_file, image_size);
	byte2float(image_byte, image_float, image_size);
	
	/* Initialize float array */
	//initialize_float_array(image_float, 0.0, image_size);

	/* Define rect function for i, j values between 128 - rect_size/2 and 128 + rect_size/2 */
/*	for (i = image_size/2 - rect_size/2; i <= image_size/2 + rect_size/2; i++)
	{
		for (j = image_size/2 - rect_size/2; j <= image_size/2 + rect_size/2; j++)
		{
			image_float[i][j] = 255.0;
		}
	}
*/

/**********************5ai***************************
	image_float[image_size/2][image_size/2+8] = 255.0;
	image_float[image_size/2][image_size/2-8] = 255.0;
	
/***************************************************/

/**********************5bi**************************
	for(i=0;i<image_size;i++){
		image_float[i][0]=255;
	}
/***************************************************/

/***********************5ci****************************
	for(i=0;i<image_size;i+=16){
		for(j=0;j<image_size;j+=16){
			image_float[i][j]=255;
		}
	}
/***************************************************/


/*****************generate output image********************/
	/* Perform a 2D circular shift */
	shift_2d_float(image_float, image_size);
	
	/* Convert to float to complex */
    float2complex(image_float, image_complex, image_size);

	/* Take the 2D FFT of the complex image */
	if( FFT2D(image_complex, image_size, 1) == 0)
	{
		fprintf(stderr, "Image size is not a power of 2! Exiting.\n");
		exit(1);
	}


	
	/* Take the magnitude of the array */
	array_abs_val(image_complex, image_float, &max_value, &avg_value, image_size);
/********************squared magnitude****************************/
	float new_max_value1=0.0;	
	for(k=0;k<image_size;k++){
		for(m=0;m<image_size;m++){
			image_float[k][m] = image_float[k][m]*image_float[k][m];
			if(image_float[k][m]>new_max_value1) {new_max_value1=image_float[k][m];}
		}
	}

/*********************II_3****************************/
	scale_array(image_float, 1/new_max_value1, image_size);
	
/*********************II_4****************************/
	
	float new_max_value2=0.0;	
	float c1=9.0;
	for(k=0;k<image_size;k++){
		for(m=0;m<image_size;m++){
			image_float[k][m] = (255.0 / c1) * (c1 + log10(image_float[k][m]));
			if(image_float[k][m]>new_max_value2) {new_max_value2=image_float[k][m];}
		}
	}
	
/****************************************************/

	/* Shift the DC component of the transform to the center of the spectrum */
	shift_2d_float(image_float, image_size);

	/* We must scale by 255.0/max_val to get the correct output */
	/* For visibility  purposes, we scale by less */
//	float2byte(image_float, image_byte, output_scale/max_value, image_size);
	float2byte(image_float, image_byte, output_scale/new_max_value2, image_size);
/**********************************************************/


/****************generate imput image**********************
	float2byte(image_float, image_byte, 1, image_size);
/**********************************************************/

	/* Output to a file */
	generate_header(hdr, image_size, image_size);
	write_image(hdr, image_byte, output_file, image_size);
}
