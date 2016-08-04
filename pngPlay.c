#include "pixutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
  char *inputfile=0,*outputfile=0;
  float degrees=0, grayFlag=0;
  int option = 0;
  
  //write the parser yourself or use a package like https://github.com/skeeto/optparse
  
		//check for flags -i -o -r -g - can be in any order
  //-i is followed by the input png
  //-o is followed the output png
  //-r is followd by the rotation angle in degrees (float) <optional for user>
  //-g is whether th png should be grayed <optional for user>
  
  pixMap *p=pixMap_init_filename(inputfile);
  if(degrees)pixMap_rotate(p,degrees);
  if(grayFlag)pixMap_gray(p);
  pixMap_write(p,outputfile);
  pixMap_destroy(p);
  
  //while any option is i/o/r/g and not -1
  while((option = getopt(argc, argv, "i:o:r:g")) != -1) {

	  //switch cases for i/o/r/g
	  switch(option) {

		  //input file
		  case 'i':
			  inputfile = optarg;
			  fprintf(stderr,"The input file is %s\n", inputfile);
			  break;

		  //output file
		  case 'o':
			  outputfile = optarg;
			  fprintf(stderr,"The output file is %s\n", outputfile);
			  //write out to output file
			  pixMap_write(p, outputfile);
			  break;

		  //if user wants to rotate image
		  case 'r':
			  degrees = atof(optarg);
			  fprintf(stderr,"The image is being rotated by %f degrees\n", degrees);
			  //if degrees exist
			  if(degrees) {
				  //rotate image by this amount of degrees
				  pixMap_rotate(p,degrees);
			  }
			  break;

		  //if user wants to gray image
		  case 'g':
			  grayFlag = atof(optarg);
			  fprintf(stderr,"The grayFlag is %f\n", grayFlag);
			  //if grayFlag exists
			  if(grayFlag) {
				  //gray image
				  pixMap_gray(p);
			  }
	  }
  }
  
  return 0;
}

