#include <stdio.h>

float limit(float a, float b, float c)
{
    if (c < a) return a;
    else if (c >= a && c <= b)  return c;
    else return b;
   }


float notnul(float x)
{
    if (x != 0.) return x;
    else return 1.;
   }

float insw(float x1, float x2, float x3)
{
    if(x1 < 0.)
        return x2;
    else
        return x3;
}