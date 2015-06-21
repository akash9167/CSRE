#ifndef LOAD_IMAGE_TIF
#define LOAD_IMAGE_TIF

#include<iostream>
#include<gdal.h>
#include<cpl_conv.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

using namespace std;

GDALDatasetH  hDataset;
	
void my_prog(char* s){
	
	GDALDriverH   hDriver;
	GDALRasterBandH hBand;    
    double	adfGeoTransform[6];

    GDALAllRegister();

	//Reading tif file
    hDataset = GDALOpen( s, GA_ReadOnly );
    if( hDataset == NULL )
    {
        printf("\nINVALID FILE FORMAT");

    }
   else
   {
		hDriver = GDALGetDatasetDriver( hDataset );
		printf( "Driver: %s/%s\n",
		        GDALGetDriverShortName( hDriver ),
		        GDALGetDriverLongName( hDriver ) );

		printf( "Size is %dx%dx%d\n\n",
		        GDALGetRasterXSize( hDataset ), 
		        GDALGetRasterYSize( hDataset ),
		        GDALGetRasterCount( hDataset ) );

		if( GDALGetProjectionRef( hDataset ) != NULL )
			printf( "Projection is `%s'\n\n", GDALGetProjectionRef( hDataset ) );

		if( GDALGetGeoTransform( hDataset, adfGeoTransform ) == CE_None )
		{
			printf( "Origin = (%.6f,%.6f)\n\n",
				    adfGeoTransform[0], adfGeoTransform[3] );

	        printf( "Pixel Size = (%.6f,%.6f)\n\n\n",
		            adfGeoTransform[1], adfGeoTransform[5] );
		}
	}
}


int* sizeOfImage(char* s){
	int* size;
	size = new int[2];

	size[0]=GDALGetRasterXSize( hDataset );
    size[1]=GDALGetRasterYSize( hDataset );
	
	return size;
	}


float** dynamicArray(int sizeX, int sizeY, int band){

	float**			image=0;
	int				nXSize, nYSize;
	float*			pafScanline;
	int				nBlockXSize, nBlockYSize;
    int				bGotMin, bGotMax;
    double			adfMinMax[2];
	GDALRasterBandH hBand;
	
	hBand = GDALGetRasterBand( hDataset, band);

	nXSize = GDALGetRasterBandXSize( hBand );
	nYSize = GDALGetRasterBandYSize( hBand );

	pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize*nYSize);
    GDALRasterIO( hBand, GF_Read, 0, 0, nXSize, nYSize, 
                  pafScanline, nXSize, nYSize, GDT_Float32, 
                  0, 0 );
	
	//Band Information
	GDALGetBlockSize( hBand, &nBlockXSize, &nBlockYSize );

        printf( "Block=%dx%d Type=%s, ColorInterp=%s\n",
                nBlockXSize, nBlockYSize,
                GDALGetDataTypeName(GDALGetRasterDataType(hBand)),
                GDALGetColorInterpretationName(GDALGetRasterColorInterpretation(hBand)) );

        adfMinMax[0] = GDALGetRasterMinimum( hBand, &bGotMin );
        adfMinMax[1] = GDALGetRasterMaximum( hBand, &bGotMax );
        if( ! (bGotMin && bGotMax) )
            GDALComputeRasterMinMax( hBand, TRUE, adfMinMax );

        printf( "Min=%.3fd, Max=%.3f\n\n", adfMinMax[0], adfMinMax[1] );
        
        /*if( GDALGetOverviewCount(hBand) > 0 )
            printf( "Band has %d overviews.\n\n", GDALGetOverviewCount(hBand));

        if( GDALGetRasterColorTable( hBand ) != NULL )
            printf( "Band has a color table with %d entries.\n", 
                     GDALGetColorEntryCount(
                         GDALGetRasterColorTable( hBand ) ) );*/

	//allocating pointer to each row
	image = new float*[sizeX];

	//allocating pointer to each column
	for(int i=0; i<sizeX; i++){

		image[i] = new float [sizeY];

	}

	//populating array
	int xCordinate,yCordinate;

	for( yCordinate = 0; yCordinate < sizeY; yCordinate++){

		for( xCordinate = 0; xCordinate < sizeX; xCordinate++){

			image[xCordinate][yCordinate]=*(pafScanline+xCordinate*yCordinate);

		}
	}
	return image;
}

float** add(float** image1, float** image2, int sizeX, int sizeY){
	float **result;

	//allocating pointer to each row
	result = new float*[sizeX];

	//allocating pointer to each column
	for(int i=0; i<sizeX; i++){

		result[i] = new float [sizeY];

	}

	for(int i=0; i<sizeX ;i++){
		for(int j=0; j<sizeY; j++){
			result[i][j]=image1[i][j]+image2[i][j];
		}
	}

	return result;
}

float** multiply(float** image1, float** image2, int sizeX, int sizeY){
	float **result;

	//allocating pointer to each row
	result = new float*[sizeX];

	//allocating pointer to each column
	for(int i=0; i<sizeX; i++){

		result[i] = new float [sizeY];

	}

	for(int i=0; i<sizeX ;i++){
		for(int j=0; j<sizeY; j++){
			result[i][j]=image1[i][j]*image2[i][j];
		}
	}

	return result;
}

float** subtract(float** image1, float** image2, int sizeX, int sizeY){
	float **result;

	//allocating pointer to each row
	result = new float*[sizeX];

	//allocating pointer to each column
	for(int i=0; i<sizeX; i++){

		result[i] = new float [sizeY];

	}

	for(int i=0; i<sizeX ;i++){
		for(int j=0; j<sizeY; j++){
			result[i][j]=image1[i][j]-image2[i][j];
		}
	}

	return result;
}

float** divide(float** image1, float** image2, int sizeX, int sizeY){
	float **result;

	//allocating pointer to each row
	result = new float*[sizeX];

	//allocating pointer to each column
	for(int i=0; i<sizeX; i++){

		result[i] = new float [sizeY];

	}

	for(int i=0; i<sizeX ;i++){
		for(int j=0; j<sizeY; j++){
			if(image2[i][j]!=0)
				result[i][j]=image1[i][j]/image2[i][j];
		}
	}

	return result;
}
#endif