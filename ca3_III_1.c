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
	float** image_gc;
	float** image_orig;
	float** image_gr;
	float** image_g;
	float** image_edge;
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

	int row;
	int column;
	
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
	image_orig = (float **) malloc(image_size * sizeof(float *));
	image_complex = (Complex **) malloc(image_size * sizeof(Complex *));
	image_gc = (float **) malloc(image_size * sizeof(float *));
	image_gr = (float **) malloc(image_size * sizeof(float *));
	image_g = (float **) malloc(image_size * sizeof(float *));
	image_edge = (float **) malloc(image_size * sizeof(float *));

	for (i = 0; i < image_size; i++) 
	{
		image_byte[i] = (unsigned char *) malloc(image_size * sizeof(unsigned char));
		image_orig[i] = (float *) malloc(image_size * sizeof(float));
		image_complex[i] = (Complex *) malloc(image_size * sizeof(Complex));
		image_gc[i] = (float *) malloc(image_size * sizeof(float));
		image_gr[i] = (float *) malloc(image_size * sizeof(float));
		image_g[i] = (float *) malloc(image_size * sizeof(float));
		image_edge[i] = (float *) malloc(image_size * sizeof(float));
	}

	/* Read in the image and convert to a float */
	read_image(hdr, image_byte, input_file, image_size);
	//byte2float(image_byte, image_float, image_size);
	byte2float(image_byte, image_orig, image_size);

/**************************gc(m,n)*************************************/
	for(row = 1; row < image_size-1; row++)
	{
		for(column = 1; column < image_size-1; column++) 
		{
			image_gc[row][column] = 0.25*image_orig[row-1][column+1]+0.5*image_orig[row-1][column]+0.25*image_orig[row-1][column-1]-0.25*image_orig[row+1][column+1]-0.5*image_orig[row+1][column]-0.25*image_orig[row+1][column-1];
        	}
	}
/***************************gr(m,n)*************************************/
	for(row = 1; row < image_size-1; row++)
	{
		for(column = 1; column < image_size-1; column++) 
		{
			image_gr[row][column] = 0.25*image_orig[row-1][column-1]+0.5*image_orig[row][column-1]+0.25*image_orig[row+1][column-1]-0.25*image_orig[row+1][column+1]-0.5*image_orig[row][column+1]-0.25*image_orig[row-1][column+1];
        	}
	}
/****************************g(m,n)***************************************/
	for(row = 1; row < image_size-1; row++)
	{
		for(column = 1; column < image_size-1; column++) 
		{
			image_g[row][column] = sqrt(image_gc[row][column]*image_gc[row][column]+image_gr[row][column]*image_gr[row][column]);
        	}
	}
/*****************************edge point*********************************************/
	float thresh=60;
	int edgepts=0;
	for(row = 1; row < image_size-1; row++)
	{
		for(column = 1; column < image_size-1; column++) 
		{
			if((image_g[row][column]>=thresh)&&(abs(image_gr[row][column])>=abs(image_gr[row][column-1]))&&(abs(image_gr[row][column])>=abs(image_gr[row][column+1]))&&(abs(image_gr[row][column])>=2*abs(image_gc[row][column]))){
				image_edge[row][column]=255;
				edgepts++;
			}
			else if((image_g[row][column]>=thresh)&&(abs(image_gc[row][column])>=abs(image_gc[row+1][column]))&&(abs(image_gc[row][column])>=abs(image_gc[row-1][column]))&&(abs(image_gc[row][column])>=2*abs(image_gr[row][column]))){
				image_edge[row][column]=255;
				edgepts++;
			}
			else{
				image_edge[row][column]=0;		
			}

       	}
	}
	printf("the number of edge points: %d\n", edgepts);
/*****************************************************************************/
	float2byte(image_edge, image_byte, 1.0, image_size);

	
	/* Output to a file */
	generate_header(hdr, image_size, image_size);
	write_image(hdr, image_byte, output_file, image_size);
}
