#include <stdio.h>
#include <stdlib.h>
#include "wofost.h"
#include "extern.h"

/* ---------------------------------------------------------------*/
/*  function SoilNutrientRates()                                  */
/*  Purpose: Calculation of the soil nutrient rates kg ha-1 d-1   */
/* ---------------------------------------------------------------*/     

void SoilNutrientRates()
{
    float N_fert;
    float P_fert;
    float K_fert;
    
    float day_fl;
    
    if (DevelopmentStage > 0. && DevelopmentStage <= DevelopmentStageNLimit)
    {
        SoilNtrs.rt_N_mins = min (Site.N_Mins* Site.NRecoveryFrac, SoilNtrs.st_N_tot); 
        SoilNtrs.rt_P_mins = min (Site.P_Mins* Site.PRecoveryFrac, SoilNtrs.st_P_tot); 
        SoilNtrs.rt_K_mins = min (Site.K_Mins* Site.KRecoveryFrac, SoilNtrs.st_K_tot); 
    }
    else
    {
        SoilNtrs.rt_N_mins = 0.;
        SoilNtrs.rt_P_mins = 0.;
        SoilNtrs.rt_K_mins = 0.;
    }
    
    day_fl = (float)Day;
    
    N_fert = Afgen(N_Fert_table, &day_fl) * Afgen(N_Uptake_frac, &day_fl);
    P_fert = Afgen(P_Fert_table, &day_fl) * Afgen(P_Uptake_frac, &day_fl);
    K_fert = Afgen(K_Fert_table, &day_fl) * Afgen(K_Uptake_frac, &day_fl);
    
    SoilNtrs.rt_N_tot = (N_fert / Step) - Crop.N_rt.Uptake  + SoilNtrs.rt_N_mins;
    SoilNtrs.rt_P_tot = (P_fert / Step) - Crop.P_rt.Uptake  + SoilNtrs.rt_P_mins;
    SoilNtrs.rt_K_tot = (K_fert / Step) - Crop.K_rt.Uptake  + SoilNtrs.rt_K_mins;
    printf("  SoilNtrs.rt_N_tot: %5.1f SoilNtrs.rt_P_tot: %5.1f SoilNtrs.rt_K_tot: %5.1f", SoilNtrs.rt_N_tot, SoilNtrs.rt_P_tot, SoilNtrs.rt_K_tot);
}
