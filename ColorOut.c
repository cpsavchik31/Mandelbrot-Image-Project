#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorOut.h"

/**************
**Translate pixel value given in MandelOut array into a color that can be found in the colorMap array
**Length of the MandelOut array = size * size
**Output should be written to outputfile with P3 format
***************/
int P3colorpalette(u_int64_t size, uint8_t** colorMap, int colorcount, u_int64_t * MandelOut, char* outputfile)
{
    FILE* fp;
    //CHECK SPACING BETWEEN PIXELS + NEW LINE BETWEEN EACH PIXEL
    fp = fopen(outputfile, "w");
    //write header
    fprintf(fp, "%s %ld %ld %d\n", "P3", size, size, 255);
    //iterate through mandelout array to access pixels
    int len = size * size;
    for (int i = 0; i < len; i++) {
        int val = *(MandelOut + i);
        if (val == 0) {
            //write as black
            fprintf(fp, "%d %d %d\n", 0, 0, 0);
            continue;
        }
        if (val > colorcount) {
            //value greater than # of colors available
                val = val % colorcount;
                if (val == 0) {
                    val = 6;
                }
        }
        //account for indexing starting at zero
        val = val - 1;
        //pointer to rgb color array for specific pixel
        u_int8_t* rgb0 = *(colorMap + val);
        fprintf(fp, "%d %d %d\n", *rgb0, *(rgb0 + 1), *(rgb0 + 2));
    }
    fclose(fp);
    return 0;
}

/**************
**Same as above, but with P6 format
***************/
int P6colorpalette(u_int64_t size, uint8_t** colorMap, int colorcount, u_int64_t * MandelOut, char* outputfile)
{
    FILE* fp2;
    fp2 = fopen(outputfile, "w");
    //write header, is this always constant?
    fprintf(fp2, "%s %ld %ld %d\n", "P6", size, size, 255);
    int len = size * size;
    int checker = 0;
    for (int l = 0; l < len; l++) {
        int val1 = *(MandelOut + l);
        if (val1 == 0) {
            //write as black
            char z = (char) 0;
            for (int j = 0; j < 3; j++) {
                fwrite(&z, sizeof(char), 1, fp2);
            }
            continue;
        }
        if (val1 > colorcount) {
            //value greater than # of colors available
            val1 = (val1 % colorcount);
            if (val1 == 0) {
                val1 = 6;
            }
        }
        val1 = val1 - 1;
        u_int8_t* rgb = *(colorMap + val1);
        char r = (char) *rgb;
        fwrite(&r, sizeof(char), 1, fp2);
        char g = (char) *(rgb + 1);
        fwrite(&g, sizeof(char), 1, fp2);
        char b = (char) *(rgb + 2);
        fwrite(&b, sizeof(char), 1, fp2);
    }
    //fputc('\n', fp2);
    fclose(fp2);
    return 0;
} 