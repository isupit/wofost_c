#include <stdio.h>
#include "wofost.h"

/* ---------------------------------------------------------------------------*/
/*  function FillCropVariables()                                              */ 
/*  Purpose: To fill the crop parameters that are used  for the crop growth   */
/*           simulation                                                       */
/* ---------------------------------------------------------------------------*/

int FillCropVariables(float *Variable)
{
    int i;
TempBaseEmergence        = Variable[0];
TempEffMax               = Variable[1];
TSumEmergence            = Variable[2];                      	     

/**  Phenology  **/
IdentifyAnthesis         = (int) Variable[3]; 
OptimumDaylength         = Variable[4];	            
CriticalDaylength        = Variable[5];
TempSum1                 = Variable[6];       
TempSum2                 = Variable[7]; 
InitialDVS		 = Variable[8];
DevelopStageHarvest      = Variable[9];

/** Initial Values  **/
InitialDryWeight         = Variable[10];
LAIEmergence             = Variable[11];
RelIncreaseLAI           = Variable[12];

/**  Green Area  **/
SpecificPodArea          = Variable[13];
LifeSpan                 = Variable[14];
TempBaseLeaves           = Variable[15];

/** Conversion assimilates into biomass **/
ConversionLeaves         = Variable[16];
ConversionStorage        = Variable[17];
ConversionRoots          = Variable[18];
ConversionStems          = Variable[19];

/** Maintenance Respiration **/
Q10                      = Variable[20];
RelRespiLeaves           = Variable[21];
RelRespiStorage          = Variable[22];
RelRespiRoots            = Variable[23];
RelRespiStems            = Variable[24];

/** Death Rates  **/
MaxRelDeathRate          = Variable[25];

/** Water Use  **/
CorrectionTransp         = Variable[26];
CropGroupNumber          = Variable[27];
Airducts                 = Variable[28];

/** Rooting **/
/** Convert cm to mm */
InitRootingDepth         = 10. * Variable[29];
MaxIncreaseRoot          = 10. * Variable[30];
MaxRootingDepth          = 10. * Variable[31];

/** Nutrients  **/
DyingLeaves_NPK_Stress   = Variable[32];
DevelopmentStageNLimit   = Variable[33];
DevelopmentStageNT       = Variable[34];
FracTranslocRoots        = Variable[35];
Opt_N_Frac               = Variable[36];
Opt_P_Frac               = Variable[37];
Opt_K_Frac               = Variable[38];
N_MaxRoots               = Variable[39];
N_MaxStems               = Variable[40];
P_MaxRoots               = Variable[41];
P_MaxStems               = Variable[42];
K_MaxRoots               = Variable[43];
K_MaxStems               = Variable[44];
NitrogenStressLAI        = Variable[45];                	     
NLUE                     = Variable[46];
Max_N_storage            = Variable[47];
Max_P_storage            = Variable[48];
Max_K_storage            = Variable[49];
N_lv_partitioning        = Variable[50];
NutrientStessSLA         = Variable[51];
N_ResidualFrac_lv        = Variable[52];
N_ResidualFrac_st        = Variable[53];
N_ResidualFrac_ro        = Variable[54];
P_ResidualFrac_lv        = Variable[55];
P_ResidualFrac_st        = Variable[56];
P_ResidualFrac_ro        = Variable[57]; 
K_ResidualFrac_lv        = Variable[58];
K_ResidualFrac_st        = Variable[59];
K_ResidualFrac_ro        = Variable[60];
TCNT                     = Variable[61];
TCPT                     = Variable[62];
TCKT                     = Variable[63];
N_fixation               = Variable[64];


for (i=0;i<=NR_VARIABLES_CRP;i++) {
    Variable[i] = 0.;
}

 return 1;
}

int FillSoilVariables(float *Variable) {
    int i;
    
    WatBal.ct.MoistureWP       = Variable[0];
    WatBal.ct.MoistureFC       = Variable[1];
    WatBal.ct.MoistureSAT      = Variable[2];
    WatBal.ct.CriticalSoilAirC = Variable[3];

    WatBal.ct.MaxPercolRTZ     = Variable[5];
    WatBal.ct.MaxPercolSubS    = Variable[6];
    WatBal.ct.K0               = Variable[4];
    
    /* No workability parameters will be used in this version */
    
    for (i=0;i<=NR_VARIABLES_SOIL;i++) {
        Variable[i]= 0.;
    }
    
    return 1;
}

int FillSiteVariables(float *Variable) {
    int i;
   
    Site.FlagGroundWater                 = Variable[0];
    Site.InfRainDependent                = Variable[1];
    Site.FlagDrains                      = Variable[2];
    Site.MaxSurfaceStorage               = Variable[3];
    Site.InitSoilMoisture                = Variable[4];
    Site.GroundwaterDepth                = Variable[5];
    Site.DD                              = Variable[6];
    Site.SoilLimRootDepth                = Variable[7];
    Site.NotInfiltrating                 = Variable[8];
    Site.SurfaceStorage                  = Variable[9];
    Site.MaxInitSoilM                    = Variable[10];
    
    CO2 = Variable[11];
       
    for (i=0;i<=NR_VARIABLES_SITE;i++) {
        Variable[i]= 0.;
    }
    
    return 1;
}

int FillManageVariables(float *Variable) {
    int i;
    
    Site.N_Mins                          = Variable[0];
    Site.NRecoveryFrac                   = Variable[1];
    Site.P_Mins                          = Variable[2];
    Site.PRecoveryFrac                   = Variable[3];
    Site.K_Mins                          = Variable[4];
    Site.KRecoveryFrac                   = Variable[5];
    
    for (i=0;i<=NR_VARIABLES_MANAGEMENT;i++) {
        Variable[i]= 0.;
    }
    
    return 1;
}
