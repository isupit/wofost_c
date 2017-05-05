#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include "mmla.h"
#include "wofost.h"
#include "dynamic.h"
#include "extern.h"
#include "penman.h"
/* ---------------------------------------------------------------------------*/
/*  function NutrientLoss                                                     */
/*  Purpose: To calculate nutrient loss rate of dying of roots, stems leaves  */
/*           and storage organs (kg N ha-1 d-1)                               */
/* ---------------------------------------------------------------------------*/     
void NutrientLoss() 
{         
    Crop.N_rt.death_lv = N_ResidualFrac_ro  * Crop.drt.leaves;
    Crop.N_rt.death_st = N_ResidualFrac_st  * Crop.drt.stems;
    Crop.N_rt.death_ro = N_ResidualFrac_lv * Crop.drt.roots;
    
    Crop.P_rt.death_lv = P_ResidualFrac_ro  * Crop.drt.leaves;
    Crop.P_rt.death_st = P_ResidualFrac_st  * Crop.drt.stems;
    Crop.P_rt.death_ro = P_ResidualFrac_lv * Crop.drt.roots;
    
    Crop.K_rt.death_lv = K_ResidualFrac_ro  * Crop.drt.leaves;
    Crop.K_rt.death_st = K_ResidualFrac_st  * Crop.drt.stems;
    Crop.K_rt.death_ro = K_ResidualFrac_lv * Crop.drt.roots;
}     


/* ---------------------------------------------------------------------------*/
/*  function NutrientRates()                                                  */
/*  Purpose: To calculate nutrient changes (i.e. rates) in the plant          */
/*           organs (kg N,P,K ha-1 d-1)                                       */
/* ---------------------------------------------------------------------------*/
void CropNutrientRates()
{   
   Crop.N_rt.leaves  = Crop.N_rt.Uptake_lv - Crop.N_rt.Transloc_lv - Crop.N_rt.death_lv;
   Crop.N_rt.stems   = Crop.N_rt.Uptake_st - Crop.N_rt.Transloc_st - Crop.N_rt.death_st;
   Crop.N_rt.roots   = Crop.N_rt.Uptake_ro - Crop.N_rt.Transloc_ro - Crop.N_rt.death_ro;
   
   Crop.P_rt.leaves  = Crop.P_rt.Uptake_lv - Crop.P_rt.Transloc_lv - Crop.P_rt.death_lv;
   Crop.P_rt.stems   = Crop.P_rt.Uptake_st - Crop.P_rt.Transloc_st - Crop.P_rt.death_st;
   Crop.P_rt.roots   = Crop.P_rt.Uptake_ro - Crop.P_rt.Transloc_ro - Crop.P_rt.death_ro;
   
   Crop.K_rt.leaves  = Crop.K_rt.Uptake_lv - Crop.K_rt.Transloc_lv - Crop.K_rt.death_lv;
   Crop.K_rt.stems   = Crop.K_rt.Uptake_st - Crop.K_rt.Transloc_st - Crop.K_rt.death_st;
   Crop.K_rt.roots   = Crop.K_rt.Uptake_ro - Crop.K_rt.Transloc_ro - Crop.K_rt.death_ro;
   
   
    /* Rate of N,P,K uptake in storage organs (kg N,P,K ha-1 d-1) */
    if (DevelopmentStage <  DevelopmentStageNT)
    {
        Crop.N_rt.storage = min(Crop.N_rt.Demand_so, Crop.N_rt.Transloc/TCNT);
        Crop.P_rt.storage = min(Crop.P_rt.Demand_so, Crop.P_rt.Transloc/TCPT);
        Crop.K_rt.storage = min(Crop.K_rt.Demand_so, Crop.K_rt.Transloc/TCKT); 
    }
    else
    {
        Crop.N_rt.storage = 0;
        Crop.P_rt.storage = 0;
        Crop.K_rt.storage = 0;  
    }
}


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
    Crop.K_st.Max_st = K_MaxRoots* Crop.K_st.Max_lv;
    Crop.K_st.Max_ro = K_MaxStems* Crop.K_st.Max_lv;
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
 
      
      
/* ---------------------------------------------------------------------------*/
/*  function Translocation()                                                  */ 
/*  Purpose: To compute the amount of nutrients in the organs that can        */
/*  be translocated (kg N ha-1)                                               */
/* ---------------------------------------------------------------------------*/
void Translocation()                                                                                                       
{
     float Avail_N_lv, Avail_P_lv, Avail_K_lv;
     float Avail_N_st, Avail_P_st, Avail_K_st;                                                                                                               
     float Avail_N_rt, Avail_P_rt, Avail_K_rt; 
                                                                                                                      
    /* N amount available for translocation */                                                                                                              
    Avail_N_lv = max(0.,Crop.N_st.leaves - Crop.st.leaves * N_ResidualFrac_lv);
    Avail_N_st = max(0.,Crop.N_st.stems  - Crop.st.stems  * N_ResidualFrac_st);
    Avail_N_rt = max(0.,min((Crop.N_rt.Transloc_lv + Crop.N_rt.Transloc_st) * FracTranslocRoots, 
            Crop.N_st.roots - Crop.st.roots * N_ResidualFrac_ro));
   
    /* P amount available for translocation */ 
    Avail_P_lv = max (0.,Crop.P_st.leaves - Crop.st.leaves * P_ResidualFrac_lv);
    Avail_P_st = max (0.,Crop.P_st.stems  - Crop.st.stems  * P_ResidualFrac_st);
    Avail_P_rt = max (0.,min((Crop.P_rt.Transloc_lv + Crop.P_rt.Transloc_st) * FracTranslocRoots, 
            Crop.P_st.roots - Crop.st.roots * P_ResidualFrac_ro));
    
    /* K amount available for translocation */ 
    Avail_K_lv = max(0.,Crop.K_st.leaves - Crop.st.leaves * K_ResidualFrac_lv);
    Avail_K_st = max(0.,Crop.K_st.stems  - Crop.st.stems  * K_ResidualFrac_st);
    Avail_K_rt = max(0.,min((Crop.K_rt.Transloc_lv + Crop.K_rt.Transloc_st) * FracTranslocRoots, 
            Crop.K_st.roots - Crop.st.roots * P_ResidualFrac_ro));

    /* Total available nutrient amount for translocation */
    Crop.N_rt.Transloc = Avail_N_lv + Avail_N_st + Avail_N_rt;
    Crop.P_rt.Transloc = Avail_P_lv + Avail_P_st + Avail_P_rt;
    Crop.K_rt.Transloc = Avail_K_lv + Avail_K_st + Avail_K_rt;
    
    /* Actual N translocation rate */ 
    if (Crop.N_rt.Transloc > 0.001)
    {
        Crop.N_rt.Transloc_lv = Crop.N_rt.storage * Avail_N_lv / Crop.N_rt.Transloc;
        Crop.N_rt.Transloc_st = Crop.N_rt.storage * Avail_N_st / Crop.N_rt.Transloc;
        Crop.K_rt.Transloc_ro = Crop.N_rt.storage * Avail_N_rt / Crop.N_rt.Transloc;  
    }
    else
    {
        Crop.N_rt.Transloc_lv = 0.;       
        Crop.N_rt.Transloc_st = 0.;
        Crop.K_rt.Transloc_ro = 0.;
    }
    /* Actual P translocation rate */
    if (Crop.P_rt.Transloc > 0.001)
    {
        Crop.P_rt.Transloc_lv = Crop.P_rt.storage * Avail_P_lv / Crop.P_rt.Transloc;
        Crop.P_rt.Transloc_st = Crop.P_rt.storage * Avail_P_st / Crop.P_rt.Transloc;
        Crop.P_rt.Transloc_ro = Crop.P_rt.storage * Avail_P_rt / Crop.P_rt.Transloc;  
    }
    else
    {
        Crop.P_rt.Transloc_lv = 0.;       
        Crop.P_rt.Transloc_st = 0.;
        Crop.P_rt.Transloc_ro = 0.;
    }
    
    /* Actual K translocation rate */
    if (Crop.K_rt.Transloc > 0.001)
    {
        Crop.K_rt.Transloc_lv = Crop.K_rt.storage * Avail_K_lv / Crop.K_rt.Transloc;
        Crop.K_rt.Transloc_st = Crop.K_rt.storage * Avail_K_st / Crop.K_rt.Transloc;
        Crop.K_rt.Transloc_ro = Crop.K_rt.storage * Avail_K_rt / Crop.K_rt.Transloc;  
    }
    else
    {
        Crop.K_rt.Transloc_lv = 0.;       
        Crop.K_rt.Transloc_st = 0.;
        Crop.K_rt.Transloc_ro = 0.;
    }
}     

                                                                                                                                                                                                                                                                                                                                    
                                                                                                                  
/* -------------------------------------------------------------------------*/
/*  function NutrientPartioning()                                           */
/*  Purpose: To compute the partitioning of the total N/P/K uptake rate     */
/*           (N,P,K UPTR) over leaves, stem, and roots (kg N/P/K ha-1 d-1)  */
/* -------------------------------------------------------------------------*/
void NutrientPartioning()
{     
    float Total_N_demand;
    float Total_P_demand;
    float Total_K_demand;
    
    float NutrientLimit;
    float N_Fix_rt;
    
    Total_N_demand = Crop.N_rt.Demand_lv + Crop.N_rt.Demand_st + Crop.N_rt.Demand_ro;
    Total_P_demand = Crop.P_rt.Demand_lv + Crop.P_rt.Demand_st + Crop.P_rt.Demand_ro;
    Total_K_demand = Crop.K_rt.Demand_lv + Crop.K_rt.Demand_st + Crop.K_rt.Demand_ro;
    
    NutrientLimit = insw(DevelopmentStage - DevelopmentStageNLimit , 
            insw(WatBal.rt.Transpiration/Evtra.MaxTranspiration -0.01,0.,1.) , 0.0);
    
    /* Nutrient uptake cannot be larger than the availability and is larger or equal to zero */
    Crop.N_rt.Uptake = (max(0., min((1.-N_fixation)*Total_N_demand, SoilNtrs.st_N_tot))* NutrientLimit)/Step;
    Crop.P_rt.Uptake = (max(0., min(Total_P_demand, SoilNtrs.st_P_mins))* NutrientLimit)/Step;
    Crop.K_rt.Uptake = (max(0., min(Total_K_demand, SoilNtrs.st_K_mins))* NutrientLimit)/Step;
    
    N_Fix_rt= max(0.,Crop.N_rt.Uptake * N_fixation / max(0.02, 1.-N_fixation));
   
    /* N uptake per crop organ kg ha-1 d-1*/
    if (Total_N_demand > 0.001)
    {
        Crop.N_rt.Uptake_lv = (Crop.N_rt.Demand_lv / Total_N_demand)* (Crop.N_rt.Uptake + N_Fix_rt);
        Crop.N_rt.Uptake_st = (Crop.N_rt.Demand_st / Total_N_demand)* (Crop.N_rt.Uptake + N_Fix_rt);
        Crop.N_rt.Uptake_ro = (Crop.N_rt.Demand_ro / Total_N_demand)* (Crop.N_rt.Uptake + N_Fix_rt);
    }
    else
    {
        Crop.N_rt.Uptake_lv = 0.;
        Crop.N_rt.Uptake_st = 0.;
        Crop.N_rt.Uptake_ro = 0.;
    }
    
    /* P uptake per crop organ kg ha-1 d-1 */
    if (Total_P_demand > 0.001)
    {
        Crop.P_rt.Uptake_lv = (Crop.P_rt.Demand_lv / Total_P_demand)* Crop.P_rt.Uptake;
        Crop.P_rt.Uptake_st = (Crop.P_rt.Demand_st / Total_P_demand)* Crop.P_rt.Uptake;
        Crop.P_rt.Uptake_ro = (Crop.P_rt.Demand_ro / Total_P_demand)* Crop.P_rt.Uptake; 
    }
    else
    {
        Crop.P_rt.Uptake_lv = 0.;      
        Crop.P_rt.Uptake_st = 0.;      
        Crop.P_rt.Uptake_ro = 0.;      
    }
    
   /* K uptake per crop organ kg ha-1 d-1*/
    if (Total_K_demand > 0.001)
    {
        Crop.K_rt.Uptake_lv = (Crop.K_rt.Demand_lv / Total_K_demand)* Crop.K_rt.Uptake;
        Crop.K_rt.Uptake_st = (Crop.K_rt.Demand_st / Total_K_demand)* Crop.K_rt.Uptake;
        Crop.K_rt.Uptake_ro = (Crop.K_rt.Demand_ro / Total_K_demand)* Crop.K_rt.Uptake;   
    }
    else
    {
        Crop.K_rt.Uptake_lv = 0.;
        Crop.K_rt.Uptake_st = 0.;
        Crop.K_rt.Uptake_ro = 0.;        
    }
}    

void SoilNutrientRates()
{
    float N_fert;
    float P_fert;
    float K_fert;
    
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
    
    N_fert = Afgen(N_Fert_table, &Day) * Afgen(N_Uptake_frac, &Day);
    P_fert = Afgen(P_Fert_table, &Day) * Afgen(P_Uptake_frac, &Day);
    K_fert = Afgen(K_Fert_table, &Day) * Afgen(K_Uptake_frac, &Day);
    
    SoilNtrs.rt_N_tot = N_fert / Step + Crop.N_rt.Uptake  + SoilNtrs.rt_N_mins;
    SoilNtrs.rt_P_tot = P_fert / Step + Crop.P_rt.Uptake  + SoilNtrs.rt_P_mins;
    SoilNtrs.rt_K_tot = K_fert / Step + Crop.K_rt.Uptake  + SoilNtrs.rt_K_mins;
}



/* --------------------------------------------------------------------*/
/*  function NutritionINDX()                                           */
/*  Purpose: To compute N,P,K Nutrition Index (-)                      */
/* --------------------------------------------------------------------*/
void NutritionINDX()
{    
    float VegetativeMass;
    
    float N_opt_veg;
    float P_opt_veg;
    float K_opt_veg;
    
    float N_Veg;
    float P_Veg;
    float K_Veg;
    
    float N_res;
    float P_res;
    float K_res;
 
    /* Total vegetative living above-ground biomass (kg DM ha-1)     */
    VegetativeMass = Crop.st.leaves + Crop.st.stems;
    
    
    if (VegetativeMass > 0.)
    {
        /* N,P,K concentration in total vegetative living per */
        /* kg above-ground biomass  (kg N,P,K kg-1 DM)        */
        N_Veg  = (Crop.N_st.leaves + Crop.N_st.stems)/VegetativeMass;
        P_Veg  = (Crop.P_st.leaves + Crop.P_st.stems)/VegetativeMass;
        K_Veg  = (Crop.K_st.leaves + Crop.K_st.stems)/VegetativeMass;

        /* Residual N,P,K concentration in total vegetative living */
        /* above-ground biomass  (kg N,P,K kg-1 DM)                */
        N_res = (Crop.st.leaves * N_ResidualFrac_lv +Crop.st.stems*N_ResidualFrac_st)/VegetativeMass;
        P_res = (Crop.st.leaves * P_ResidualFrac_lv +Crop.st.stems*P_ResidualFrac_st)/VegetativeMass;
        K_res = (Crop.st.leaves * K_ResidualFrac_lv +Crop.st.stems*K_ResidualFrac_st)/VegetativeMass;

        N_opt_veg = (Crop.N_st.Optimum_lv + Crop.N_st.Optimum_st)/VegetativeMass;
        P_opt_veg = (Crop.P_st.Optimum_lv + Crop.P_st.Optimum_st)/VegetativeMass;
        K_opt_veg = (Crop.K_st.Optimum_lv + Crop.K_st.Optimum_st)/VegetativeMass;
    }
    
    
    float tiny=0.001;
    Crop.N_st.Indx = limit(tiny,1.0, ((N_Veg -N_res)/notnul(N_opt_veg - N_res)));
    Crop.P_st.Indx = limit(tiny,1.0, ((P_Veg -P_res)/notnul(P_opt_veg - P_res)));
    Crop.K_st.Indx = limit(tiny,1.0, ((K_Veg -K_res)/notnul(K_opt_veg - K_res)));
    
    Crop.NPK_Indx = (Crop.N_st.Indx < Crop.P_st.Indx) ? Crop.N_st.Indx : Crop.P_st.Indx;
    Crop.NPK_Indx = (Crop.NPK_Indx < Crop.K_st.Indx) ? Crop.NPK_Indx : Crop.K_st.Indx;
    
    /* Nutrient reduction factor */
    Crop.NutrientStress = limit(0., 1.0, 1.-NLUE*pow((1.0001-Crop.NPK_Indx),2));
    
}

   
/* --------------------------------------------------------------------*/
/*  function NutrientsInitialize()                                            */
/*  Purpose: Initialization of nutrient parameters                     */
/* --------------------------------------------------------------------*/
void InitializeNutrients()
{
    /* Initial maximum N concentration in plant organs per kg biomass [kg N kg-1 dry biomass]   */
    Crop.N_st.Max_lv = Afgen(N_MaxLeaves, &DevelopmentStage);
    Crop.N_st.Max_st  = N_MaxStems * Crop.N_st.Max_lv;
    Crop.N_st.Max_ro  = N_MaxRoots * Crop.N_st.Max_lv;
        
    /* Initial maximum N concentration in plant organs [kg N ]           */
    Crop.N_st.leaves = Crop.N_st.Max_lv * Crop.st.leaves;
    Crop.N_st.stems  = Crop.N_st.Max_st  * Crop.st.stems;
    Crop.N_st.roots  = Crop.N_st.Max_ro  * Crop.st.roots;
    Crop.N_st.storage = 0.;
       
    /* Initial maximum P concentration in plant organs per kg biomass [kg N kg-1 dry biomass]   */
    Crop.P_st.Max_lv = Afgen(P_MaxLeaves, &DevelopmentStage);
    Crop.P_st.Max_st  = P_MaxStems * Crop.P_st.Max_lv;
    Crop.P_st.Max_ro  = P_MaxRoots * Crop.P_st.Max_lv;
           
    /* Initial maximum P concentration in plant organs [kg N ] */
    Crop.P_st.leaves = Crop.P_st.Max_lv * Crop.st.leaves;
    Crop.P_st.stems  = Crop.P_st.Max_st  * Crop.st.stems;
    Crop.P_st.roots  = Crop.P_st.Max_ro  * Crop.st.roots;
    Crop.P_st.storage = 0.;
                  
    /* Initial maximum K concentration in plant organs per kg biomass [kg N kg-1 dry biomass]    */
    Crop.K_st.Max_lv = Afgen(K_MaxLeaves, &DevelopmentStage);
    Crop.K_st.Max_st  = K_MaxStems * Crop.K_st.Max_lv;
    Crop.K_st.Max_ro  = K_MaxRoots * Crop.K_st.Max_lv;
           
    /* Initial maximum k concentration in plant organs [kg N ] */
    Crop.K_st.leaves = Crop.K_st.Max_lv * Crop.st.leaves;
    Crop.K_st.stems  = Crop.K_st.Max_st  * Crop.st.stems;
    Crop.K_st.roots  = Crop.K_st.Max_ro  * Crop.st.roots;
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
    
     /* Set the soil nutrient states */
    SoilNtrs.st_N_tot = Afgen(N_Fert_table, &Day) * Afgen(N_Uptake_frac, &Day);
    SoilNtrs.st_P_tot = Afgen(P_Fert_table, &Day) * Afgen(P_Uptake_frac, &Day);
    SoilNtrs.st_K_tot = Afgen(K_Fert_table, &Day) * Afgen(K_Uptake_frac, &Day);

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
      
/* ---------------------------------------------------------------*/
/*  function NutrientSowing()                                     */
/*  Purpose: Initialization of nutrient parameters at sowing      */
/* ---------------------------------------------------------------*/      
void IntegrationNutrients()
{   
    /* Integration of the total of soil N,P,K */
    SoilNtrs.st_N_tot =+ SoilNtrs.rt_N_tot;
    SoilNtrs.st_P_tot =+ SoilNtrs.rt_P_tot;
    SoilNtrs.st_K_tot =+ SoilNtrs.rt_K_tot;
    
    /* Integration of the total N,P,K soil mineralization */
    SoilNtrs.st_N_mins =+ SoilNtrs.rt_N_mins;
    SoilNtrs.st_P_mins =+ SoilNtrs.rt_P_mins;
    SoilNtrs.st_K_mins =+ SoilNtrs.rt_K_mins;
        
    /* Crops nutrients */
    Crop.N_st.leaves  =+ Crop.N_rt.leaves ;
    Crop.N_st.stems   =+ Crop.N_rt.stems  ;
    Crop.N_st.roots   =+ Crop.N_rt.roots  ;
    Crop.N_st.storage =+ Crop.N_rt.storage;
          
    Crop.P_st.leaves  =+ Crop.P_rt.leaves ;
    Crop.P_st.stems   =+ Crop.P_rt.stems  ;
    Crop.P_st.roots   =+ Crop.P_rt.roots  ;
    Crop.P_st.storage =+ Crop.P_rt.storage;
   
    Crop.K_st.leaves  =+ Crop.K_rt.leaves ;
    Crop.K_st.stems   =+ Crop.K_rt.stems  ;
    Crop.K_st.roots   =+ Crop.K_rt.roots  ;
    Crop.K_st.storage =+ Crop.K_rt.storage;
    
    /* Nutrient losses due to dying plant parts */
    Crop.N_st.death_lv =+ Crop.N_rt.death_lv;
    Crop.N_st.death_st =+ Crop.N_rt.death_st;
    Crop.N_st.death_ro =+ Crop.N_rt.death_ro;
  
    Crop.P_st.death_lv =+ Crop.P_rt.death_lv;
    Crop.P_st.death_st =+ Crop.P_rt.death_st;
    Crop.P_st.death_ro =+ Crop.P_rt.death_ro;
 
    Crop.K_st.death_lv =+ Crop.K_rt.death_lv;
    Crop.K_st.death_st =+ Crop.K_rt.death_st;
    Crop.K_st.death_ro =+ Crop.K_rt.death_ro;
    
}


void  NutrientStress()
{
    /* Establish the optimum nutrient concentrations in the crop organs */
    NutrientOptimum();
    
    /* Calculate the nutrition index */
    NutritionINDX();
    
}

void RateCalcultionNutrients()
{
    NutrientMax();
    
    NutrientDemand();
    
    NutrientLoss();
    
    NutrientPartioning();
    
    Translocation();
    
    CropNutrientRates();
    
    SoilNutrientRates();
}
