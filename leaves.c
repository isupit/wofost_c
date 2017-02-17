#include <stdio.h>
#include <stdlib.h>
#include "dynamic.h"
#include "mmla.h"
#include "wofost.h"

float DyingLeaves()
{
  float Death, Death1, Death2, DeathStress, DeathAge;
  float CriticalLAI;
  Green *wipe;

  /* death due to water stress or high LAI */
  /* Death1 = WeightLeaves*(1. - Tra/TraMax)*MaxRelDeathRate ; */
    
  CriticalLAI = 3.2/Afgen(KDiffuseTb, &DevelopmentStage);
  Death1      = 0;
  Death2      = Crop.st.leaves * limit(0.,0.03, 0.03*(LAI-CriticalLAI)/CriticalLAI);
  Death       = max(Death1, Death2);  
  
  DeathStress = Death;

  while(Death > Crop.LeaveProperties->weight && Crop.LeaveProperties != NULL)
    { Death                = Death - Crop.LeaveProperties->weight;
      wipe                 = Crop.LeaveProperties; 
      Crop.LeaveProperties = Crop.LeaveProperties->next; 
      free(wipe);}
    
  Crop.LeaveProperties->weight = Crop.LeaveProperties->weight - Death;

  DeathAge = 0;
  while(Crop.LeaveProperties != NULL && Crop.LeaveProperties->age > LifeSpan)
     {wipe                   = Crop.LeaveProperties; 
      DeathAge               = DeathAge + Crop.LeaveProperties->weight;
      Crop.LeaveProperties   = Crop.LeaveProperties->next; 
      free(wipe);}

  return (DeathStress + DeathAge); 
}
