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


int leap_year(int *year)
{
    if(*year%4 == 0)
    {
        if( *year%100 == 0)
        {
            // year is divisible by 400, hence the year is a leap year
            if ( *year%400 == 0)
                return 366;
            else
                return 365;
        }
        else
            return 366;
    }
    else
        return 365;
    
}

