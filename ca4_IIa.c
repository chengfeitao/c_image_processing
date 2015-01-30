

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
	
	
	float A=0;
	float B=0;
	float C=0;
	float D=0;
	float E=0;
	float F=0;

	int size = 10000;

	for(i=0;i<size;i++){
		float r = uniformly_dist_random_number();
		if(r>=0&&r<0.1){
			A++;	
		}
		if(r>=0.1&&r<0.5){
			B++;	
		}
		if(r>=0.5&&r<0.56){
			C++;	
		}
		if(r>=0.56&&r<0.66){
			D++;	
		}
		if(r>=0.66&&r<0.7){
			E++;	
		}
		if(r>=0.7&&r<1){
			F++;	
		}
	}

	printf("A: %f\n", A);
	printf("B: %f\n", B);
	printf("C: %f\n", C);
	printf("D: %f\n", D);
	printf("E: %f\n", E);
	printf("F: %f\n", F);

	float codeLength = A/size*3+B/size*1+C/size*5+D/size*4+E/size*5+F/size*2;

	printf("codeLength: %f\n", codeLength);

}
