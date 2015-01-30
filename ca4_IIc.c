

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

	int k=0;	
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


	int symbolsize = 10000;
	int ranstreamsize=symbolsize*4;
	unsigned char* ranstream;
	ranstream = (unsigned char *) malloc(ranstreamsize * sizeof(unsigned char));
	int index=0;
	
	float A=0;
	float B=0;
	float C=0;
	float D=0;
	float E=0;
	float F=0;

/*********encode ranstream******************/
	for(i=0;i<ranstreamsize;i++){
		float r = uniformly_dist_random_number();
		if(r<0.5){
			ranstream[i]='0';
		}
		if(r>=0.5){
			ranstream[i]='1';
		}
	}
/*******************************************/

/**********decode symbol****************/
	for(i=0;i<ranstreamsize;){
		if(ranstream[i]=='1'){
			B++;
			i=i+1;
		}
		else if(ranstream[i]=='0'&&ranstream[i+1]=='0'){
			F++;
			i=i+2;
		}
		else if(ranstream[i]=='0'&&ranstream[i+1]=='1'&&ranstream[i+2]=='1'){
			A++;
			i=i+3;
		}
		else if(ranstream[i]=='0'&&ranstream[i+1]=='1'&&ranstream[i+2]=='0'&&ranstream[i+3]=='0'){
			D++;
			i=i+4;	
		}
		else if(ranstream[i]=='0'&&ranstream[i+1]=='1'&&ranstream[i+2]=='0'&&ranstream[i+3]=='1'&&ranstream[i+4]=='0'){
			C++;
			i=i+5;
		}
		else if(ranstream[i]=='0'&&ranstream[i+1]=='1'&&ranstream[i+2]=='0'&&ranstream[i+3]=='1'&&ranstream[i+4]=='1'){
			E++;
			i=i+5;	
		}
		else{i++;}
	}
/***************************************/
	
/*
	for(i=0;i<symbolsize;i++){
		float r = uniformly_dist_random_number();
		if(r>=0&&r<0.1){
			A++;
			ranstream[k]='0';
			ranstream[k+1]='1';
			ranstream[k+2]='1';
			k=k+3;	
		}
		if(r>=0.1&&r<0.5){
			B++;
			ranstream[k]='1';
			k=k+1;	
		}
		if(r>=0.5&&r<0.56){
			C++;	
			ranstream[k]='0';
			ranstream[k+1]='1';
			ranstream[k+2]='0';
			ranstream[k+3]='1';
			ranstream[k+4]='0';
			k=k+5;
		}
		if(r>=0.56&&r<0.66){
			D++;	
			ranstream[k]='0';
			ranstream[k+1]='1';
			ranstream[k+2]='0';
			ranstream[k+3]='0';
			k=k+4;
		}
		if(r>=0.66&&r<0.7){
			E++;	
			ranstream[k]='0';
			ranstream[k+1]='1';
			ranstream[k+2]='0';
			ranstream[k+3]='1';
			ranstream[k+4]='1';
			k=k+5;
		}
		if(r>=0.7&&r<1){
			F++;	
			ranstream[k]='0';
			ranstream[k+1]='0';
			k=k+2;
		}
	}
*/

	printf("A: %f\n", A);
	printf("B: %f\n", B);
	printf("C: %f\n", C);
	printf("D: %f\n", D);
	printf("E: %f\n", E);
	printf("F: %f\n", F);

	float codeLength = A/symbolsize*3+B/symbolsize*1+C/symbolsize*5+D/symbolsize*4+E/symbolsize*5+F/symbolsize*2;

	//printf("codeLength: %f\n", codeLength);

}
