#ifndef MY_SOBEL_HW
#define MY_SOBEL_HW

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

typedef unsigned char din_t;
typedef unsigned char dout_t;

#define SIZE	1024
#define ROWS 32
#define ARRAY_SIZE ROWS*SIZE
unsigned char input[ARRAY_SIZE], output[ARRAY_SIZE];


void sobel(din_t input[ARRAY_SIZE], dout_t output[ARRAY_SIZE]);
int convolution2D_H(int posy, int posx, const unsigned char *input);
int convolution2D_V(int posy, int posx, const unsigned char *input);
#endif
