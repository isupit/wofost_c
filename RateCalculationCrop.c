#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include "wofost.h"
#include "extern.h"

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

void RateCalculationCrop()
{
       float TotalAssimilation;
       float Maintenance, GrossAssimilation, GrossGrowth;
       float Stress;
       
       /* Set rates to 0 */
       Crop.rt.roots   = 0.;
       Crop.rt.leaves  = 0.;
       Crop.rt.stems   = 0.;
       Crop.rt.storage = 0.;
       Crop.rt.LAIExp  = 0.;      
      
       /* Assimilation */
       GrossAssimilation = DailyTotalAssimilation();
       
       /* Establish WatBal.WaterStress */
       EvapTra();
       
       /* Stress: either nutrient shortage or water shortage */
       Stress = min(Crop.NutrientStress, WatBal.WaterStress);
       //Stress = 1.;

       /* Correction for low minimum temperatures and stress factors */
       TotalAssimilation = Stress * Correct(GrossAssimilation);       

       /* Respiration */
       Maintenance = RespirationRef(TotalAssimilation);

       /* Conversion */
       GrossGrowth = Conversion(TotalAssimilation-Maintenance); 
      
       /* Growth of root, stems, leaves and storage organs */
       Growth(GrossGrowth);
           
       printf("  Dmi: %5.1f MRes: %5.1f Gass: %5.1f RtSt: %5.1f", GrossGrowth, Maintenance, TotalAssimilation, Crop.rt.stems );
}
