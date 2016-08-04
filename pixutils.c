#include "pixutils.h"
#include "lodepng.h"

//private methods
static pixMap *pixMap_init(); //allocate memory for pixMap object, set variables to zero, and return a pointer to the object
static void pixMap_reset();  //free the allocated memoray and set to zero but do not free memory for pixMap
static void pixMap_copy(pixMap *dest,pixMap *source); //copy one pixmap to another
static int pixMap_read(pixMap *p,char *filename); //read in a pixmap from file

static pixMap* pixMap_init(){
    //allocate memory for pixMap object, set variables to zero, and return a pointer to the object
 
    //create memory for pixMap pointer *p
    pixMap *p = malloc(sizeof(pixMap));

    //if pointer p doesn't exists
    if (!p)(exit(1));

    //set variables to zero
    p->image = 0; //same as *p->image

    //return pointer
    return (p);
}	

pixMap* pixMap_init_filename(char *filename){
    //use pixMap_int to create the pixMap object
    //use pixMap read to fill the fields of the object
    //return the pointer to the new object
	
    //created pointer with pixMap_init function
    pixMap *p = pixMap_init();

    //use pixMap read to fill the fields of the object
    pixMap_read(p, filename);

    //return the pointer to the new object
    return (p);
}

static int pixMap_read(pixMap *p,char *filename){
    //read and allocate image, read in width and height using using lodepng_decode32_file
    //example is in lodepng/examples - one liner
    //then allocate p->pixArray to hold p->height pointers
    //set p->pixArray[0] to p->image and p->pixArray[i]=p->pixArray[i-1]+p->width
	
    //variables
    int i = 0;

    //use lodepng_decode32_file for reading
    unsigned readImage = lodepng_decode32_file(&(p->image), &(p->width), &(p->height), filename);

    //if readImage exists
    if(readImage) {
        printf("Error %u: %s\n", readImage, lodepng_error_text(readImage));
    }

    //create pointer to pixel array
    p->pixArray = malloc(p->height*sizeof(rgba*));

    //if p doesn't exist
    if(!p)(exit(1));

    //set p->pixArray[0] to p->image
    p->pixArray[0] = (rgba*)p->image;

    //for as long as p's height is not reached
    for(i = 1; i < p->height; i++) {

        //set p->pixArray[i]=p->pixArray[i-1]+p->width
	p->pixArray[i] = p->pixArray[i-1] + p->width;
    }
	
    return readImage;
}	

static void pixMap_copy(pixMap *dest,pixMap *source){
	//if source image is zero then reset dest and copy width and height
	//if source image is not zero
	  //if width or height are different
	    //if width*height is different then we need to allocate dest->image
	      //if dest->image is zero use malloc to allocate memory for dest->image
	      //else use realloc to allocate memory dest->image
	    //
	    //if dest->height is different
	      //malloc or realloc dest->pixArray depending on whether dest->pixArray is zero
	    //
	    //even if the height is the same set dest->pixArray[0] to dest->image and dest->pixArray[i]=dest->pixArray[i-1]+source->width 
	  //
	  //do a memcpy from source->image to dest->image
	  //set dest->width and dest->height to source values
	//
	
    //variables
    int i = 0;

    //if source image is zero then reset dest and copy width and height
    if (source->image == 0) {

        //reset dest
	dest->image = 0;

	//copy width and height
	dest->height = source->height;
	dest->width = source->width;
    }
    //if source image is not zero
    else if (source->image != 0) {

	//if widths or heights of source and dest are different
	if (source->width != dest->width || source->height != dest->height) {

		//if the width*height of source and dest are not the same
		if (source->width * source->height != dest->width * dest->height) {

			//allocate memory for dest->image
			dest->image = malloc(source->height * source->width * sizeof(rgba));

			//if dest->image is zero
			if (dest->image == 0) {

				//allocate memory for dest->image
				dest->image = malloc(source->height * source->width * sizeof(rgba));
			}
			else {

				//realloc dest->image
				dest->image = realloc(dest->image, (source->height * source->width * sizeof(rgba)));
			}
		}

		//if dest height is not the same
		if(dest->height != source->height) {

			//use realloc to allocate memory dest->image
			dest->image = realloc(dest->image, (source->height * source->width * sizeof(rgba)));
		}

		//if both heights of dest and source are the same
		if(dest->height == source->height) {

			//make this equal to dest->image
			dest->pixArray[0] = (rgba*)dest->image;

			//use a for loop to set each value in pixel array with a different value
			for (i = 0; i < dest->height; i++) {

				dest->pixArray[i] = dest->pixArray[i-1]+source->width;
			}
		}
	}

	//memcpy from source to dest image
	memcpy(dest->image, source->image, source->height * source->width * sizeof(rgba));

	//set dest's width and height to souce values
	dest->width = source->width;
	dest->height = source->height;
    }
}

static void pixMap_reset(pixMap *p){
	//free all the memory in the fields and set everything to zero
	
	//if p and p->image exists
	if(p && p->image) {
		//free image
		free(p->image);
	}

	//free all other memory of p
	free(p->image);
	free(p->pixArray);

	//set everything to zero
	p->image = 0;
	p->pixArray = 0;
}	


void pixMap_destroy(pixMap *p){
	//reset pixMap and free the object p
	
	//reset p
	pixMap_reset(p);

	//free object p
	free(p);

	//set p to 0
	p = 0;
}

void pixMap_rotate (pixMap *p, float theta){
	
 //make a new temp blank pixMap structure
 //copy p to temp
 //set the values in the image to zero using memset - Note that the 3rd argument of memset is the size in BYTES
 
 	//make a new temp blank pixMap structure
	pixMap *temp = pixMap_init();

	//copy p to temp
	pixMap_copy(temp, p);

	//set values in image to zero using memset
	memset(temp->image, 0, (p->height * p->width * sizeof(rgba)));

 //calculate the coordinates ox and oy of the middle of the png graphic
 //calculate the values of sine and cosine used by the rotation formula FOR -theta

	//calculate the coordinates ox and oy of the middle of the png graphic
	int ox = p->height/2; //should give the middle height
	int oy = p->width/2; //should give the middle width
	int x = 0; //for width
	int y = 0; //for height
	
	//calculate the values of sine and cosine used by the rotation formula
	int cosValue = cos(degreesToRadians(-theta));
	int sinValue = sin(degreesToRadians(-theta));

	//for(int y=0;y<p->height;y++){   //two for loops to loop through each pixel in the new rotated image
	 //for(int x=0;x<p->width;x++){
	 
	    //calculate the old coordinates rotx roty by rotating by -theta and using the formula described here
	    //http://stackoverflow.com/questions/2259476/rotating-a-point-about-another-point-2d
	    //use the answer from stackoverflowery
	    
	    //However this answer assumes that y is going from the bottom to the top (mathematical convention)
	    //but the pixmap starts at the upper left hand corner and height grows DOWN (scan order)
	    //so use this formula instead where c is cos(degreesToRadians(-theta)) and s is sin(degreeToRadians(-theta))
	    //    float rotx = c*(x-ox) - s * (oy-y) + ox;
     //    float roty = -(s*(x-ox) + c * (oy-y) - oy);
	    
 	   //round the coordinates to the nearest integer in your calculations (add 0.5 and cast to integer)	
	
	    //if old coordinates are within the height and width limits
	      //copy the pixel at the old coords to the pixel to the temporary copy using memcpy
	      //i.e. 	memcpy(temp->pixArray[y]+x,p->pixArray[roty]+rotx,sizeof(rgba))
	      
	//use for loop to go through each pixel
	for(y = 0; y < p->height; y++) {
		for(x = 0; x < p->width; x++) {
			//calculate new rotated coordinates

			//create variables for storing rotated x and y
			float rotx = cosValue * (x-ox) - sinValue * (oy-y) + ox;
			float roty = - (sinValue * (x-ox) + cosValue * (oy-y) - oy);

			//add 0.5 to rotx and roty to get nearest integer and cast them in int
			rotx = rotx + 0.5;
			int rotxInt = (int)rotx;

			roty = roty + 0.5;
			int rotyInt = (int)roty;

			//if rotated coordinates are within the height & width of array
			if((rotxInt < (p->width) && rotxInt >= 0) && (rotyInt < (p->height)  && rotyInt >= 0)) {
				//copy the pixel at the old coords to the pixel to the temporary copy using memcpy
				//i.e. 	memcpy(temp->pixArray[roty]+rotx,p->pixArray[y]+x,sizeof(rgba))

				//place pixel from original to a different position in temp
				memcpy(temp->pixArray[y]+x, p->pixArray[rotyInt]+rotxInt, sizeof(rgba));
			}
		}
	}
	
	//copy the temp pixMap to the original
	//destroy the temp;
	
	//copy temp to original
	pixMap_copy(p, temp);

	//get rid of temp
	pixMap_destroy(temp);
}	

void pixMap_gray (pixMap *p){
	//for() loop through pixels using two for loops 
	  //for()
	    //calculate average value of r and g and b values (assign to a float variable)
	    //for example the red value of for the pixel at height i, width j would be p->pixel[i][j].r
	    //round float variable to integer (add 0.5 before casting to integer)
	    //assign the rounded value to r,g and b values
	  //
	//   
	
	//variables
	int i = 0;
	int j = 0;
	float sum = 0;
	float avgValue = 0;

	//for as long as p->height is not reached
	for(i = 0; i < p->height; i++) {

		//for as long as p->width is not reached
		for(j = 0; j < p->width; j++) {

			//add values of r, g, and b together
			sum = p->pixArray[i][j].r + p->pixArray[i][j].g + p->pixArray[i][j].b;

			//calculate average and add 0.5 to round it up
			avgValue = (sum/3) + 0.5;

			//cast float into int
			int avgIntValue = (int)(avgValue);

			//assign rounded value to r/g/b values
			p->pixArray[i][j].r = avgIntValue;
			p->pixArray[i][j].g = avgIntValue;
			p->pixArray[i][j].b = avgIntValue;
		}
	}
}

int pixMap_write(pixMap *p,char *filename){
 //write out to filename using lodepng_encode32_file
 //example is in lodepng/examples - one liner
 
 	//create writeImage to write to filename
	unsigned writeImage = lodepng_encode32_file(filename, (p-> image), (p-> width), (p-> height));

	//if writeImage exists
	if(writeImage) {
		printf("Error %u: %s\n", writeImage, lodepng_error_text(writeImage));
	}
	
	return 0;
}	 
