#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mmla.h"
#include "dynamic.h"
#include "wofost.h"

float LeaveGrowth(float LAIExp, float NewLeaves)
{
  float GrowthExpLAI;
  float GrowthSourceLimited;
  float SpecLeafArea; 
  float Stress;
  
  Green *New=NULL;
  Green *LeaveProperties=NULL;
  

  /* Specific Leaf area(m2/g), as dependent on NPK stress */
  SpecLeafArea = Afgen(SpecificLeaveArea, &DevelopmentStage)* exp(-NutrientStessSLA * (1.-Crop.NPK_Indx));

 /* Leave area not to exceed exponential growth */
  if (LAIExp < 6 && NewLeaves > 0.) 
  {
      GrowthExpLAI = LAIExp*RelIncreaseLAI*max(0.,Temp - TempBaseLeaves);
      if (DevelopmentStage < 0.2 && LAI < 0.75)
      {
        Stress = WatBal.WaterStress * exp(-NitrogenStressLAI * (1. - Crop.N_st.Indx));
      }
      
      /* Correction for nutrient stress */
      GrowthExpLAI = GrowthExpLAI * Stress;
    
      /* Source limited leaf area increase */
      GrowthSourceLimited = NewLeaves* Afgen(SpecificLeaveArea, &DevelopmentStage);
    
      /* Sink-limited leaf area increase */
      SpecLeafArea = min(GrowthExpLAI, GrowthSourceLimited)/NewLeaves;
 }
 else
    GrowthExpLAI = 0.;
        
  New         = malloc(sizeof(Green));
  New->weight = NewLeaves;
  New->age    = 0.;
  New->area   = SpecLeafArea;
  New->next   = NULL;
  
  /* Store the start address */
  LeaveProperties = Crop.LeaveProperties;
  
  /* Loop until the last element in the list */
  while (Crop.LeaveProperties->next != NULL)
  {
      Crop.LeaveProperties = Crop.LeaveProperties->next; 
  }
          
  /* Add new element to the list */
  Crop.LeaveProperties->next = New;
     
  /* Restore the start address */
  Crop.LeaveProperties = LeaveProperties;
  
  return GrowthExpLAI;
}
