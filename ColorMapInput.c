/*********************
**  Color Map generator
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"


/**************
**This function frees the memory allocated for colorMap.
**Length represents the number of elements in colorMap 
***************/
void freeMap(int length, uint8_t** colorMap)
{
    for(int x = 0; x < length; x++){
        free(colorMap[x]);
    }
    free(colorMap);
}

/**************
**This function reads in a file name colorfile.
**It then uses the information in colorfile to create a color array, with each color represented by an int[3].
***************/
uint8_t** FileToColorMap(char* colorfile, int* colorcount){
   /* Therefore, output[i] points to an array of size 3 that contains the RGB value. (output[i][0] = R, output[i][1Co] = G, output[i][2] = B)*/
    //CHECK SPACING BETWEEN PIXELS + NEW LINE BETWEEN EACH PIXEL
    FILE* f;
    f = fopen(colorfile, "r");
    //u_int8_t** d_array = NULL;
    int numColors;
    if (f == NULL) {
        return NULL;
    }
    if (fscanf(f, "%d", &numColors) != 1) {
        fclose(f);
        return NULL;
    }
    *colorcount = numColors;
    if (*colorcount == 0) {
        fclose(f);
        return NULL;
    }
    u_int8_t** d_array = (u_int8_t**)malloc(sizeof(u_int8_t*) * (*colorcount));
    int r, g, b;
    int x = 1;
    int y = 0;
    for (int i = 0; i < *colorcount; i++) {
        //access current array of rgb 
        if (fscanf(f, "%d%d%d", &r, &g, &b) == 3) {
            if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
                x = 0;
                break;
            }
            d_array[i] = (u_int8_t*)malloc(3 * sizeof(u_int8_t));
            //assign temp ptr to nested array 
            d_array[i][0] = (uint8_t)r;
            d_array[i][1] = (uint8_t)g;
            d_array[i][2] = (uint8_t)b;
            y = i + 1;
        }
        else {
            x = 0;
            break;
        }
     }
    if (x == 0) {
        freeMap(y, d_array);
        fclose(f);
        return NULL;
    }
        fclose(f);
        return d_array;
    }
//int main() {
//
//    int* ptr;
//
//    uint8_t** myTestMap;
//
//    uint8_t* myPtr;
//
//    myTestMap = FileToColorMap("defaultcolormap.txt", ptr);
//
//    if (!myTestMap) {
//
//        printf("Program Fault: Broken\n");
//
//    }
//
//    else {
//
//        int x = 0;
//
//        for (x = 0; x < *ptr; x++) {
//
//            myPtr = myTestMap[x];
//
//            printf("%i\t%i\t%i\n", myPtr[0], myPtr[1], myPtr[2]);
//
//        }
//
//        freeMap(4, myTestMap);
//
//    }
//
//    free(ptr);
//
//    return 0;
//
//}