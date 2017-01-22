#include <stdio.h>
#include "wofost.h"


int FillVariables(float *Variable)
{
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
Depnr                    = Variable[27];
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

 return 1;
}

