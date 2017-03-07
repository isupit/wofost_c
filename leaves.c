#include <stdio.h>
#include <stdlib.h>
#include "dynamic.h"
#include "mmla.h"
#include "wofost.h"
#include "penman.h"

float DyingLeaves()
{
  float Death, Death1, Death2, DeathStress, DeathAge;
  float CriticalLAI;
  Green *wipe;

  /* Dying rate of leaves due to water stress or high LAI */
    
  CriticalLAI = 3.2/Afgen(KDiffuseTb, &DevelopmentStage);
  Death1      = Crop.st.leaves *(1. - WatBal.rt.Transpiration / 
                Evtra.MaxTranspiration)*MaxRelDeathRate ;
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
