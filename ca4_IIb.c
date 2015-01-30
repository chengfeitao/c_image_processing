

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
	float m;
	float n=0;

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


	int symbolsize = 30;
	int ranstreamsize=symbolsize*4;
	unsigned char* ranstream;
	unsigned char* symbol;
	unsigned char* decodedsymbol;
	ranstream = (unsigned char *) malloc(ranstreamsize * sizeof(unsigned char));
	symbol = (unsigned char *) malloc(2*symbolsize * sizeof(unsigned char));
	decodedsymbol = (unsigned char *) malloc(2*symbolsize * sizeof(unsigned char));
	
	float A=0;
	float B=0;
	float C=0;
	float D=0;
	float E=0;
	float F=0;

	int flip;



/*********encode stream******************/
	for(i=0;i<symbolsize;i++){
		float r = uniformly_dist_random_number();
		if(r>=0&&r<0.1){
			symbol[i]='a';
			ranstream[k]='0';
			ranstream[k+1]='1';
			ranstream[k+2]='1';
			k=k+3;	
		}
		if(r>=0.1&&r<0.5){
			symbol[i]='b';
			ranstream[k]='1';
			k=k+1;	
		}
		if(r>=0.5&&r<0.56){
			symbol[i]='c';	
			ranstream[k]='0';
			ranstream[k+1]='1';
			ranstream[k+2]='0';
			ranstream[k+3]='1';
			ranstream[k+4]='0';
			k=k+5;
		}
		if(r>=0.56&&r<0.66){
			symbol[i]='d';
			ranstream[k]='0';
			ranstream[k+1]='1';
			ranstream[k+2]='0';
			ranstream[k+3]='0';
			k=k+4;
		}
		if(r>=0.66&&r<0.7){
			symbol[i]='e';
			ranstream[k]='0';
			ranstream[k+1]='1';
			ranstream[k+2]='0';
			ranstream[k+3]='1';
			ranstream[k+4]='1';
			k=k+5;
		}
		if(r>=0.7&&r<1){
			symbol[i]='f';
			ranstream[k]='0';
			ranstream[k+1]='0';
			k=k+2;
		}
	}
/*******************************************/

	printf("stream size: %d\n",k);

for(flip=20;flip<k-20;flip++){

	if(ranstream[flip]=='0'){
		ranstream[flip]='1';
	}
	else{
		ranstream[flip]='0';	
	}

/**********decode symbol****************/
	i=0;
	for(j=0;i<k;j++){
		if(ranstream[i]=='1'){
			decodedsymbol[j]='b';
			i=i+1;
		}
		else if(ranstream[i]=='0'&&ranstream[i+1]=='0'){
			decodedsymbol[j]='f';
			i=i+2;
		}
		else if(ranstream[i]=='0'&&ranstream[i+1]=='1'&&ranstream[i+2]=='1'){
			decodedsymbol[j]='a';
			i=i+3;
		}
		else if(ranstream[i]=='0'&&ranstream[i+1]=='1'&&ranstream[i+2]=='0'&&ranstream[i+3]=='0'){
			decodedsymbol[j]='d';
			i=i+4;	
		}
		else if(ranstream[i]=='0'&&ranstream[i+1]=='1'&&ranstream[i+2]=='0'&&ranstream[i+3]=='1'&&ranstream[i+4]=='0'){
			decodedsymbol[j]='c';
			i=i+5;
		}
		else if(ranstream[i]=='0'&&ranstream[i+1]=='1'&&ranstream[i+2]=='0'&&ranstream[i+3]=='1'&&ranstream[i+4]=='1'){
			decodedsymbol[j]='e';
			i=i+5;	
		}
		else{i++;}
	}
/***************************************/

	

/************resync time*********************/
	int error=0;
	int d=0;
	int resync=1;
	int flag=0;
	for(i=0;d<(symbolsize-5);){
		if(flag==1){break;}
		if(symbol[i]!=decodedsymbol[d]){
			int start_d=d;
			d++;
			i++;
			int a;
			int b;
			
			for(b=i;b<i+10;b++){
				if(flag==1){break;}
				for(a=d;a<d+10;){
					if(decodedsymbol[a]==symbol[i]&&decodedsymbol[a+1]==symbol[i+1]&&decodedsymbol[a+2]==symbol[i+2]){
						error=a-start_d;
						flag=1;
						break;
					}
					else{
						a++;
					}
				}
				
			}
			//printf("error\n");
			resync=0;
		}
		
		else{
			i++;
			d++;
			//printf("correct\n");
		}
	}
	//printf("resync time: %d\n",error);
	n+=error;
/**************************************/

}

m=n/k;

printf("symbol size: %d\n",symbolsize);

printf("average resync time: %f\n",m);
	

/*
	printf("A: %f\n", A);
	printf("B: %f\n", B);
	printf("C: %f\n", C);
	printf("D: %f\n", D);
	printf("E: %f\n", E);
	printf("F: %f\n", F);
*/
	//float codeLength = A/symbolsize*3+B/symbolsize*1+C/symbolsize*5+D/symbolsize*4+E/symbolsize*5+F/symbolsize*2;

	//printf("codeLength: %f\n", codeLength);

}
