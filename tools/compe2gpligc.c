#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <proj_api.h>
#include <math.h>

#define WGS84_SCALE 1
#define NOF_ARGS 4
#define BUFF_SIZE 255
#define MIN_PIX_DIST 100.0

void printUsage()
{
	printf("usage: compe2gpligc [-w | -u] -f xyz.imp\n\n");
}

int main(int argc, char **argv)
{
	typedef enum {UTM, WGS84}ScaleModeType;
	FILE *input_fp;
	char buffer[BUFF_SIZE];
	char jpgFile[BUFF_SIZE];
	double jpgx1;
	double jpgy1;
	double jpgx2;
	double jpgy2;
	double scaleX;
	double scaleY;
	int pixX;
	int pixY; 
	char *args[NOF_ARGS] = {"proj=utm", "ellps=WGS84", "x_0=500000"};
	char zoneString[10];
	double deltaXpix;
	double deltaYpix;
	projPJ proj;
	projUV projPt_1utm;
	projUV projPt_2utm;
	projUV projPt_1wgs84;
	projUV projPt_2wgs84;
	char c;
	char *filename;
	ScaleModeType scaleMode = UTM;
	
	while((c = getopt(argc, argv, "f:uw")) != -1)
	{
		switch(c)
		{
			case 'u':
				scaleMode = UTM;
			break;
			case 'w':
				scaleMode = WGS84;
			break;
			case 'f':
				filename = optarg;
			break;
			case '?':
				if(isprint(optopt))
				{
					printf("Unknown option `-%c'.\n", optopt);
				}
				else
				{
					printf("Unknown option character `\\x%x'.\n", optopt);
				}
				printUsage();
				return -1;
			default:
				return -1;
		}
	}
	
	input_fp = fopen(filename, "r");
	
	if(input_fp == NULL)
	{
		printf("Error: Can not open file.\n");
		printUsage();
		return -1;
	}
	
	/* jpg filename */
	fgets(jpgFile, BUFF_SIZE, input_fp);
	
	/* jpg1 */
	fgets(buffer, BUFF_SIZE, input_fp);
	jpgx1 = atof(buffer);
	fgets(buffer, BUFF_SIZE, input_fp);
	jpgy1 = atof(buffer);
	
	/* utm1 */
	fgets(buffer, BUFF_SIZE, input_fp);
	projPt_1utm.u = atof(buffer);
	fgets(buffer, BUFF_SIZE, input_fp);
	projPt_1utm.v = atof(buffer);
	
	/* jpg2 */
	fgets(buffer, BUFF_SIZE, input_fp);
	jpgx2 = atof(buffer);
	fgets(buffer, BUFF_SIZE, input_fp);
	jpgy2 = atof(buffer);
	
	/* utm2 */
	fgets(buffer, BUFF_SIZE, input_fp);
	projPt_2utm.u = atof(buffer);
	fgets(buffer, BUFF_SIZE, input_fp);
	projPt_2utm.v = atof(buffer);
	
	/* skip line */
	fgets(buffer, BUFF_SIZE, input_fp);
	
	/* pix */
	fgets(buffer, BUFF_SIZE, input_fp);
	pixX = (int)atof(buffer);
	fgets(buffer, BUFF_SIZE, input_fp);
	pixY = (int)atof(buffer);
	
	/* zone */
	fgets(buffer, BUFF_SIZE, input_fp);
	sprintf(zoneString, "zone=%2i", atoi(buffer));
	args[3] = zoneString;
	
	deltaXpix = fabs(jpgx2 - jpgx1);
	deltaYpix = fabs(jpgy2 - jpgy1);
	
	proj = pj_init(NOF_ARGS, args);

	if(scaleMode == WGS84)
	{
		printf("scale with WGS84\n");
		
		/* calc projection */
		projPt_1wgs84 = pj_inv(projPt_1utm, proj);
		projPt_2wgs84 = pj_inv(projPt_2utm, proj);
		
		scaleX = fabs((projPt_2wgs84.u - projPt_1wgs84.u) / deltaXpix);
		scaleY = fabs((projPt_2wgs84.v - projPt_1wgs84.v) / deltaYpix);
	}
	else
	{
		printf("scale with UTM\n");
		
		scaleX = fabs((projPt_2utm.u - projPt_1utm.u) / (jpgx2 - jpgx1));
		scaleY = fabs((projPt_2utm.v - projPt_1utm.v) / (jpgy2 - jpgy1));
	}
	
	if((deltaXpix < MIN_PIX_DIST) && (deltaYpix < MIN_PIX_DIST))
	{
		printf("Error: Points are to near. It's not possible to calculate scale.\n");
		return -1;
	}
	else if(deltaYpix < MIN_PIX_DIST)
	{
		printf("Warning: y points are to near. Take x scale for y\n");
		scaleY = scaleX;
	}
	else if(deltaXpix < MIN_PIX_DIST)
	{
		printf("Warning: x points are to near. Take y scale for x\n");
		scaleX = scaleY;
	}

	printf("scaleX=%f, scaleY=%f\n", scaleX, scaleY);

	if(scaleMode == WGS84)
	{
		/* move to 0,0 */
		projPt_1wgs84.u -= scaleX * jpgx1;
		projPt_1wgs84.v -= scaleY * jpgy1;
	
		/* move to end,end */
		projPt_2wgs84.u = projPt_1wgs84.u + scaleX * pixX;
		projPt_2wgs84.v = projPt_1wgs84.v + scaleY * pixY;
	}
	else
	{
		/* move to 0,0 */
		projPt_1utm.u -= scaleX * jpgx1;
		projPt_1utm.v -= scaleY * jpgy1;
	
		/* move to end,end */
		projPt_2utm.u = projPt_1utm.u + scaleX * pixX;
		projPt_2utm.v = projPt_1utm.v + scaleY * pixY;
		
		/* calc projection */
		projPt_1wgs84 = pj_inv(projPt_1utm, proj);
		projPt_2wgs84 = pj_inv(projPt_2utm, proj);
	}

	printf("\n%s", jpgFile);
	printf("MAP_LEFT %f\n", projPt_1wgs84.u*RAD_TO_DEG);
	printf("MAP_BOTTOM %f\n", projPt_1wgs84.v*RAD_TO_DEG);
	printf("MAP_RIGHT %f\n", projPt_2wgs84.u*RAD_TO_DEG);
	printf("MAP_TOP %f\n", projPt_2wgs84.v*RAD_TO_DEG);

	return 0;
}
