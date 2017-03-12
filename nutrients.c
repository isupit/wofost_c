#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dynamic.h"
#include "wofost.h"

/* ---------------------------------------------------------------------------*/
/*  NutrientLoss                                                              */
/*  Purpose: To calculate nutrient loss rate of dying of roots, stems leaves  */
/*           and storage organs (kg N ha-1 d-1)                               */
/* ---------------------------------------------------------------------------*/     
void NutrientLoss() 
{         
    RNLDRT = N_ResidualFracRoots * Crop.drt.roots;
    RNLDST = N_ResidualFracStems * Crop.drt.stems;
    RNLDLV = N_ResidualFracLeaves* Crop.drt.leaves;
    
    RPLDRT = P_ResidualFracRoots * Crop.drt.roots;
    RPLDST = P_ResidualFracStems * Crop.drt.stems;
    RPLDLV = P_ResidualFracLeaves* Crop.drt.leaves;
    
    RKLDRT = K_ResidualFracRoots * Crop.drt.roots;
    RKLDST = K_ResidualFracStems * Crop.drt.stems;
    RKLDLV = K_ResidualFracLeaves* Crop.drt.leaves;
}      


void NutrientMax()
{
    float VegetativeMass;
    
    /* Total vegetative living above-ground biomass (kg DM ha-1)     */
    float VegetativeMass = Crop.st.leaves + Crop.st.stems;  
        
    /* Maximum N/P/K concentration in the leaves, from which the N/P/K conc. in the   */
    /* stems and roots are derived, as a function of development stage (kg N kg-1 DM) */
    
    Crop.N_st.Max_leaves = Afgen(N_MaxLeaves, &DevelopmentStage);
    Crop.P_st.Max_leaves = Afgen(P_MaxLeaves, &DevelopmentStage);
    Crop.K_st.Max_leaves = Afgen(K_MaxLeaves, &DevelopmentStage);
      
    /* Maximum N/P/K concentrations in stems and roots (kg powfN kg-1 DM) */
    Crop.N_st.Max_stems = LSNR * Crop.N_st.Max_leaves;
    Crop.N_st.Max_roots = LRNR * Crop.N_st.Max_leaves;
    
    Crop.P_st.Max_stems = LSPR * Crop.P_st.Max_leaves;
    Crop.P_st.Max_roots = LRPR * Crop.P_st.Max_leaves;
    
    Crop.K_st.Max_stems = LSKR * Crop.K_st.Max_leaves;
    Crop.K_st.Max_roots = LRKR * Crop.K_st.Max_leaves;
}

void NutrientOptimum()
{  
    /* Optimal N/P/K amount in vegetative above-ground living biomass */
    /* and its N concentration                                        */
    Crop.N_st.Opt_leaves = FRNX * Crop.N_st.Max_leaves * Crop.st.leaves;
    Crop.N_st.Opt_stems  = FRNX * Crop.N_st.Max_stems  * Crop.st.stems;
        
    Crop.P_st.Opt_leaves = FRPX * Crop.P_st.Max_leaves * Crop.st.leaves;
    Crop.P_st.Opt_stems  = FRPX * Crop.P_st.Max_stems  * Crop.st.stems;

    Crop.K_st.Opt_leaves = FRKX * Crop.K_st.Max_leaves * Crop.st.leaves;
    Crop.K_st.Opt_stems  = FRKX * Crop.K_st.Max_stems  * Crop.st.stems;
}
 
/* -------------------------------------------------------------------------*/
/*  function NutrientDemand()                                                       */
/*  Purpose: To compute the nutrient demand of crop organs (kg N/P/K ha-1 d-1)  */
/* -------------------------------------------------------------------------*/

void NutrientDemand()
{
    Crop.N_rt.Dmnd_leaves  =  max (Crop.N_st.Max_leaves *Crop.st.leaves - Crop.N_st.leaves, 0.);
    Crop.N_rt.Dmnd_stems   =  max (Crop.N_st.Max_stems  *Crop.st.stems  - Crop.N_st.stems, 0.);
    Crop.N_rt.Dmnd_roots   =  max (Crop.N_st.Max_roots  *Crop.st.roots  - Crop.N_st.roots, 0.);
    Crop.N_rt.Dmnd_storage =  max (Crop.N_st.Max_storage*Crop.st.storage- Crop.N_st.storage, 0.)/TCNT;

    Crop.P_rt.Dmnd_leaves =  max (Crop.P_st.Max_leaves *Crop.st.leaves - Crop.P_st.leaves, 0.);
    Crop.P_rt.Dmnd_stems  =  max (Crop.P_st.Max_stems  *Crop.st.stems  - Crop.P_st.stems, 0.);
    Crop.P_rt.Dmnd_roots  =  max (Crop.P_st.Max_roots  *Crop.st.roots  - Crop.P_st.roots, 0.);
    Crop.P_rt.Dmnd_storage=  max (Crop.P_st.Max_storage*Crop.st.storage- Crop.P_st.storage, 0.)/TCPT;

    Crop.K_rt.Dmnd_leaves =  max (Crop.K_st.Max_leaves *Crop.st.leaves - Crop.K_st.leaves, 0.);
    Crop.K_rt.Dmnd_stems  =  max (Crop.K_st.Max_stems  *Crop.st.stems  - Crop.K_st.stems, 0.);
    Crop.K_rt.Dmnd_roots  =  max (Crop.K_st.Max_roots  *Crop.st.roots  - Crop.K_st.roots, 0.);
    Crop.K_rt.Dmnd_storage=  max (Crop.K_st.Max_storage*Crop.st.storage- Crop.K_st.storage, 0.)/TCKT;
}
 
      
      
/* ---------------------------------------------------------------------------*/
/*  function AvailableTranslocation                                           */                                  */
/*  Purpose: To compute the amount of nutrients in the organs that can        */
/*  be translocated (kg N ha-1)                                                */
/* ---------------------------------------------------------------------------*/

void Translocation()
{
    float N_TranslocLeaves, P_TranslocLeaves, K_TranslocLeaves;
    float N_TranslocStems,  P_TranslocStems,  K_TranslocStems;
    float N_TranslocRoots,  P_TranslocRoots,  K_TranslocRoots;
    float N_AvailTransloc,  P_AvailTransloc,  K_AvailTransloc;
    
    /* N amount available for translocation */                                                                                                              
    N_TranslocLeaves = max(0.,Crop.N_st.leaves - Crop.st.leaves * N_ResidualFracLeaves);
    N_TranslocStems  = max(0.,Crop.N_st.stems  - Crop.st.stems  * N_ResidualFracStems);
    N_TranslocRoots  = max(0.,min((N_TranslocLeaves + N_TranslocStems) * FracTranslocRoots, 
            Crop.N_st.roots - Crop.st.roots * N_ResidualFracRoots));
    N_AvailTransloc = N_TranslocLeaves + N_TranslocStems + N_TranslocRoots;
    
    
    /* P amount available for translocation */ 
    P_TranslocLeaves = max (0.,Crop.P_st.leaves - Crop.st.leaves * P_ResidualFracLeaves);
    P_TranslocStems  = max (0.,Crop.P_st.stems  - Crop.st.stems  * P_ResidualFracStems);
    P_TranslocRoots  = max (0.,min((P_TranslocLeaves + P_TranslocStems) * FracTranslocRoots, 
            Crop.P_st.roots - Crop.st.roots * P_ResidualFracRoots));
    P_AvailTransloc  = P_TranslocLeaves + P_TranslocStems + P_TranslocRoots;
    
    
    /* K amount available for translocation */ 
    K_TranslocLeaves = max(0.,Crop.K_st.leaves - Crop.st.leaves * K_ResidualFracLeaves);
    K_TranslocStems  = max(0.,Crop.K_st.stems  - Crop.st.stems  * K_ResidualFracStems);
    K_TranslocRoots  = max(0.,min((K_TranslocLeaves + K_TranslocStems) * FracTranslocRoots, 
            Crop.K_st.roots - Crop.st.roots * P_ResidualFracRoots));
    K_AvailTransloc  = K_TranslocLeaves + K_TranslocStems + K_TranslocRoots;

    
    RNTLV= Crop.N_rt.storage * N_TranslocLeaves/ notnul(N_AvailTransloc);
    RNTST= Crop.N_rt.storage * N_TranslocStems / notnul(N_AvailTransloc);
    RNTRT= Crop.N_rt.storage * N_TranslocRoots / notnul(N_AvailTransloc);
    
    RPTLV= Crop.P_rt.storage * P_TranslocLeaves/ notnul(P_AvailTransloc);
    RPTST= Crop.P_rt.storage * P_TranslocStems / notnul(P_AvailTransloc);
    RPTRT= Crop.P_rt.storage * P_TranslocRoots / notnul(P_AvailTransloc);

    RKTLV= Crop.K_rt.storage * K_TranslocLeaves/ notnul(K_AvailTransloc);
    RKTST= Crop.K_rt.storage * K_TranslocStems / notnul(K_AvailTransloc);
    RKTRT= Crop.K_rt.storage * K_TranslocRoots / notnul(K_AvailTransloc);
}     

/* -------------------------------------------------------------------------*/
/*  function NutrientPartioning()                                                         */
/*  Purpose: To compute the partitioning of the total N/P/K uptake rate     */
/*           (N/P/K UPTR) over leaves, stem, and roots (kg N/P/K ha-1 d-1)  */
/* -------------------------------------------------------------------------*/

void NutrientPartioning()
{     
      RNULV = (Crop.N_rt.Dmnd_leaves / notnul(NDEMTO))* (NUPTR+NFIXTR);
      RNUST = (Crop.N_rt.Dmnd_stems  / notnul(NDEMTO))* (NUPTR+NFIXTR);
      RNURT = (Crop.N_rt.Dmnd_roots  / notnul(NDEMTO))* (NUPTR+NFIXTR);
    
      RPULV = (Crop.P_rt.Dmnd_leaves / notnul(PDEMTO))* PUPTR;
      RPUST = (Crop.P_rt.Dmnd_stems  / notnul(PDEMTO))* PUPTR;
      RPURT = (Crop.P_rt.Dmnd_roots  / notnul(PDEMTO))* PUPTR;

      RKULV = (Crop.K_rt.Dmnd_leaves / notnul(KDEMTO))* KUPTR;
      RKUST = (Crop.K_rt.Dmnd_stems  / notnul(KDEMTO))* KUPTR;
      RKURT = (Crop.K_rt.Dmnd_roots  / notnul(KDEMTO))* KUPTR;
}     


/* --------------------------------------------------------------------*/
/*  function NutritionINDX()                                           */
/*  Purpose: To compute N/P/K Nutrition Index (-)                      */
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
    
    float NPKI;
    
    /* Total vegetative living above-ground biomass (kg DM ha-1)     */
    VegetativeMass = Crop.st.leaves + Crop.st.stems;
    
    /* N/P/K concentration in total vegetative living per */
    /* kg above-ground biomass  (kg N/P/K kg-1 DM)        */
    N_Veg  = (Crop.N_st.leaves + Crop.N_st.stems)/notnul(VegetativeMass);
    P_Veg  = (Crop.P_st.leaves + Crop.P_st.stems)/notnul(VegetativeMass);
    K_Veg  = (Crop.K_st.leaves + Crop.K_st.stems)/notnul(VegetativeMass);
    
    /* Residual N/P/K concentration in total vegetative living */
    /* above-ground biomass  (kg N/P/K kg-1 DM)                */
    N_res = (Crop.st.leaves * N_ResidualFracLeaves +Crop.st.stems.*N_ResidualFracStems)/notnul(VegetativeMass);
    P_res = (Crop.st.leaves * P_ResidualFracLeaves +Crop.st.stems.*P_ResidualFracStems)/notnul(VegetativeMass);
    K_res = (Crop.st.leaves * K_ResidualFracLeaves +Crop.st.stems.*K_ResidualFracStems)/notnul(VegetativeMass);
       
    N_opt_veg = (Crop.N_st.Opt_leaves + Crop.N_st.Opt_stems) /notnul(VegetativeMass);
    P_opt_veg = (Crop.N_st.Opt_leaves + Crop.N_st.Opt_stems) /notnul(VegetativeMass);
    K_opt_veg = (Crop.N_st.Opt_leaves + Crop.N_st.Opt_stems) /notnul(VegetativeMass);
    
    float tiny=0.001;
    Crop.N_st.Indx = limit(tiny,1.0, ((N_Veg -N_res)/notnul(N_opt_veg - N_res)));
    Crop.P_st.Indx = limit(tiny,1.0, ((P_Veg -P_res)/notnul(P_opt_veg - P_res)));
    Crop.K_st.Indx = limit(tiny,1.0, ((N_Veg -K_res)/notnul(K_opt_veg - K_res)));
    
    NPKI = (Crop.N_st.Indx < Crop.P_st.Indx) ? Crop.N_st.Indx : Crop.P_st.Indx;
    NPKI = (NPKI < Crop.K_st.Indx) ? NPKI : Crop.K_st.Indx;
    
    /* Nutrient reduction factor */
    return limit(0., 1.0, 1.-NLUE*pow((1.0001-NPKI),2));
    
}

/* --------------------------------------------------------------------*/
/*  function NutrientINIT()                                            */
/*  Purpose: Initialization of nutrient parameters                     */
/* --------------------------------------------------------------------*/

void NutrientINIT()
{
        NLOSSL = 0.
        NLOSSR = 0.
	NLOSSS = 0.
	NUPTT  = 0.
	NFIXTT = 0.
	RNMINS = 0.
	RNMINT = 0.

        RNLV   = 0.
        RNST   = 0.
        RNRT   = 0.
        RNSO   = 0.
        TRANRF = 1.
        
	NNI = 1.
        PNI = 1.
        KNI = 1.

        RNLDST = 0.
        RNLDLV = 0.
        RNLDRT = 0.  
            
        PLOSSL = 0.
	PLOSSR = 0.
	PLOSSS = 0.
	PUPTT  = 0.
	RPMINS = 0.
	RPMINT = 0.
          
        KLOSSL = 0.
        KLOSSR = 0.
        KLOSSS = 0.
        KUPTT  = 0.
        RKMINS = 0.
        RKMINT = 0.
}     
      
/* --------------------------------------------------------------------*/
/*  function NutrientEmergence()                                       */
/*  Purpose: Initialization of nutrient parameters at emergence        */
/* --------------------------------------------------------------------*/

void NutrientEmergence()
{
    /* Initial maximum N concentration in plant organs per kg biomass [kg N kg-1 dry biomass]   */
    Crop.N_st.Max_leaves = Afgen(N_MaxLeaves, &DevelopmentStage);
    Crop.N_st.Max_stems  = N_MaxStems * Crop.N_st.Max_leaves;
    Crop.N_st.Max_roots  = N_MaxRoots * Crop.N_st.Max_leaves;
        
    /* Initial maximum N concentration in plant organs [kg N ]           */
    Crop.N_st.leaves = Crop.N_st.Max_leaves * Crop.st.leaves;
    Crop.N_st.stems  = Crop.N_st.Max_stems  * Crop.st.stems;
    Crop.N_st.roots  = Crop.N_st.Max_roots  * Crop.st.roots;
    Crop.N_st.storage = 0.;
       
    /* Initial maximum P concentration in plant organs per kg biomass [kg N kg-1 dry biomass]   */
    Crop.P_st.Max_leaves = Afgen(P_MaxLeaves, &DevelopmentStage);
    Crop.P_st.Max_stems  = P_MaxStems * Crop.P_st.Max_leaves;
    Crop.P_st.Max_roots  = P_MaxRoots * Crop.P_st.Max_leaves;
           
    /* Initial maximum P concentration in plant organs [kg N ] */
    Crop.P_st.leaves = Crop.P_st.Max_leaves * Crop.st.leaves;
    Crop.P_st.stems  = Crop.P_st.Max_stems  * Crop.st.stems;
    Crop.P_st.roots  = Crop.P_st.Max_roots  * Crop.st.roots;
    Crop.P_st.storage = 0.;
                  
    /* Initial maximum K concentration in plant organs per kg biomass [kg N kg-1 dry biomass]    */
    Crop.K_st.Max_leaves = Afgen(K_MaxLeaves, &DevelopmentStage);
    Crop.K_st.Max_stems = K_MaxStems * Crop.K_st.Max_leaves;
    Crop.K_st.Max_roots = K_MaxRoots * Crop.K_st.Max_leaves;
           
    /* Initial maximum k concentration in plant organs [kg N ] */
    Crop.K_st.leaves = Crop.K_st.Max_leaves * Crop.st.leaves;
    Crop.K_st.stems  = Crop.K_st.Max_stems  * Crop.st.stems;
    Crop.K_st.roots  = Crop.K_st.Max_roots  * Crop.st.roots;
    Crop.K_st.storage = 0.;
    
    /* Set the nutrient change rates to 0. */
    Crop.N_rt.leaves  = 0.;
    Crop.N_rt.stems   = 0.;
    Crop.N_rt.roots   = 0.;
    Crop.N_rt.storage = 0.;
           
    Crop.P_rt.leaves  = 0.;
    Crop.P_rt.stems   = 0.;
    Crop.P_rt.roots   = 0.;
    Crop.P_rt.storage = 0.;
           
    Crop.K_rt.leaves  = 0.;
    Crop.K_rt.stems   = 0.;
    Crop.K_rt.roots   = 0.;
    Crop.K_rt.storage = 0.;      
}     
      
/* ---------------------------------------------------------------*/
/*  function NutrientSowing()                                     */
/*  Purpose: Initialization of nutrient parameters at sowing      */
/* ---------------------------------------------------------------*/      
void NutrientSowing()
{    
    /* Set the nutrient states to 0. */
    Crop.N_st.leaves  = 0.;
    Crop.N_st.stems   = 0.;
    Crop.N_st.roots   = 0.;
    Crop.N_st.storage = 0.;
            
    Crop.P_st.leaves  = 0.;
    Crop.P_st.stems   = 0.;
    Crop.P_st.roots   = 0.;
    Crop.P_st.storage = 0.;
            
    Crop.K_st.leaves  = 0.;
    Crop.K_st.stems   = 0.;
    Crop.K_st.roots   = 0.;
    Crop.K_st.storage = 0.;
}            
  
/*------------------------------------------------------*/
/*  function  InitSoil()                                */
/*  Purpose: Initialization of soil nutrient parameters */
/* -----------------------------------------------------*/      
void InitSoil()
{
    NMINI = NMINS
    NMIN  = NMINI
            
    /* To avoid nutrient stress on day 1 */
    FERTN = LINT (FERNTAB,ILFERN, DAY)
    NRF   = LINT (NRFTAB,ILNRFT, DAY)
    NMINT = FERTN * NRF
    
    PMINI  = PMINS
    PMIN   = PMINI
            
    /* To avoid nutrient stress on day 1 */
    FERTP  = LINT (FERPTAB,ILFERP, DAY)
    PRF    = LINT (PRFTAB,ILPRFT, DAY)
    PMINT  = FERTP * PRF
       
    KMINI  = KMINS
    KMIN   = KMINI
            
    /* To avoid nutrient stress on day 1 */
    FERTK  = LINT (FERKTAB,ILFERK, DAY) reduction factor due to drought/wetness   
      TRANRF = TRA/NOTNUL(TRAMX)
    KRF    = LINT (KRFTAB,ILKRFT, DAY)
    KMINT  = FERTK * KRF
}        

void NutrientAvailStorage()
{
    /* N/P/K supply to the storage organs (kg N ha-1 d-1) */
    N_to_storage = insw(DevelopmentStage -DevelopmentStageNT,0.,ATN/TCNT)
    P_to_storage = insw(DevelopmentStage -DevelopmentStageNT,0.,ATP/TCPT)
    K_to_storage = insw(DevelopmentStage -DevelopmentStageNT,0.,ATK/TCKT)
    
    /* Total N/P/K demand (kg N/P/K ha-1) */
     NDEMTO = NDEML + NDEMS + NDEMR
     PDEMTO = PDEML + PDEMS + PDEMR
     KDEMTO = KDEML + KDEMS + KDEMR
    
    /* Rate of N/P/K uptake in grains (kg N/P/K ha-1 d-1) */
    Crop.N_rt.storage = min(Crop.N_rt.Dmnd_storage, N_to_storage);
    Crop.P_rt.storage = min(Crop.P_rt.Dmnd_storage, P_to_storage);
    Crop.K_rt.storage = min(Crop.K_rt.Dmnd_storage, K_to_storage);  
}

 
void NutrientStress()
{
    /* Establish the maximum nutrient concentrations in the crop organs*/
    NutrientMax();
    
    /* Establish the optimum nutrient concentrations in the crop organs*/
    NutrientOptimum();
    
    /* Calculate the nutrition index */
    NutritionINDX();
    
}

void NutrientRates()
{
    NutrientLoss();
    
    NutrientDemand();
    
    NutrientAvailStorage();
    
    
}
