#include <stdio.h>
#include "wofost.h"


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
InitRootingDepth         = Variable[29];
MaxIncreaseRoot          = Variable[30];
MaxRootingDepth          = Variable[31];

/** Nutrients  **/
NMINSO                   = Variable[32];
NMINVE                   = Variable[33];
NMAXSO                   = Variable[34];
NMAXVE                   = Variable[35];

PMINSO                   = Variable[36];
PMINVE                   = Variable[37];
PMAXSO                   = Variable[38];
PMAXVE                   = Variable[39];

KMINSO                   = Variable[40];
KMINVE                   = Variable[41];
KMAXSO                   = Variable[42];
KMAXVE                   = Variable[43];

YZERO                    = Variable[44];
NFIX                     = Variable[45];                	     

for (i=0;i<=NUMBER_OF_VARIABLES;i++) {
    Variable[i] = 0.;
}

 return 1;
}

int FillSoilVariables(float *Variable) {
    int i;
    
    SoilMoistureWP       = Variable[0];
    SoilMoistureFC       = Variable[1];
    SoilMoistureSAT      = Variable[2];
    CriticalSoilAirC     = Variable[3];

    HydraulicConductivityC   = Variable[4];
    RootZoneMaxPercolationR  = Variable[5];
    SubSoilMaxPercolationR   = Variable[6];
    
    /* No workability parameters will be used in this version */
    
    for (i=0;i<=NUMBER_OF_VARIABLES;i++) {
        Variable[i]= 0.;
    }
    
    return 1;
}

int FillSiteVariables(float *Variable) {
    int i;
    
    
    Site.SoilName                        = Variable[0];
    Site.FlagGroundWater                 = Variable[1];
    Site.FlagFixInfiltration             = Variable[2];
    Site.FlagDrains                      = Variable[3];
    Site.MaxSurfaceStorage               = Variable[4];
    Site.InitSoilMoisture                = Variable[5];
    Site.GroundwaterDepth                = Variable[6];
    Site.DD                              = Variable[7];
    Site.SoilLimRootDepth                = Variable[8];
    Site.NotInfiltrating                 = Variable[9];
    Site.NBase                           = Variable[10];
    Site.NRecoveryFrac                   = Variable[11];
    Site.PBase                           = Variable[12];
    Site.PRecoveryFrac                   = Variable[13];
    Site.KBase                           = Variable[14];
    Site.KRecoveryFrac                   = Variable[15];
    Site.SurfaceStorage                  = Variable[16];
    Site.SurfaceStorageLim               = Variable[17];
       
    for (i=0;i<=NUMBER_OF_VARIABLES;i++) {
        Variable[i]= 0.;
    }
    
    return 1;
}