#include <stdio.h>
#include <stdlib.h>
#include "wofost.h"
#include "extern.h"


/* ---------------------------------------------------------------------------*/
/*  function NutrientRates()                                                  */
/*  Purpose: To calculate nutrient changes (i.e. rates) in the plant          */
/*           organs (kg N,P,K ha-1 d-1)                                       */
/* ---------------------------------------------------------------------------*/

void CropNutrientRates()
{   
    float Avail_N;
    float Avail_P;
    float Avail_K;
    
    Crop->N_rt.leaves  = Crop->N_rt.Uptake_lv - Crop->N_rt.Transloc_lv - Crop->N_rt.death_lv;
    Crop->N_rt.stems   = Crop->N_rt.Uptake_st - Crop->N_rt.Transloc_st - Crop->N_rt.death_st;
    Crop->N_rt.roots   = Crop->N_rt.Uptake_ro - Crop->N_rt.Transloc_ro - Crop->N_rt.death_ro;

    Crop->P_rt.leaves  = Crop->P_rt.Uptake_lv - Crop->P_rt.Transloc_lv - Crop->P_rt.death_lv;
    Crop->P_rt.stems   = Crop->P_rt.Uptake_st - Crop->P_rt.Transloc_st - Crop->P_rt.death_st;
    Crop->P_rt.roots   = Crop->P_rt.Uptake_ro - Crop->P_rt.Transloc_ro - Crop->P_rt.death_ro;

    //printf("%4d %8.6f %8.6f\n",Crop->GrowthDay,Crop->P_rt.Uptake_lv,Crop->P_rt.death_lv);
    
    Crop->K_rt.leaves  = Crop->K_rt.Uptake_lv - Crop->K_rt.Transloc_lv - Crop->K_rt.death_lv;
    Crop->K_rt.stems   = Crop->K_rt.Uptake_st - Crop->K_rt.Transloc_st - Crop->K_rt.death_st;
    Crop->K_rt.roots   = Crop->K_rt.Uptake_ro - Crop->K_rt.Transloc_ro - Crop->K_rt.death_ro;
   
   
    /* Rate of N,P,K uptake in storage organs (kg N,P,K ha-1 d-1) */
    if (Crop->st.Development >  Crop->prm.DevelopmentStageNT)
    {
        Crop->N_rt.storage = min(Crop->N_rt.Demand_so, Crop->N_rt.Supply);
        Crop->P_rt.storage = min(Crop->P_rt.Demand_so, Crop->P_rt.Supply);
        Crop->K_rt.storage = min(Crop->K_rt.Demand_so, Crop->K_rt.Supply); 
    }
    
        /* Total available nutrients for translocation */
    Avail_N = Crop->N_st.Avail_lv + Crop->N_st.Avail_st + Crop->N_st.Avail_ro;
    Avail_P = Crop->P_st.Avail_lv + Crop->P_st.Avail_st + Crop->P_st.Avail_ro;
    Avail_K = Crop->K_st.Avail_lv + Crop->K_st.Avail_st + Crop->K_st.Avail_ro;
    
    
    /* Actual N translocation rate partitioned over the organs */ 
    if ( Avail_N > 0.001)
    {  
        Crop->N_rt.Transloc_lv = Crop->N_rt.storage * Crop->N_st.Avail_lv / Avail_N;
        Crop->N_rt.Transloc_st = Crop->N_rt.storage * Crop->N_st.Avail_st / Avail_N;
        Crop->N_rt.Transloc_ro = Crop->N_rt.storage * Crop->N_st.Avail_ro / Avail_N;
    }
    else
    {
        Crop->N_rt.Transloc_lv = 0.;
        Crop->N_rt.Transloc_st = 0.;
        Crop->N_rt.Transloc_ro = 0.;
    }
    
    /* Actual P translocation rate partitioned over the organs */ 
    if ( Avail_P > 0.001)
    {
        Crop->P_rt.Transloc_lv = Crop->P_rt.storage * Crop->P_st.Avail_lv / Avail_P;
        Crop->P_rt.Transloc_st = Crop->P_rt.storage * Crop->P_st.Avail_st / Avail_P;
        Crop->P_rt.Transloc_ro = Crop->P_rt.storage * Crop->P_st.Avail_ro / Avail_P;
    }
    else
    {
        Crop->P_rt.Transloc_lv = 0.;
        Crop->P_rt.Transloc_st = 0.;
        Crop->P_rt.Transloc_ro = 0.;
    }
    
        /* Actual P translocation rate partitioned over the organs */ 
    if ( Avail_K > 0.001)
    {
        Crop->K_rt.Transloc_lv = Crop->K_rt.storage * Crop->K_st.Avail_lv / Avail_K;
        Crop->K_rt.Transloc_st = Crop->K_rt.storage * Crop->K_st.Avail_st / Avail_K;
        Crop->K_rt.Transloc_ro = Crop->K_rt.storage * Crop->K_st.Avail_ro / Avail_K;
    }
    else
    {
        Crop->K_rt.Transloc_lv = 0.;
        Crop->K_rt.Transloc_st = 0.;
        Crop->K_rt.Transloc_ro = 0.;
    }
    
    //printf("  Crop->N_rt.storage: %10.4f Crop->P_rt.storage: %10.4f Crop->K_rt.storage: %10.4f\n", Crop->N_st.storage, Crop->P_st.storage, Crop->K_st.storage);
    //printf("  Crop->N_st.leaves: %5.1f Crop->P_st.leaves: %5.1f Crop->K_st.leaves: %5.1f\n", Crop->N_st.leaves, Crop->P_st.leaves, Crop->K_st.leaves);
    //printf("%7.4f %7.4f\n",Crop->N_rt.Demand_so, Crop->N_rt.Transloc);
}
