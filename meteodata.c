#include <stdio.h>
#include "wofost.h"

int GetMeteoData()
{
  int c, i, day;
  float crap1, crap2, Rad;
  FILE *fq;

 if ((fq = fopen("../data/nl1.973", "rt")) == NULL)
    {fprintf(stderr, "Cannot open input \file.\n"); return 0;}

  while ((c=fgetc(fq)) == '*') 
     while ((c=fgetc(fq)) != '\n');
 
 fscanf(fq,"%f %f %f %f %f", &Longitude, &Latitude, &Altitude,  &crap1, &crap2);
 for (i=0;i<366;i++)
    {fscanf(fq,"%d %d %d %f %f %f %f %f %f", &Station, &Year, &day, &Rad,
                                          &Tmin[i], &Tmax[i], &Vapour[i],
					  &Windspeed[i], &Rain[i]);		

/* Transform Radiation from KJ m-2 d-1 to J m-2 d-1 */
    Radiation[i] = 1000.*Rad;
    }				                                    
 return 1;
}
