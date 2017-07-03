#include <stdio.h>
#include <stdlib.h>
#include "wofost.h"
#include "extern.h"

/* --------------------------------------------------------------------*/
/*  function NutrientsInitialize()                                     */
/*  Purpose: Initialization of nutrient parameters                     */
/* --------------------------------------------------------------------*/

void InitializeNutrients()
{
    float day_fl;
    
    /* Initial maximum N concentration in plant organs per kg biomass [kg N kg-1 dry biomass]   */
    Crop.N_st.Max_lv = Afgen(N_MaxLeaves, &DevelopmentStage);
    Crop.N_st.Max_st  = N_MaxStems * Crop.N_st.Max_lv;
    Crop.N_st.Max_ro  = N_MaxRoots * Crop.N_st.Max_lv;
        
    /* Initial maximum N concentration in plant organs [kg N ]           */
    Crop.N_st.leaves = Crop.N_st.Max_lv * Crop.st.leaves;
    Crop.N_st.stems  = Crop.N_st.Max_st * Crop.st.stems;
    Crop.N_st.roots  = Crop.N_st.Max_ro * Crop.st.roots;
    Crop.N_st.storage = 0.;
       
    /* Initial maximum P concentration in plant organs per kg biomass [kg N kg-1 dry biomass]   */
    Crop.P_st.Max_lv = Afgen(P_MaxLeaves, &DevelopmentStage);
    Crop.P_st.Max_st  = P_MaxStems * Crop.P_st.Max_lv;
    Crop.P_st.Max_ro  = P_MaxRoots * Crop.P_st.Max_lv;
           
    /* Initial maximum P concentration in plant organs [kg N ] */
    Crop.P_st.leaves = Crop.P_st.Max_lv * Crop.st.leaves;
    Crop.P_st.stems  = Crop.P_st.Max_st * Crop.st.stems;
    Crop.P_st.roots  = Crop.P_st.Max_ro * Crop.st.roots;
    Crop.P_st.storage = 0.;
                  
    /* Initial maximum K concentration in plant organs per kg biomass [kg N kg-1 dry biomass]    */
    Crop.K_st.Max_lv = Afgen(K_MaxLeaves, &DevelopmentStage);
    Crop.K_st.Max_st  = K_MaxStems * Crop.K_st.Max_lv;
    Crop.K_st.Max_ro  = K_MaxRoots * Crop.K_st.Max_lv;
           
    /* Initial maximum k concentration in plant organs [kg N ] */
    Crop.K_st.leaves = Crop.K_st.Max_lv * Crop.st.leaves;
    Crop.K_st.stems  = Crop.K_st.Max_st * Crop.st.stems;
    Crop.K_st.roots  = Crop.K_st.Max_ro * Crop.st.roots;
    Crop.K_st.storage = 0.;
    
    /* No nutrient losses at initialization */
    Crop.N_st.death_lv = 0.;
    Crop.N_st.death_st = 0.;
    Crop.N_st.death_ro = 0.;
   
    Crop.P_st.death_lv = 0.;
    Crop.P_st.death_st = 0.;
    Crop.P_st.death_ro = 0.;
  
    Crop.K_st.death_lv = 0.;
    Crop.K_st.death_st = 0.;
    Crop.K_st.death_ro = 0.;
        
    /* Set the soil nutrient rates to zero */
    SoilNtrs.rt_N_tot = 0.;
    SoilNtrs.rt_P_tot = 0.;
    SoilNtrs.rt_K_tot = 0.;

    SoilNtrs.rt_N_mins = 0.;
    SoilNtrs.rt_P_mins = 0.;
    SoilNtrs.rt_K_mins = 0.;
    
    day_fl = (float)Day;
    
     /* Set the soil nutrient states */
    SoilNtrs.st_N_tot = Afgen(N_Fert_table, &day_fl) * Afgen(N_Uptake_frac, &day_fl);
    SoilNtrs.st_P_tot = Afgen(P_Fert_table, &day_fl) * Afgen(P_Uptake_frac, &day_fl);
    SoilNtrs.st_K_tot = Afgen(K_Fert_table, &day_fl) * Afgen(K_Uptake_frac, &day_fl);

    SoilNtrs.st_N_mins = Site.N_Mins;
    SoilNtrs.st_P_mins = Site.P_Mins;
    SoilNtrs.st_K_mins = Site.K_Mins;
    
    /* Set the crop nutrient rates to zero */
    Crop.N_rt.Uptake = 0.;
    Crop.P_rt.Uptake = 0.;
    Crop.K_rt.Uptake = 0.; 

    Crop.N_rt.Uptake_lv = 0.;
    Crop.N_rt.Uptake_st = 0.;
    Crop.N_rt.Uptake_ro = 0.;

    Crop.P_rt.Uptake_lv = 0.;
    Crop.P_rt.Uptake_st = 0.;
    Crop.P_rt.Uptake_ro = 0.;

    Crop.K_rt.Uptake_lv = 0.;
    Crop.K_rt.Uptake_st = 0.;
    Crop.K_rt.Uptake_ro = 0.;

    Crop.N_rt.Transloc_lv = 0.;
    Crop.N_rt.Transloc_st = 0.;
    Crop.N_rt.Transloc_ro = 0.;

    Crop.P_rt.Transloc_lv = 0.;
    Crop.P_rt.Transloc_st = 0.;
    Crop.P_rt.Transloc_ro = 0.;

    Crop.K_rt.Transloc_lv = 0.;
    Crop.K_rt.Transloc_st = 0.;
    Crop.K_rt.Transloc_ro = 0.;
     
    Crop.N_rt.death_lv = 0.;
    Crop.N_rt.death_st = 0.;
    Crop.N_rt.death_ro = 0.;
    
    Crop.P_rt.death_lv = 0.;
    Crop.P_rt.death_st = 0.;
    Crop.P_rt.death_ro = 0.;
    
    Crop.K_rt.death_lv = 0.;
    Crop.K_rt.death_st = 0.;
    Crop.K_rt.death_ro = 0.;
    
    Crop.N_rt.leaves = 0.;
    Crop.N_rt.stems  = 0.;
    Crop.N_rt.roots  = 0.;
    Crop.N_rt.storage= 0.;
    
    Crop.P_rt.leaves = 0.;
    Crop.P_rt.stems  = 0.;
    Crop.P_rt.roots  = 0.;
    Crop.P_rt.storage= 0.;
          
    Crop.K_rt.leaves = 0.;
    Crop.K_rt.stems  = 0.;
    Crop.K_rt.roots  = 0.;
    Crop.K_rt.storage= 0.;
    
    /* No nutrient stress at initialization */
    Crop.N_st.Indx = 1.;
    Crop.P_st.Indx = 1.;
    Crop.K_st.Indx = 1.;
    Crop.NPK_Indx  = 1.;
    
    /* No nutrient losses at initialization */
    Crop.N_rt.death_lv = 0.;
    Crop.N_rt.death_st = 0.;
    Crop.N_rt.death_ro = 0.;
   
    Crop.P_rt.death_lv = 0.;
    Crop.P_rt.death_st = 0.;
    Crop.P_rt.death_ro = 0.;
  
    Crop.K_rt.death_lv = 0.;
    Crop.K_rt.death_st = 0.;
    Crop.K_rt.death_ro = 0.;
    
}     
