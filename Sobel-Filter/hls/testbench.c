// This will apply the sobel filter and return the PSNR between the golden sobel and the produced sobel
// sobelized image
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include "sobel_hw.h"

#define SIZE	1024
#define INPUT_FILE	"input.gre"
#define OUTPUT_FILE	"output.gre"
#define GOLDEN_FILE	"golden.gre"
#define TEST_FILE	"test.gre"

unsigned char golden[SIZE*SIZE], total_output[SIZE*SIZE];

double psnr_func()
{
	//unsigned char input[SIZE*SIZE], output[SIZE*SIZE];

	double PSNR = 0, t;
	int i, j, k, l;
	unsigned int p;
	int res;
	FILE *f_in, *f_out, *f_golden, *f_test;

	printf("1");


	f_in = fopen(INPUT_FILE, "r");
	if (f_in == NULL) {
		printf("File " INPUT_FILE " not found\n");
		exit(1);
	}

	f_out = fopen(OUTPUT_FILE, "wb");
	if (f_out == NULL) {
		printf("File " OUTPUT_FILE " could not be created\n");
		fclose(f_in);
		exit(1);
	}

	f_golden = fopen(GOLDEN_FILE, "r");
	if (f_golden == NULL) {
		printf("File " GOLDEN_FILE " not found\n");
		fclose(f_in);
		fclose(f_out);
		exit(1);
	}
	f_test = fopen(TEST_FILE, "wb");
	fread(golden, sizeof(unsigned char), SIZE*SIZE, f_golden);

	for(k=0;k<(SIZE/ROWS)+3;k = k+1){
		if(k == 0){
			fread(input, sizeof(unsigned char), ROWS*SIZE, f_in);
		}
		else if(k<(SIZE/ROWS)+2){
			memmove(input, &input[(ROWS-2)*SIZE], 2*SIZE);
			fread(&input[2*SIZE], sizeof(unsigned char), (ROWS-2)*SIZE, f_in);
		}
		else if(k==(SIZE/ROWS)+2){
			memmove(input, &input[(2)*SIZE], (ROWS-2)*SIZE);
			fread(&input[(ROWS-2)*SIZE], sizeof(unsigned char), 2*SIZE, f_in);
		}

//		fread(input, sizeof(unsigned char), ROWS*SIZE, f_in);

		sobel(input,output);
		if(k == 0){
			for(l = 0; l<SIZE; l=l+1){
				output[l] = 0;
			}
			fwrite(output, sizeof(unsigned char), (ROWS-1)*SIZE, f_out);
		}
		else if(k<(SIZE/ROWS)+2){
			fwrite(&output[SIZE], sizeof(unsigned char), (ROWS-2)*SIZE, f_out);
		}
		else if(k == (SIZE/ROWS)+2){
			for(l = 0; l<SIZE; l=l+1){
				output[(ROWS-1)*SIZE + l] = 0;
			}
			fwrite(&output[(ROWS-3)*SIZE], sizeof(unsigned char), 3*SIZE, f_out);
		}
		for(l=0;l<SIZE/ROWS;l = l+1){
			total_output[k*SIZE + l] = output[l];
		}
	}
//	memset(total_output, 0, SIZE*sizeof(unsigned char));
//	memset(&total_output[SIZE*(SIZE-1)], 0, SIZE*sizeof(unsigned char));
	fclose(f_in);
	fclose(f_golden);
	fclose(f_out);
	f_out = fopen(OUTPUT_FILE, "r");
	fread(total_output, sizeof(unsigned char), SIZE*SIZE, f_out);

	for (i=1; i<SIZE-1; i++) {
		for (j=1; j<SIZE-1; j++) {
			t = pow((total_output[i*SIZE+j] - golden[i*SIZE+j]),2);
			PSNR += t;
		}
	}

	PSNR /= (double)(SIZE*SIZE);
	PSNR = 10*log10(65536/PSNR);

	/* Write the output file */
	//fwrite(total_output, sizeof(unsigned char), SIZE*SIZE, f_out);
	fclose(f_out);

	return PSNR;
}


int main(int argc, char* argv[])
{
	double PSNR;

	PSNR = psnr_func();
	printf("PSNR of original Sobel and computed Sobel image: %g\n", PSNR);
	printf("A visualization of the sobel filter can be found at " OUTPUT_FILE ", or you can run 'make image' to get the jpg\n");

	return 0;
}

