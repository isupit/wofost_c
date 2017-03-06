#include <stdio.h>
#include <stdlib.h>
#include "mmla.h"
#include "dynamic.h"
#include "wofost.h"

float LeaveGrowth(float LAIExp, float NewLeaves)
{
  float GrowthExpLAI;
  float GrowthSourceLimited;
  float SpecLeafArea; 
  Green *New=NULL;
  Green *LeaveProperties=NULL;
 
  SpecLeafArea = Afgen(SpecificLeaveArea, &DevelopmentStage);

 /* Leave area not to exceed exponential growth */
  if (LAIExp < 6 && NewLeaves > 0.) {
      GrowthExpLAI = LAIExp*RelIncreaseLAI*max(0.,Temp - TempBaseLeaves);
    
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
