#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wofost.h"
#include "extern.h"
#include "penman.h"

/* ---------------------------------------------------------------------------*/
/*  function NutrientMax()                                                    */
/*  Purpose: To calculate the max nutrient concentration in the stems, leaves */
/*           and roots organs (kg N,P,K ha-1)                                 */
/* ---------------------------------------------------------------------------*/
void NutrientMax()
{           
    /* Maximum N,P,K concentration in the leaves, from which the */
    /* N,P,K concentration in the stems and roots is derived     */
    /* as a function of development stage (kg N kg-1 DM)         */
    Crop.N_st.Max_lv = Afgen(N_MaxLeaves, &DevelopmentStage);
    Crop.P_st.Max_lv = Afgen(P_MaxLeaves, &DevelopmentStage);
    Crop.K_st.Max_lv = Afgen(K_MaxLeaves, &DevelopmentStage);
      
    /* Maximum N concentrations in stems and roots (kg N kg-1) */
    Crop.N_st.Max_st = N_MaxRoots * Crop.N_st.Max_lv;
    Crop.N_st.Max_ro = N_MaxStems * Crop.N_st.Max_lv;
    
    /* Maximum P concentrations in stems and roots (kg P kg-1) */
    Crop.P_st.Max_st = P_MaxRoots * Crop.P_st.Max_lv;
    Crop.P_st.Max_ro = P_MaxStems * Crop.P_st.Max_lv;
    
    /* Maximum K concentrations in stems and roots (kg K kg-1) */
    Crop.K_st.Max_st = K_MaxRoots * Crop.K_st.Max_lv;
    Crop.K_st.Max_ro = K_MaxStems * Crop.K_st.Max_lv;
}


/* ---------------------------------------------------------------------------*/
/*  function NutrientOptimum()                                                */
/*  Purpose: To compute the optimal nutrient concentration in the crop        */
 /*  organs (kg N,P,K ha-1 )                                                  */
/* ---------------------------------------------------------------------------*/
void NutrientOptimum()
{  
    /* Optimum N,P,K amount in vegetative above-ground living biomass */
    /* and its N concentration                                        */
    Crop.N_st.Optimum_lv = Opt_N_Frac * Crop.N_st.Max_lv * Crop.st.leaves;
    Crop.N_st.Optimum_st = Opt_N_Frac * Crop.N_st.Max_st * Crop.st.stems;
        
    Crop.P_st.Optimum_lv = Opt_P_Frac * Crop.P_st.Max_lv * Crop.st.leaves;
    Crop.P_st.Optimum_st = Opt_P_Frac * Crop.P_st.Max_st * Crop.st.stems;

    Crop.K_st.Optimum_lv = Opt_K_Frac * Crop.K_st.Max_lv * Crop.st.leaves;
    Crop.K_st.Optimum_st = Opt_K_Frac * Crop.K_st.Max_st * Crop.st.stems;
}
 
/* ----------------------------------------------------------------------------*/
/*  function NutrientDemand()                                                  */
/*  Purpose: To compute the nutrient demand of crop organs (kg N,P,K ha-1 d-1) */
/* ----------------------------------------------------------------------------*/
void NutrientDemand()
{
    Crop.N_rt.Demand_lv =  max (Crop.N_st.Max_lv *Crop.st.leaves - Crop.N_st.leaves, 0.);
    Crop.N_rt.Demand_st =  max (Crop.N_st.Max_st *Crop.st.stems  - Crop.N_st.stems, 0.);
    Crop.N_rt.Demand_ro =  max (Crop.N_st.Max_ro *Crop.st.roots  - Crop.N_st.roots, 0.);
    Crop.N_rt.Demand_so =  max (Crop.N_st.Max_so *Crop.st.storage- Crop.N_st.storage, 0.)/TCNT;

    Crop.P_rt.Demand_lv =  max (Crop.P_st.Max_lv *Crop.st.leaves - Crop.P_st.leaves, 0.);
    Crop.P_rt.Demand_st =  max (Crop.P_st.Max_st *Crop.st.stems  - Crop.P_st.stems, 0.);
    Crop.P_rt.Demand_ro =  max (Crop.P_st.Max_ro *Crop.st.roots  - Crop.P_st.roots, 0.);
    Crop.P_rt.Demand_so =  max (Crop.P_st.Max_so *Crop.st.storage- Crop.P_st.storage, 0.)/TCPT;

    Crop.K_rt.Demand_lv =  max (Crop.K_st.Max_lv *Crop.st.leaves - Crop.K_st.leaves, 0.);
    Crop.K_rt.Demand_st =  max (Crop.K_st.Max_st *Crop.st.stems  - Crop.K_st.stems, 0.);
    Crop.K_rt.Demand_ro =  max (Crop.K_st.Max_ro *Crop.st.roots  - Crop.K_st.roots, 0.);
    Crop.K_rt.Demand_so =  max (Crop.K_st.Max_so *Crop.st.storage- Crop.K_st.storage, 0.)/TCKT;
}
 
               
void RateCalcultionNutrients()
{
    NutrientMax();
    
    NutrientDemand();
    
    NutrientLoss();
    
    NutrientPartioning();
    
    NutrientTranslocation();
    
    CropNutrientRates();
    
    SoilNutrientRates();
    
    /* Establish the optimum nutrient concentrations in the crop organs */
    NutrientOptimum();
    
    /* Calculate the nutrition index */
    NutritionINDX();    
    
}
