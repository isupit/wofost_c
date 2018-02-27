#include <stdio.h>
#include "astro.h"
#include "extern.h"
#include "wofost.h"

/* ----------------------------------------------------------------------------*/
/*  function GetDevelopmentStage()                                             */
/*  Purpose: Calculation of the development stage as function of the effective */
/*  daily temperature                                                          */
/*-----------------------------------------------------------------------------*/

float GetDevelopmentStage(void)
{
    float DevelopmentRate;

    if (Crop.DevelopmentStage  < 1.)
    {
        DevelopmentRate = Afgen(Crop.prm.DeltaTempSum, &Temp)/Crop.prm.TempSum1;
        if (Crop.prm.IdentifyAnthesis == 1 || Crop.prm.IdentifyAnthesis == 2) DevelopmentRate = DevelopmentRate *
                limit(0., 1., (PARDaylength-Crop.prm.CriticalDaylength)/(Crop.prm.OptimumDaylength-Crop.prm.CriticalDaylength));

        /* adjustment of development stage */
        if (DevelopmentRate + Crop.DevelopmentStage >= 1. ) return 1.;
    }
    else 
         DevelopmentRate = Afgen(Crop.prm.DeltaTempSum, &Temp)/Crop.prm.TempSum2;  

return (Crop.DevelopmentStage + DevelopmentRate);

}
