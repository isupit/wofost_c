#include <stdio.h>
#include <stdlib.h>
#include "mmla.h"
#include "dynamic.h"
#include "wofost.h"

Green *LeaveGrowth(float LAIExp, float NewLeaves, float *GrowthExpLAI)
{
  float SpecLeafArea, GrowthSourceLimited; 
  Green *New=NULL;
 
  SpecLeafArea = Afgen(SpecificLeaveArea, &DevelopmentStage);

 /* leave area not to exceed exponential growth */
  if (LAIExp < 6 && NewLeaves > 0.) {
      *GrowthExpLAI = LAIExp*RelIncreaseLAI*max(0.,Temp - TempBaseLeaves);
    
      /* source limited leaf area increase */
      GrowthSourceLimited = NewLeaves* Afgen(SpecificLeaveArea, &DevelopmentStage);
    
      /* sink-limited leaf area increase */
      SpecLeafArea = min(*GrowthExpLAI, GrowthSourceLimited)/NewLeaves;
    }
 else
    *GrowthExpLAI = 0.;
        
  New         = malloc(sizeof(Green));
  New->weight = NewLeaves;
  New->age    = 0.;
  New->area   = SpecLeafArea;
  New->next   = NULL;
     
return New;
}
