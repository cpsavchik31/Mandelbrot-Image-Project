#include <stdio.h>
#include <stdlib.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include <sys/types.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


/*
This function returns the number of iterations before the initial point >= the threshold.
If the threshold is not exceeded after maxiters, the function returns 0.
*/
u_int64_t MandelbrotIterations(u_int64_t maxiters, ComplexNumber * c, double threshold)
{
    u_int64_t count = 0;
    ComplexNumber* z = newComplexNumber(0,0);
    double newr;
    double newi;
    while (count < maxiters && ComplexAbs(z) < threshold) { 
        newr = Re(z) * Re(z) - Im(z) * Im(z) + Re(c);
        // 2*z_real*z_imaginary + c_imaginary 
        newi = 2 * Re(z) * Im(z) + Im(c);
        // Updating z_real = tempx 
        freeComplexNumber(z);
        z = newComplexNumber(newr, newi);
        count++;
    }
    if (ComplexAbs(z) >= threshold) {
        freeComplexNumber(z);
        return count;
    }
    freeComplexNumber(z);
    return 0;
} 

/*
This function calculates the Mandelbrot plot and stores the result in output.
The number of pixels in the image is resolution * 2 + 1 in one row/column. It's a square image.
Scale is the the distance between center and the top pixel in one dimension.
*/
void Mandelbrot(double threshold, u_int64_t max_iterations, ComplexNumber* center, double scale, u_int64_t resolution, u_int64_t* output){
    u_int64_t size = 2 * resolution + 1;
    u_int64_t iters = size * size;
    /*check if 8 truly the correct number of bytes*/
    unsigned int rowcount = 0;
    unsigned int colcount = 0;
    //increment to change real/complex values 
    double inc = scale /resolution;
    //top to bottom currvalr is decremented by inc(scale/resolution) 
    double currvalr = scale;
    //left to right -scale is incremented by inc(scale/resolution)
    double currvalc = -scale;
    bool withinrow = true;
    //need to preserve decimal
    double cc = -scale;
    double rr = scale;
    for (unsigned int i = 0; i < iters; i++) {
        //generate mandelbrot funct
        if (colcount == 2 * resolution) {
            //hit end of row
            withinrow = false;
        }
        if (withinrow) {
            //ComplexNumber* tempptr = newComplexNumber(currvalc, currvalr);
            //ComplexNumber* tempptr = newComplexNumber(cc, rr);
            double rsum = cc + Re(center);
            double isum = rr + Im(center);
            //ComplexNumber* sumit0 = ComplexSum(tempptr, center);
            ComplexNumber* sumit0 = newComplexNumber(rsum, isum);
            *(output + i) = MandelbrotIterations(max_iterations, sumit0, threshold);
            colcount++;
            //currvalc = currvalc + inc;
            //free(tempptr);
            free(sumit0);
            //cc = -scale + (scale / resolution) * (double)colcount;
            //ADVANCE COLUMN
            cc = scale * colcount;
            cc = cc / resolution;
            cc = -scale + cc;
        }
        else {
            //reset variables for next row of iterations
            //currvalc = -scale;
            double rsum1 = cc + Re(center);
            double isum1 = rr + Im(center);
            //ComplexNumber* sumit0 = ComplexSum(tempptr, center);
            ComplexNumber* sumit1 = newComplexNumber(rsum1, isum1);
            /*ComplexNumber* newrowptr = newComplexNumber(cc, rr);
            ComplexNumber* sumit = ComplexSum(newrowptr, center);*/
            *(output + i) = MandelbrotIterations(max_iterations, sumit1, threshold);
            free(sumit1);
            cc = -scale;
            colcount = 0;
            rowcount++;
            //ADVANCE ROW
            //rr = scale - (double)(scale/resolution)*(double)rowcount;
            rr = scale / resolution;
            rr = rr * rowcount;
            rr = scale - rr;
            withinrow = true;
            
            //free(newrowptr);
        }
    }
}

//int main() {
//
//    ComplexNumber* test = newComplexNumber(1,2);
//    double threshold = 5;
//    u_int64_t maxiterations = 100;
//    ComplexNumber* center = newComplexNumber(4, 4);
//    double scale = 2;
//    u_int64_t resolution = 3;
//    u_int64_t* output;
//    output = (u_int64_t*)malloc((2 * resolution + 1) * sizeof(u_int64_t));
//    Mandelbrot(threshold, maxiterations, center, scale, resolution, output);
//    u_int64_t* myPtr = (u_int64_t*)malloc(sizeof(u_int64_t));
//            for (int x = 0; x < pow(2*resolution+ 1, 2); x++) {
//                *myPtr = output[x];
//                printf("%ld\n", *myPtr);
//               }
//            free(myPtr);
//    return 0;
//
//}


