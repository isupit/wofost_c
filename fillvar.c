#include <stdio.h>
#include "wofost.h"

/* ---------------------------------------------------------------------------*/
/*  function FillCropVariables(float *Variable)                               */
/*  Purpose: Fill the crop parameters that are read by GetCropData()          */
/* ---------------------------------------------------------------------------*/

Plant FillCropVariables(Plant *CROP, float *Variable)
{
    int i;
    CROP->prm.TempBaseEmergence        = Variable[0];
    CROP->prm.TempEffMax               = Variable[1];
    CROP->prm.TSumEmergence            = Variable[2];                      	     

    /**  Phenology  **/
    CROP->prm.IdentifyAnthesis         = (int) Variable[3]; 
    CROP->prm.OptimumDaylength         = Variable[4];	            
    CROP->prm.CriticalDaylength        = Variable[5];
    CROP->prm.TempSum1                 = Variable[6];       
    CROP->prm.TempSum2                 = Variable[7]; 
    CROP->prm.InitialDVS               = Variable[8];
    CROP->prm.DevelopStageHarvest      = Variable[9];

    /** Initial Values  **/
    CROP->prm.InitialDryWeight         = Variable[10];
    CROP->prm.LAIEmergence             = Variable[11];
    CROP->prm.RelIncreaseLAI           = Variable[12];

    /**  Green Area  **/
    CROP->prm.SpecificPodArea          = Variable[13];
    CROP->prm.LifeSpan                 = Variable[14];
    CROP->prm.TempBaseLeaves           = Variable[15];

    /** Conversion assimilates into biomass **/
    CROP->prm.ConversionLeaves         = Variable[16];
    CROP->prm.ConversionStorage        = Variable[17];
    CROP->prm.ConversionRoots          = Variable[18];
    CROP->prm.ConversionStems          = Variable[19];

    /** Maintenance Respiration **/
    CROP->prm.Q10                      = Variable[20];
    CROP->prm.RelRespiLeaves           = Variable[21];
    CROP->prm.RelRespiStorage          = Variable[22];
    CROP->prm.RelRespiRoots            = Variable[23];
    CROP->prm.RelRespiStems            = Variable[24];

    /** Death Rates  **/
    CROP->prm.MaxRelDeathRate          = Variable[25];

    /** Water Use  **/
    CROP->prm.CorrectionTransp         = Variable[26];
    CROP->prm.CropGroupNumber          = Variable[27];
    CROP->prm.Airducts                 = Variable[28];

    /** Rooting **/
    /** Convert cm to mm */
    CROP->prm.InitRootingDepth         = 10. * Variable[29];
    CROP->prm.MaxIncreaseRoot          = 10. * Variable[30];
    CROP->prm.MaxRootingDepth          = 10. * Variable[31];

    /** Nutrients  **/
    CROP->prm.DyingLeaves_NPK_Stress   = Variable[32];
    CROP->prm.DevelopmentStageNLimit   = Variable[33];
    CROP->prm.DevelopmentStageNT       = Variable[34];
    CROP->prm.FracTranslocRoots        = Variable[35];
    CROP->prm.Opt_N_Frac               = Variable[36];
    CROP->prm.Opt_P_Frac               = Variable[37];
    CROP->prm.Opt_K_Frac               = Variable[38];
    CROP->prm.N_MaxRoots               = Variable[39];
    CROP->prm.N_MaxStems               = Variable[40];
    CROP->prm.P_MaxRoots               = Variable[41];
    CROP->prm.P_MaxStems               = Variable[42];
    CROP->prm.K_MaxRoots               = Variable[43];
    CROP->prm.K_MaxStems               = Variable[44];
    CROP->prm.NitrogenStressLAI        = Variable[45];                	     
    CROP->prm.NLUE                     = Variable[46];
    CROP->prm.Max_N_storage            = Variable[47];
    CROP->prm.Max_P_storage            = Variable[48];
    CROP->prm.Max_K_storage            = Variable[49];
    CROP->prm.N_lv_partitioning        = Variable[50];
    CROP->prm.NutrientStessSLA         = Variable[51];
    CROP->prm.N_ResidualFrac_lv        = Variable[52];
    CROP->prm.N_ResidualFrac_st        = Variable[53];
    CROP->prm.N_ResidualFrac_ro        = Variable[54];
    CROP->prm.P_ResidualFrac_lv        = Variable[55];
    CROP->prm.P_ResidualFrac_st        = Variable[56];
    CROP->prm.P_ResidualFrac_ro        = Variable[57]; 
    CROP->prm.K_ResidualFrac_lv        = Variable[58];
    CROP->prm.K_ResidualFrac_st        = Variable[59];
    CROP->prm.K_ResidualFrac_ro        = Variable[60];
    CROP->prm.TCNT                     = Variable[61];
    CROP->prm.TCPT                     = Variable[62];
    CROP->prm.TCKT                     = Variable[63];
    CROP->prm.N_fixation               = Variable[64];


    for (i=0;i<=NR_VARIABLES_CRP;i++) {
    Variable[i] = 0.;
    }

 return *CROP;
}

Soil FillSoilVariables(Soil *SOIL, float *Variable) {
    int i;
    
    SOIL->ct.MoistureWP       = Variable[0];
    SOIL->ct.MoistureFC       = Variable[1];
    SOIL->ct.MoistureSAT      = Variable[2];
    SOIL->ct.CriticalSoilAirC = Variable[3];
    SOIL->ct.K0               = Variable[4];
    SOIL->ct.MaxPercolRTZ     = Variable[5];
    SOIL->ct.MaxPercolSubS    = Variable[6];
    
    
    /* No workability parameters will be used in this version */
    
    for (i=0;i<=NR_VARIABLES_SOIL;i++) {
        Variable[i]= 0.;
    }
    
    return *SOIL;
}

Field FillSiteVariables(Field *SITE, float *Variable) {
    int i;
   
    SITE->FlagGroundWater                 = Variable[0];
    SITE->InfRainDependent                = Variable[1];
    SITE->FlagDrains                      = Variable[2];
    SITE->MaxSurfaceStorage               = Variable[3];
    SITE->InitSoilMoisture                = Variable[4];
    SITE->GroundwaterDepth                = Variable[5];
    SITE->DD                              = Variable[6];
    SITE->SoilLimRootDepth                = Variable[7];
    SITE->NotInfiltrating                 = Variable[8];
    SITE->SurfaceStorage                  = Variable[9];
    SITE->MaxInitSoilM                    = Variable[10];
    
    CO2 = Variable[11];
       
    for (i=0;i<=NR_VARIABLES_SITE;i++) {
        Variable[i]= 0.;
    }
    
    return *SITE;
}

Management FillManageVariables(Management *MNG, float *Variable) {
    int i;
    
    MNG->N_Mins                          = Variable[0];
    MNG->NRecoveryFrac                   = Variable[1];
    MNG->P_Mins                          = Variable[2];
    MNG->PRecoveryFrac                   = Variable[3];
    MNG->K_Mins                          = Variable[4];
    MNG->KRecoveryFrac                   = Variable[5];
    
    for (i=0;i<=NR_VARIABLES_MANAGEMENT;i++) {
        Variable[i]= 0.;
    }
    
    return *MNG;
}
