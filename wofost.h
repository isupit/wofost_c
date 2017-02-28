#define NR_VARIABLES_CRP	46
#define NR_TABLES_CRP   	15
#define NR_VARIABLES_SITE       17
#define NR_TABLES_SITE          1
#define NR_VARIABLES_SOIL       12
#define NR_TABLES_SOIL          2



typedef struct TABLE {
	     float x;
	     float y;
	     struct TABLE *next;
	     } AFGEN;


typedef struct SITE {
   float AngstA;
   float AngstB;
   float SoilName;
   float FlagGroundWater;
   float InfRainDependent;
   float FlagDrains;
   float MaxSurfaceStorage;     
   float InitSoilMoisture;
   float GroundwaterDepth;
   float DD;
   float SoilLimRootDepth;
   float NotInfiltrating;
   float NBase;
   float NRecoveryFrac;
   float PBase;
   float PRecoveryFrac;
   float KBase; 
   float KRecoveryFrac;
   float SurfaceStorage;
   float MaxInitSoilM;
   AFGEN *NotInfTB;
} Field;

Field Site;

typedef struct CONSTANTS {
    float MaxEvapWater;
    float MoistureFC;
    float MoistureWP;
    float MoistureSAT;
    float CriticalSoilAirC;
    float MaxPercolRTZ;
    float MaxPercolSubS;
    float MaxSurfaceStorge;
    float K0;
} Constants;

typedef struct STATES {
        float EvapWater;
        float EvapSoil;
        float Infiltration;
        float Irrigation;
        float Loss;
        float Moisture;
        float MoistureLOW;
        float Percolation;
        float Rain;
        float RootZoneMoisture;
        float Runoff;
        float SurfaceStorage;
        float Transpiration;
        float WaterRootExt;
        } States;

typedef struct RATES {
        float EvapWater;
        float EvapSoil;
        float Infiltration;
        float Irrigation;
        float Loss;
        float Moisture;
        float MoistureLOW;
        float Percolation;
        float RootZoneMoisture;
        float Runoff;
        float SurfaceStorage;
        float Transpiration;
        float WaterRootExt;
        } Rates;
 
typedef struct SOIL {
    float DaysSinceLastRain;
    float SoilMaxRootingDepth;
    Constants ct;
    States st;
    Rates rt;
    } Soil;
    
Soil WatBal;

typedef struct P_RATES {
    float roots;
    float stems;
    float leaves;
    float LAIExp;
    float storage;
} p_rates;

typedef struct P_STATES {
    float roots;
    float stems;
    float leaves;
    float LAIExp;
    float storage;
} p_states;


typedef struct GREEN {
	float weight;
	float age;
	float area;
	struct GREEN *next;
	} Green;

typedef struct PLANT {
             float RootDepth;
             float RootDepth_prev;
             float MaxRootingDepth;
             p_rates rt;
             p_states st; 
	     Green *LeaveProperties;
	     } Plant;
Plant Crop;


/* Tables for Crop */
AFGEN *Roots, *Stems, *Leaves, *Storage;
AFGEN *DeltaTempSum, *SpecificLeaveArea, *SpecificStemArea, *KDiffuseTb, *EFFTb;
AFGEN *MaxAssimRate, *FactorAssimRateTemp, *FactorGrossAssimTemp; 
AFGEN *FactorSenescence, *DeathRateStems, *DeathRateRoots;
     
/* Tables for Soil */
AFGEN *VolumetricSoilMoisture;
AFGEN *HydraulicConducitiy; /* currently not used */

/* Table for rainfall dependent infiltration */
AFGEN *NotInfTB; 

/** Meteorological Variables  **/
int Station, Year;
float Longitude, Latitude, Altitude;
float Tmin[366], Tmax[366], Radiation[366], Rain[366];
float Windspeed[366], Vapour[366];

/* Time step */
float Step;

/** Static Variables  **/
/**  Emergence  **/
float TempBaseEmergence;
float TempEffMax;
float TSumEmergence;                      	     

/**  Phenology  **/
int   IdentifyAnthesis; 
float OptimumDaylength;	            
float CriticalDaylength;
float TempSum1;       
float TempSum2; 
float InitialDVS;
float DevelopStageHarvest;

/** Initial Values  **/
float InitialDryWeight;
float LAIEmergence;
float RelIncreaseLAI;

/**  Green Area  **/
float SpecificPodArea;
float LifeSpan;
float TempBaseLeaves;

/** Conversion assimilates into biomass **/
float ConversionLeaves;
float ConversionStorage;
float ConversionRoots;
float ConversionStems;

/** Maintenance Respiration **/
float Q10;
float RelRespiLeaves;
float RelRespiStorage;
float RelRespiRoots;
float RelRespiStems;

/** Death Rates  **/
float MaxRelDeathRate;

/** Water Use  **/
float CorrectionTransp;
float CropGroupNumber;
float Airducts;

/** Rooting **/
float InitRootingDepth;
float MaxIncreaseRoot;
float MaxRootingDepth;

/** Nutrients  **/
float NMINSO;
float NMINVE;
float NMAXSO;
float NMAXVE;

float PMINSO;
float PMINVE;
float PMAXSO;
float PMAXVE;

float KMINSO;
float KMINVE;
float KMAXSO;
float KMAXVE;

float YZERO;
float NFIX;

/** Soil **/
float SoilMoistureFC;
float SoilMoistureSAT;
float SoilMoistureWP;
float CriticalSoilAirC;

float SoilMoisture;
float DaysOxygenStress;
    
float RootZoneMaxPercolationR;
float HydraulicConductivityC;
float SubSoilMaxPercolationR;

float ShadedWaterMaxEvap;
float ShadedSoilMaxEvap;
float InitWaterAv;
float InitSurfaceStorage;
float MaxSurfaceStorage;
float MaxInitTopSoilMoistureC;

float AvailableSoilM;
float AvailRootZoneMoisture;
float InitAvailMoisture;
float InitSoilMoisture;
float DaysSinceLastRain;
float MoistureLOW;