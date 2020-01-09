#include <stdio.h>
#include <math.h>
#include "sobel_hw.h"

//char horiz_operator[3][3] = {{-1, 0, 1},
//                             {-2, 0, 2},
//                             {-1, 0, 1}};
//char vert_operator[3][3] = {{1, 2, 1},
//                            {0, 0, 0},
//                            {-1, -2, -1}};


int convolution2D_H(int posy, int posx, const unsigned char *input) {
	int size, res;

	res = 0;
	size = posy*SIZE + posx;
	res = res -input[size-SIZE - 1] + input[size-SIZE + 1] -(input[size- 1]<<1) + (input[size+ 1]<<1) -input[size+SIZE- 1]+input[size+SIZE+ 1];
	return(res);
}

int convolution2D_V(int posy, int posx, const unsigned char *input) {
	int i,j,size, res;

	res = 0;
	size = posy*SIZE + posx;
	res = res + input[size-SIZE - 1] +(input[size-SIZE]<<1) + input[size-SIZE + 1] - input[size+SIZE - 1] - (input[size+SIZE]<<1) - input[size+SIZE + 1];
	return(res);
}

void sobel(din_t input[ARRAY_SIZE], dout_t output[ARRAY_SIZE])
{
	int i, j,k;
	unsigned int p;
	int res;
//	unsigned char prefetch[3*SIZE];

	sobel_0_loop:for (i = 1; i < ROWS-1; i++) {
		output[i*SIZE] = 0;
		output[i*SIZE + SIZE - 1] = 0;
	}
//	memset(prefetch,0,3*SIZE);

	/* This is the main computation. Get the starting time. */
	/* For each pixel of the output image */
	sobel_outerr_loop:for (i=1; i<ROWS-1; i+=1) {
//		for(i = 0; i<SIZE; i=i+1){
//			prefetch[i]=input[j*(SIZE-1) + i];
//			prefetch[SIZE+i]=input[j*SIZE + i];
//			prefetch[2*SIZE+i]=input[j*(SIZE+1)+i];
//		}
		sobel_inner_loop:for (j=1; j<SIZE-1; j+=1 ) {

			/* Apply the sobel filter and calculate the magnitude *
			 * of the derivative.								  */
//			p = (convolution2D_H(i, j, input))*(convolution2D_H(i, j, input)) +
//				(convolution2D_V(i, j, input))*(convolution2D_V(i, j, input));
//			res = (int) sqrt((double) p);
			res = abs(convolution2D_H(i, j, input)) + abs(convolution2D_V(i, j, input));
			if (res > 255){
				output[i*SIZE + j] = 255;
//				one++;
			}
			else{
				output[i*SIZE + j] = (unsigned char)res;
//				two++;
			}
		}
	}

}
