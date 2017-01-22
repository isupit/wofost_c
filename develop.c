#include <stdio.h>
#include "astro.h"
#include "dynamic.h"  
#include "wofost.h"

float GetDevelopmentStage(void)
{
  float DevelopmentRate;
  
  if (DevelopmentStage  < 1.)
    {
    DevelopmentRate = Afgen(DeltaTempSum, &Temp)/TempSum1;
    if (IdentifyAnthesis == 1 || IdentifyAnthesis == 2) DevelopmentRate = DevelopmentRate*
	  limit(0., 1., (PARDaylength-CriticalDaylength)/(OptimumDaylength-CriticalDaylength));
    
    /* adjustment of development stage */
    if (DevelopmentRate + DevelopmentStage >= 1. ) return 1.;
    }
  else 
       DevelopmentRate = Afgen(DeltaTempSum, &Temp)/TempSum2;  

  return (DevelopmentStage + DevelopmentRate);

}
