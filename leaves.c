#include <stdio.h>
#include <stdlib.h>
#include "extern.h"
#include "wofost.h"
#include "penman.h"

float DyingLeaves()
{
    float tiny = 0.001;
    float Death, Death1, Death2, DeathStress, DeathAge;
    float CriticalLAI;
    Green *wipe;

    /* Dying rate of leaves due to water stress or high LAI */
    CriticalLAI = 3.2/Afgen(Crop->prm.KDiffuseTb, &(Crop->DevelopmentStage));
    Death1      = Crop->st.leaves *(1. - WatBal->rt.Transpiration / 
                  Evtra.MaxTranspiration) * Crop->prm.MaxRelDeathRate ;
    Death2      = Crop->st.leaves * limit(0.,0.03, 0.03*(Crop->st.LAI-CriticalLAI)/CriticalLAI);
    Death       = max(Death1, Death2);  

    /* Death rate increase due to nutrient shortage */
    Death += Crop->st.leaves * Crop->prm.DyingLeaves_NPK_Stress * (1. - Crop->NPK_Indx);

    if (Death < tiny) DeathStress = 0;
        
    DeathStress = Death;

    /* Oldest leave classes are at the beginning of the list */
    while(Death > Crop->LeaveProperties->weight && Crop->LeaveProperties != NULL)
    { 
      Death                 = Death - Crop->LeaveProperties->weight;
      wipe                  = Crop->LeaveProperties; 
      Crop->LeaveProperties = Crop->LeaveProperties->next; 
      free(wipe);
    }

    Crop->LeaveProperties->weight = Crop->LeaveProperties->weight - Death;

    DeathAge = 0;
    while(Crop->LeaveProperties != NULL && Crop->LeaveProperties->age > Crop->prm.LifeSpan)
    {
        wipe                   = Crop->LeaveProperties; 
        DeathAge              += Crop->LeaveProperties->weight;
        Crop->LeaveProperties  = Crop->LeaveProperties->next; 
        free(wipe);
    }

    return (DeathStress + DeathAge); 
}
