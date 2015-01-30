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
	float** image_8x8;
	float** var_8x8;
	float** image_ori;
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
	int n;

	int T=8;
	
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
	image_8x8 = (float **) malloc(8 * sizeof(float *));
	var_8x8 = (float **) malloc(8 * sizeof(float *));
	image_ori = (float **) malloc(image_size * sizeof(float *));
	
	for (i = 0; i < image_size; i++) 
	{
		image_byte[i] = (unsigned char *) malloc(image_size * sizeof(unsigned char));
		image_float[i] = (float *) malloc(image_size * sizeof(float));
		image_ori[i] = (float *) malloc(image_size * sizeof(float));
		image_complex[i] = (Complex *) malloc(image_size * sizeof(Complex));
	}

	for (i = 0; i < 8; i++) 
	{
		image_8x8[i] = (float *) malloc(8 * sizeof(float));
		var_8x8[i] = (float *) malloc(8 * sizeof(float));
	}

	/* Read in the image and convert to a float */
	read_image(hdr, image_byte, input_file, image_size);
	byte2float(image_byte, image_float, image_size);
	byte2float(image_byte, image_ori, image_size);	




/*******************dct saved to image_float***************************************/
	
	
	for(m=0;m<image_size;m=m+8){
		for(n=0;n<image_size;n=n+8){
			for(i=m;i<(m+8);i++){
				for(j=n;j<(n+8);j++){
					image_8x8[i-m][j-n]=image_float[i][j];
				}
			}
			if( DCT2D(image_8x8, 8, 1) == 0)
			{
				fprintf(stderr, "Image size is not a power of 2! Exiting.\n");
				exit(1);
			}
			
			
			
			for(i=m;i<(m+8);i++){
				for(j=n;j<(n+8);j++){
					image_float[i][j]=image_8x8[i-m][j-n];
				}
			}
		}	
	}
	
/*************************************************************/

/***********************var*****************************/
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			float sum=0;
			float mean;
			float var_sum=0;
			for(m=i;m<image_size;m=m+8){
				for(n=j;n<image_size;n=n+8){
					sum+=image_float[m][n];
				}
			}
			mean=sum/1024;
			for(m=i;m<image_size;m=m+8){
				for(n=j;n<image_size;n=n+8){
					var_sum+=(image_float[m][n]-mean)*(image_float[m][n]-mean);
				}
			}
			var_8x8[i][j]=var_sum/1024;
		}
	}
/**********************************************************/

	//float nMse=nmse(image_float,image_ori, image_size);
	
	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			printf("%f\t",var_8x8[i][j]);
		}
		printf("\n");
	}

	
/********************squared magnitude****************************
	float new_max_value1=0.0;	
	for(k=0;k<image_size;k++){
		for(m=0;m<image_size;m++){
			image_float[k][m] = image_float[k][m]*image_float[k][m];
			if(image_float[k][m]>new_max_value1) {new_max_value1=image_float[k][m];}
		}
	}

/*********************II_3****************************
	scale_array(image_float, 1/new_max_value1, image_size);
	


/********************dct max value****************************/
	float new_max_value3=0.0;	
	for(k=0;k<image_size;k++){
		for(m=0;m<image_size;m++){
			if(image_float[k][m]>new_max_value3) {new_max_value3=image_float[k][m];}
		}
	}
/*****************************************************************/

	/* Shift the DC component of the transform to the center of the spectrum */
//	shift_2d_float(image_float, image_size);

	/* We must scale by 255.0/max_val to get the correct output */
	/* For visibility  purposes, we scale by less */
//	float2byte(image_float, image_byte, output_scale/max_value, image_size);
//	float2byte(image_float, image_byte, output_scale/new_max_value1, image_size);
//	float2byte(image_float, image_byte, output_scale/new_max_value2, image_size);
	float2byte(image_float, image_byte, output_scale/new_max_value3, image_size);
/**********************************************************/

	//printf("the nmse between orig and T=1 is: %f\n", nMse);	


/****************generate imput image**********************
	float2byte(image_float, image_byte, 1, image_size);
/**********************************************************/

	/* Output to a file */
	generate_header(hdr, image_size, image_size);
	write_image(hdr, image_byte, output_file, image_size);
}
