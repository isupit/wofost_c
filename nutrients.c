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
    RNLDRT= RNFRT*Crop.drt.roots;
    RNLDST= RNFST*Crop.drt.stems;
    RNLDLV= RNFLV*Crop.drt.leaves;
    
    RPLDRT = RPFRT*Crop.drt.roots;
    RPLDST = RPFST*Crop.drt.stems;
    RPLDLV = RPFLV*Crop.drt.leaves;
    
    RKLDRT = RKFRT*Crop.drt.roots;
    RKLDST = RKFST*Crop.drt.stems;
    RKLDLV = RKFLV*Crop.drt.leaves;
}      


void NutrientOptimum()
{  
    float TBGMR;
    
    /* Total vegetative living above-ground biomass (kg DM ha-1)     */
    TBGMR = Crop.st.leaves + Crop.st.stems;
      
    /* Optimal N/P/K amount in vegetative above-ground living biomass and its N concentration */
    NOPTL  = FRNX * NMAXLV * Crop.st.leaves;
    NOPTS  = FRNX * NMAXST * Crop.st.stems;
    NOPTMR = (NOPTL + NOPTS)/notnul(TBGMR);
    
    POPTL  = FRPX * PMAXLV * Crop.st.leaves;
    POPTS  = FRPX * PMAXST * Crop.st.stems;
    POPTMR = (POPTL + POPTS)/notnul(TBGMR);

    KOPTL  = FRKX * KMAXLV * Crop.st.leaves;
    KOPTS  = FRKX * KMAXST * Crop.st.stems;
    KOPTMR = (KOPTL+ KOPTS)/notnul(TBGMR);
}
 
/* -------------------------------------------------------------------------*/
/*  function NutrientDemand()                                                       */
/*  Purpose: To compute the nutrient demand of crop organs (kg N/P/K ha-1)  */
/* -------------------------------------------------------------------------*/

void NutrientDemand()
{
      NDEML  =  max (NMAXLV*Crop.st.leaves  - ANLV, 0.);
      NDEMS  =  max (NMAXST*Crop.st.stems  - ANST, 0.);
      NDEMR  =  max (NMAXRT*Crop.st.roots  - ANRT, 0.);
      NDEMSO =  max (NMAXSO*WSO  - ANSO, 0.)/TCNT;

      PDEML  =  max (PMAXLV*Crop.st.leaves  - APLV, 0.);
      PDEMS  =  max (PMAXST*Crop.st.stems  - APST, 0.);
      PDEMR  =  max (PMAXRT*Crop.st.roots  - APRT, 0.);
      PDEMSO =  max (PMAXSO*WSO  - APSO, 0.)/TCPT;

      KDEML  =  max (KMAXLV*Crop.st.leaves  - AKLV, 0.);
      KDEMS  =  max (KMAXST*Crop.st.stems  - AKST, 0.);
      KDEMR  =  max (KMAXRT*Crop.st.roots  - AKRT, 0.);
      KDEMSO =  max (KMAXSO*WSO  - AKSO, 0.)/TCKT;
}
 
      
      
/* ---------------------------------------------------------------------------*/
/*  function AvailableTranslocation                                           */                                  */
/*  Purpose: To compute the amount of nutrients in the organs that can        */
/*  be translocated (kg N ha-1)                                                */
/* ---------------------------------------------------------------------------*/

void AvailableTranslocation()
{
      ATNLV = max (0. , ANLV-Crop.st.leaves*RNFLV)
      ATNST = max (0. , ANST-Crop.st.stems*RNFST)
      ATNRT = max (0. , min((ATNLV + ATNST) * FNTRT, ANRT-Crop.st.roots*RNFRT))
      ATN   = ATNLV +  ATNST + ATNRT
      
      ATPLV = max (0. , APLV-Crop.st.leaves*RPFLV)
      ATPST = max (0. , APST-Crop.st.stems*RPFST)
      ATPRT = max (0. , min((ATPLV + ATPST) * FNTRT, APRT-Crop.st.roots*RPFRT))
      ATP   = ATPLV +  ATPST + ATPRT
      
      ATKLV = max (0. , AKLV-Crop.st.leaves*RKFLV)
      ATKST = max (0. , AKST-Crop.st.stems*RKFST)
      ATKRT = max (0. , min((ATKLV + ATKST) * FNTRT, AKRT-Crop.st.roots*RKFRT))
      ATK   = ATKLV +  ATKST + ATKRT
}      
     
      
/* ---------------------------------------------------------------------------*/
/*  function Translocation()                                                  */
/*  Purpose: To compute the amount of nutrients translocated from various     */ 
/*  organs (kg N ha-1 d-1)                                                    */
/* ---------------------------------------------------------------------------*/

void Translocation()
{
      RNTLV= RNSO* ATNLV/ notnul(ATN)
      RNTST= RNSO* ATNST/ notnul(ATN)
      RNTRT= RNSO* ATNRT/ notnul(ATN)
      
      RPTLV= RPSO* ATPLV/ notnul(ATP)
      RPTST= RPSO* ATPST/ notnul(ATP)
      RPTRT= RPSO* ATPRT/ notnul(ATP)

      RKTLV= RKSO* ATKLV/ notnul(ATK)
      RKTST= RKSO* ATKST/ notnul(ATK)
      RKTRT= RKSO* ATKRT/ notnul(ATK)
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
    float NMAXLVI;
    float NMAXSTI;
    float NMAXRTI; 
    
    float PMAXLVI;
    float PMAXSTI;
    float PMAXRTI; 
    
    float KMAXLVI;
    float KMAXSTI;
    float KMAXRTI; 
    
    NMAXLVI= Afgen (N_MaxLeaves, &DevelopmentStage);
    NMAXSTI= N_MaxStems * NMAXLVI;
    NMAXRTI= N_MaxRoots * NMAXLVI;
        
    /* Initial maximum N concentration in plant organs [kg N ]           */
    Crop.N_st.leaves = NMAXLVI * Crop.st.leaves;
    Crop.N_st.stems  = NMAXSTI * Crop.st.stems;
    Crop.N_st.roots  = NMAXRTI * Crop.st.roots;
    Crop.N_st.storage = 0.;
       
    /* Initial maximum P concentration in plant organs per kg biomass [kg N kg-1 dry biomass]   */
    PMAXLVI= Afgen (P_MaxLeaves, &DevelopmentStage);
    PMAXSTI= P_MaxStems * PMAXLVI;
    PMAXRTI= P_MaxRoots * PMAXLVI;
           
    /* Initial maximum P concentration in plant organs [kg N ] */
    Crop.P_st.leaves = PMAXLVI * Crop.st.leaves;
    Crop.P_st.stems  = PMAXSTI * Crop.st.stems;
    Crop.P_st.roots  = PMAXRTI * Crop.st.roots;
    Crop.P_st.storage = 0.;
                  
    /* Initial maximum K concentration in plant organs per kg biomass [kg N kg-1 dry biomass]    */
    KMAXLVI= Afgen (K_MaxLeaves, &DevelopmentStage)
    KMAXSTI= LSPR * KMAXLVI
    KMAXRTI= LRPR * KMAXLVI
           
    /* Initial maximum k concentration in plant organs [kg N ] */
    AKLVI= KMAXLVI * Crop.st.leaves
    AKSTI= KMAXSTI * Crop.st.stems
    AKRTI= KMAXRTI * Crop.st.roots
          
    AKLV = AKLVI
    AKST = AKSTI
    AKRT = AKRTI
    AKSO = AKSOI     
}     
      
/* ---------------------------------------------------------------*/
/*  function NutrientSowing()                                     */
/*  Purpose: Initialization of nutrient parameters at sowing      */
/* ---------------------------------------------------------------*/      
void NutrientSowing()
{      
            ANLV= 0.
	    ANST= 0.
	    ANRT= 0.
            ANSO= 0.
            
            APLV= 0.
	    APST= 0.
	    APRT= 0.
            APSO= 0.
            
            AKLV= 0.
	    AKST= 0.
	    AKRT= 0.
            AKSO= 0.
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
      