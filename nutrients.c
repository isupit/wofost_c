#include <stdio.h>
#include <stdlib.h>
#include "dynamic.h"
#include "wofost.h"

/* ---------------------------------------------------------------------------*/
/*  NutrientLoss                                                              */
/*  Purpose: To calculate nutrient loss rate of dying of roots, stems leaves  */
/*           and storage organs (kg N ha-1 d-1)                               */
/* ---------------------------------------------------------------------------*/     
void NutrientLoss() 
{         
    RNLDRT= N_ResidualFracRoots * Crop.drt.roots;
    RNLDST= N_ResidualFracStems * Crop.drt.stems;
    RNLDLV= N_ResidualFracLeaves* Crop.drt.leaves;
    
    RPLDRT = P_ResidualFracRoots * Crop.drt.roots;
    RPLDST = P_ResidualFracStems * Crop.drt.stems;
    RPLDLV = P_ResidualFracLeaves* Crop.drt.leaves;
    
    RKLDRT = K_ResidualFracRoots * Crop.drt.roots;
    RKLDST = K_ResidualFracStems * Crop.drt.stems;
    RKLDLV = K_ResidualFracLeaves* Crop.drt.leaves;
}      


void NutrientOptimum(float *NMAXLV, float *PMAXLV, float *KMAXLV)
{  
    float VegetativeMass;
    
    /* Total vegetative living above-ground biomass (kg DM ha-1)     */
    VegetativeMass = Crop.st.leaves + Crop.st.stems;
      
    /* Optimal N/P/K amount in vegetative above-ground living biomass */
    /* and its N concentration                                        */
    NOPTL  = FRNX * NMAXLV * Crop.st.leaves;
    NOPTS  = FRNX * NMAXST * Crop.st.stems;
    NOPTMR = (NOPTL + NOPTS)/notnul(VegetativeMass);
    
    POPTL  = FRPX * PMAXLV * Crop.st.leaves;
    POPTS  = FRPX * PMAXST * Crop.st.stems;
    POPTMR = (POPTL + POPTS)/notnul(VegetativeMass);

    KOPTL  = FRKX * KMAXLV * Crop.st.leaves;
    KOPTS  = FRKX * KMAXST * Crop.st.stems;
    KOPTMR = (KOPTL+ KOPTS)/notnul(VegetativeMass);
}
 
/* -------------------------------------------------------------------------*/
/*  function NutrientDemand()                                                       */
/*  Purpose: To compute the nutrient demand of crop organs (kg N/P/K ha-1)  */
/* -------------------------------------------------------------------------*/

void NutrientDemand()
{
      NDEML  =  max (NMAXLV*Crop.st.leaves - Crop.N_st.leaves, 0.);
      NDEMS  =  max (NMAXST*Crop.st.stems  - Crop.N_st.stems, 0.);
      NDEMR  =  max (NMAXRT*Crop.st.roots  - Crop.N_st.roots, 0.);
      NDEMSO =  max (NMAXSO*Crop.st.storage- Crop.N_st.storage, 0.)/TCNT;

      PDEML  =  max (PMAXLV*Crop.st.leaves - Crop.P_st.leaves, 0.);
      PDEMS  =  max (PMAXST*Crop.st.stems  - Crop.P_st.stems, 0.);
      PDEMR  =  max (PMAXRT*Crop.st.roots  - Crop.P_st.roots, 0.);
      PDEMSO =  max (PMAXSO*Crop.st.storage- Crop.P_st.storage, 0.)/TCPT;

      KDEML  =  max (KMAXLV*Crop.st.leaves - Crop.K_st.leaves, 0.);
      KDEMS  =  max (KMAXST*Crop.st.stems  - Crop.K_st.stems, 0.);
      KDEMR  =  max (KMAXRT*Crop.st.roots  - Crop.K_st.roots, 0.);
      KDEMSO =  max (KMAXSO*Crop.st.storage- Crop.K_st.storage, 0.)/TCKT;
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

    
    RNTLV= RNSO * ATNLV/ notnul(N_AvailTransloc);
    RNTST= RNSO * ATNST/ notnul(N_AvailTransloc);
    RNTRT= RNSO * ATNRT/ notnul(N_AvailTransloc);
    
    RPTLV= RPSO* ATPLV/ notnul(P_AvailTransloc);
    RPTST= RPSO* ATPST/ notnul(P_AvailTransloc);
    RPTRT= RPSO* ATPRT/ notnul(P_AvailTransloc);

    RKTLV= RKSO* ATKLV/ notnul(K_AvailTransloc);
    RKTST= RKSO* ATKST/ notnul(K_AvailTransloc);
    RKTRT= RKSO* ATKRT/ notnul(K_AvailTransloc);
}     

/* -------------------------------------------------------------------------*/
/*  function NutrientPartioning()                                                         */
/*  Purpose: To compute the partitioning of the total N/P/K uptake rate     */
/*           (N/P/K UPTR) over leaves, stem, and roots (kg N/P/K ha-1 d-1)  */
/* -------------------------------------------------------------------------*/

void NutrientPartioning()
{     
      RNULV = (NDEML / notnul(NDEMTO))* (NUPTR+NFIXTR)
      RNUST = (NDEMS / notnul(NDEMTO))* (NUPTR+NFIXTR)
      RNURT = (NDEMR / notnul(NDEMTO))* (NUPTR+NFIXTR)
    
      RPULV = (PDEML / notnul(PDEMTO))* PUPTR
      RPUST = (PDEMS / notnul(PDEMTO))* PUPTR
      RPURT = (PDEMR / notnul(PDEMTO))* PUPTR

      RKULV = (KDEML / notnul(KDEMTO))* KUPTR
      RKUST = (KDEMS / notnul(KDEMTO))* KUPTR
      RKURT = (KDEMR / notnul(KDEMTO))* KUPTR
}     


/* --------------------------------------------------------------------*/
/*  function NutritionINDX()                                           */
/*  Purpose: To compute N/P/K Nutrition Index (-)                      */
/* --------------------------------------------------------------------*/

void NutritionINDX()
{    
      TINY=0.001
      NNI = limit (TINY,1.0, ((NFGMR-NRMR)/notnul(NOPTMR-NRMR)))
      PNI = limit (TINY,1.0, ((PFGMR-PRMR)/notnul(POPTMR-PRMR)))
      KNI = limit (TINY,1.0, ((KFGMR-KRMR)/notnul(KOPTMR-KRMR)))
      
      NPKI = MIN(NNI,PNI,KNI)
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
    float Max_N_leaves;
    float Max_N_stems;
    float Max_N_roots; 
    
    float Max_P_leaves;
    float Max_P_stems;
    float Max_P_roots; 
    
    float Max_K_leaves;
    float Max_K_stems;
    float Max_K_roots; 
    
    /* Initial maximum N concentration in plant organs per kg biomass [kg N kg-1 dry biomass]   */
    Max_N_leaves= Afgen (N_MaxLeaves, &DevelopmentStage);
    Max_N_stems = Fraction_N_MaxStems * Max_N_leaves;
    Max_N_roots = Fraction_N_MaxRoots * Max_N_leaves;
        
    /* Initial maximum N concentration in plant organs [kg N ]           */
    Crop.N_st.leaves = Max_N_leaves * Crop.st.leaves;
    Crop.N_st.stems  = Max_N_stems  * Crop.st.stems;
    Crop.N_st.roots  = Max_N_roots  * Crop.st.roots;
    Crop.N_st.storage = 0.;
       
    /* Initial maximum P concentration in plant organs per kg biomass [kg N kg-1 dry biomass]   */
    Max_P_leaves = Afgen (P_MaxLeaves, &DevelopmentStage);
    Max_P_stems  = Fraction_P_MaxStems * Max_P_leaves;
    Max_P_roots  = Fraction_K_MaxRoots * Max_P_leaves;
           
    /* Initial maximum P concentration in plant organs [kg N ] */
    Crop.P_st.leaves = Max_P_leaves * Crop.st.leaves;
    Crop.P_st.stems  = Max_P_stems  * Crop.st.stems;
    Crop.P_st.roots  = Max_P_roots  * Crop.st.roots;
    Crop.P_st.storage = 0.;
                  
    /* Initial maximum K concentration in plant organs per kg biomass [kg N kg-1 dry biomass]    */
    Max_K_leaves = Afgen (K_MaxLeaves, &DevelopmentStage);
    Max_K_stems = Fraction_K_MaxStems * Max_K_leaves;
    Max_K_roots = Fraction_K_MaxRoots * Max_K_leaves;
           
    /* Initial maximum k concentration in plant organs [kg N ] */
    Crop.K_st.leaves = Max_K_leaves * Crop.st.leaves;
    Crop.K_st.stems  = Max_K_stems  * Crop.st.stems;
    Crop.K_st.roots  = Max_K_roots  * Crop.st.roots;
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
    FERTK  = LINT (FERKTAB,ILFERK, DAY)
    KRF    = LINT (KRFTAB,ILKRFT, DAY)
    KMINT  = FERTK * KRF
}        
      