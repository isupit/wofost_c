#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include "wofost.h"
#include "extern.h"

/* ---------------------------------------------------------------------------*/
/*  function RateCalculationCrop()                                            */
/*  Purpose: Calculate the net amount of assimilates that is available for    */
/*  crop growth and subsequently establish the crop growth rates for the      */
/*  plant organs (kg ha-1 d-1).                                               */
/* ---------------------------------------------------------------------------*/

void RateCalculationCrop()
{
    float TotalAssimilation;
    float Maintenance;
    float GrossAssimilation;
    float GrossGrowth;
    float Stress;

    /* Assimilation */
    GrossAssimilation = DailyTotalAssimilation();
    
    /* Stress: either nutrient shortage or water shortage */
    Stress = min(Crop->NutrientStress, WatBal->WaterStress);

    /* Correction for low minimum temperatures and stress factors */
    TotalAssimilation = Stress * Correct(GrossAssimilation);       
    
    /* Respiration */
    Maintenance = RespirationRef(TotalAssimilation);

    /* Conversion */
    GrossGrowth = Conversion(TotalAssimilation-Maintenance); 
    //printf("%4d %8.6f %8.6f\n", Crop->GrowthDay,Stress,TotalAssimilation);
    /* Growth of roots, stems, leaves and storage organs */
    Growth(GrossGrowth);
    
    NutrientLoss();
    
    CropNutrientRates();
    
    /* Development rate calculation */
    DevelopmentRate();
    
           
    //printf("  Dmi: %5.1f MRes: %5.1f Gass: %5.1f RtSt: %5.1f", GrossGrowth, Maintenance, TotalAssimilation, Crop->rt.stems );
}
