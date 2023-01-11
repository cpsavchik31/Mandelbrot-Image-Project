#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include "ColorMapInput.h"
#include "ColorOut.h"
#include <sys/types.h>
#include <string.h>
 
void printUsage(char* argv[])
{
    printf("Usage: %s <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>\n", argv[0]);
    printf("    This program simulates the Mandelbrot Fractal, and creates an iteration map of the given center, scale, and resolution, then saves it in output_file\n");
}


/*
This function calculates the threshold values of every spot on a sequence of frames. The center stays the same throughout the zoom. First frame is at initialscale, and last frame is at finalscale scale.
The remaining frames form a geometric sequence of scales, so 
if initialscale=1024, finalscale=1, framecount=11, then your frames will have scales of 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1.
As another example, if initialscale=10, finalscale=0.01, framecount=5, then your frames will have scale 10, 10 * (0.01/10)^(1/4), 10 * (0.01/10)^(2/4), 10 * (0.01/10)^(3/4), 0.01 .
*/
void MandelMovie(double threshold, u_int64_t max_iterations, ComplexNumber* center, double initialscale, double finalscale, int framecount, u_int64_t resolution, u_int64_t ** output){
    double mul = pow((finalscale/initialscale), 1.0/(framecount-1));
    double scale = initialscale;
    for(int i = 0; i < framecount; i++){
    	Mandelbrot(threshold, max_iterations, center, scale, resolution, output[i]); 
    	scale *= mul;
    }
}

/**************
**This main function converts command line inputs into the format needed to run MandelMovie.
**It then uses the color array from FileToColorMap to create PPM images for each frame, and stores it in output_folder
***************/
int main(int argc, char* argv[]){
    //Tips on how to get started on main function: 
    //MandelFrame also follows a similar sequence of steps; it may be useful to reference that.
    //Make sure you complete the steps below in order. 

    //STEP 1: Convert command line inputs to local variables, and ensure that inputs are valid.
    /*
    Check the spec for examples of invalid inputs.
    Remember to process the colorfile.
    */

    if (argc != 11) {
        printf("MainError\n");
        return 1;
    }
    double threshold, initialscale, finalscale;
    ComplexNumber* center;
    u_int64_t max_iterations, resolution;
    int framecount;
    threshold = atof(argv[1]);
    max_iterations = (u_int64_t)atoi(argv[2]);
    center = newComplexNumber(atof(argv[3]), atof(argv[4]));
    initialscale = atof(argv[5]);
    finalscale = atof(argv[6]);
    framecount = atoi(argv[7]);
    resolution = (u_int64_t)atoi(argv[8]);
    u_int8_t** colormaparray;
    u_int64_t **output;
    char* outputfolder = argv[9];
    char* colorfile = argv[10];
    int* colorcount = (int*)malloc(sizeof(int));
    //check validity of variables
    if (threshold < 0 || max_iterations < 0 || initialscale < 0 || finalscale < 0 || framecount > 10000 || framecount < 0 || resolution < 0) {
        printf("Main Error\n");
        return 1;
    }
    if (framecount == 1 && (initialscale != finalscale)) {
        printf("Main Error\n");
        return 1;
    }
    //allocate space for arrays
    //initialize colormaparray
    colormaparray = FileToColorMap(colorfile, colorcount);
    if (colormaparray == NULL) {
        printf("Main Error\n");
        return 1;
    }
    output = (u_int64_t**)malloc(sizeof(u_int64_t*) * framecount);
    if (output == NULL) {
        /*for (int i = 0; i < framecount; i++) {
            free(output[i]);
        }
         free(output);*/
         freeMap(*colorcount, colormaparray);
         printf("Main Error\n");;
         return 1;
            }
    //allocate space for nested arrays in output
    for (int i = 0; i < framecount; i++) {
        output[i] = (u_int64_t*)malloc(sizeof(u_int64_t) * pow(2 * resolution + 1, 2));
    }
    //call mandel movie to update output array
    MandelMovie(threshold, max_iterations, center, initialscale, finalscale, framecount, resolution, output);
    //STEP 3: Output the results of MandelMovie to .ppm files.
    /*
    Convert from iteration count to colors, and output the results into output files.
    Use what we showed you in Part B.1.2, create a seqeunce of ppm files in the output folder.
    Feel free to create your own helper function to complete this step.
    As a reminder, we are using P6 format, not P3.
    */
    //**Translate pixel value given in MandelOut array into a color that can be found in the colorMap array
      //  ** Length of the MandelOut array = size * size
        u_int64_t size = 2 * resolution + 1;
        char* fileptr = (char*)malloc(sizeof(char)*50);
        for (int i = 0; i < framecount; i++){
            sprintf(fileptr, "%s/frame%05d.ppm", outputfolder, i);
            P6colorpalette(size, colormaparray, *colorcount, output[i], fileptr);
            }
    //STEP 4: Free all allocated memory
    /*
    Make sure there's no memory leak.
    */
         for (int i = 0; i < framecount; i++) {
             free(output[i]);
            }
         free(output);
         freeMap(*colorcount, colormaparray);
         freeComplexNumber(center);
         free(colorcount);
         free(fileptr);
    return 0;
}


