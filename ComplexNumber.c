/*********************
**  Complex Numbers
**  This file contains a few functions that will be useful when performing computations with complex numbers
**  It is advised that you work on this part first.
**********************/

#include "ComplexNumber.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct ComplexNumber
{
    double real;
    double imaginary;
}ComplexNumber;

//Returns a pointer to a new Complex Number with the given real and imaginary components
ComplexNumber* newComplexNumber(double real_component, double imaginary_component)
{ 
    ComplexNumber* newnum = (ComplexNumber*)malloc(sizeof(ComplexNumber));
    newnum->real = real_component;
    newnum->imaginary = imaginary_component;
    return newnum;
}

//Returns a pointer to a new Complex Number equal to a*b
ComplexNumber* ComplexProduct(ComplexNumber* a, ComplexNumber* b)
{
    ComplexNumber* multnum = (ComplexNumber*)malloc(sizeof(ComplexNumber));
    multnum->real = a->real * b->real - a->imaginary * b->imaginary;
    multnum->imaginary = a->imaginary * b->real + a->real * b->imaginary;
    return multnum;
}

//Returns a pointer to a new Complex Number equal to a+b
ComplexNumber* ComplexSum(ComplexNumber* a, ComplexNumber* b)
{
    ComplexNumber* addnum = (ComplexNumber*)malloc(sizeof(ComplexNumber));
    addnum->real = a->real + b->real;
    addnum->imaginary = a->imaginary + b->imaginary;
    return addnum;
}

//Returns the absolute value of Complex Number a
double ComplexAbs(ComplexNumber* a)
{
    double absval;
    absval = sqrt(pow(a->real, 2) + pow(a->imaginary, 2));
    return absval;
}

void freeComplexNumber(ComplexNumber* a)
{
    free(a);
}

double Re(ComplexNumber* a)
{
    return a->real;
}
double Im(ComplexNumber* a)
{
    return a->imaginary;
}


//Contains a few tests.
int test_complex_number()
{
    int fail = 0;
    ComplexNumber* a = newComplexNumber(2.0, 1.0);
    if (a == NULL)
    {
    	fprintf(stderr, "Creation of complex numbers not implemented\n");
    	return 0;
    }
    ComplexNumber* b = ComplexProduct(a, a);
    if (b == NULL)
    {
    	fprintf(stderr, "Multiplication of complex numbers not implemented\n");
    	free(a);
    	return 0;
    }
    ComplexNumber* c = ComplexSum(b, b);
    if (c == NULL)
    {
    	fprintf(stderr, "Addition of complex numbers not implemented\n");
    	free(a);
    	free(b);
    	return 0;
    }
    float d = ComplexAbs(c);
    if (d == 0)
    {
    	fprintf(stderr, "Absolute Value of complex numbers not implemented\n");
    	free(a);
    	free(b);
    	free(c);
    	return 0;
    }
    else if (fabsf(d - 10) < 0.0001)
    {
    	//passes
    	free(a);
    	free(b);
    	free(c);
    	return 1;
    }
    else
    {
    	fprintf(stderr, "At least one of your functions is incorrect\n");
    	free(a);
    	free(b);
    	free(c);
    	return 0;
    }
    free(a);
    free(b);
    free(c);
    return 0;
}
