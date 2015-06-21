#include<gdal.h>
#include<cpl_conv.h>
#include<stdio.h>
#include<iostream>
#include<stdlib.h>	
#include"load_tiff.h"

using namespace std;

int main()
{
	//int		band = 6;
	int*	dimensions;
	float**	image1;
	float**	image2;
	float**	result0;
	float**	result1;
	float**	result2;

	my_prog("RADIANCE.tif");

	dimensions = sizeOfImage("Radiance.tif");

	image1 = dynamicArray(dimensions[0], dimensions[1], 3);
	image2 = dynamicArray(dimensions[0], dimensions[1], 16);

	result1 = add( image1, image2, dimensions[0], dimensions[1]);
	result0 = subtract(image1, image2, dimensions[0], dimensions[1]);
	result2 = divide(result0, result1, dimensions[0], dimensions[1]);

	for(int i=0; i<dimensions[0] ;i++){
		for(int j=0; j<dimensions[1]; j++){
			if(result2[i][j]!=0 && result2[i][j] > 0)
				cout << result2[i][j] << endl ;
		}
	}
}