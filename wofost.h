#define NUMBER_OF_VARIABLES	46
#define NUMBER_OF_TABLES	15

typedef struct CONSTANTS {
    float MaxEvapWater;
    float MoistureFC;
    float MoistureWP;
    float MoistureSAT;
    float CriticalSoilAirC;
    float MaxPercolRTZ;
    float MaxPercolSubS;
    float K0;
} Constants;

typedef struct STATES {
        float EvapWater;
        float EvapSoil;
        float Infiltration;
        float Irrigation;
        float Loss;
        float MoistureLOW;
        float Percolation;
        float RootZoneMoisture;
        float SurfaceStorage;
        float Transpiration;
        float MoistureLOW;
        } States;

typedef struct RATES {
        float EvapWater;
        float EvapSoil;
        float Infiltration;
        float Irrigation;
        float Loss; 
        float MoistureLOW;
        float Percolation;
        float RootZoneMoisture;
        float SurfaceStorage;
        float Transpiration;
        float WaterRootGrowth;
        } Rates;
 
typedef struct SOIL {
    Constants ct;
    States st;
    Rates rt;
    } Soil;
    
    Soil WatBal;

typedef struct GREEN {
	float weight;
	float age;
	float area;
	struct GREEN *next;
	} Green;

typedef struct PLANT {
	     float roots;
             float rootdepth;
	     float stems;
             float leaves;
	     float storage;
	     float LAIExp;
	     Green *LeaveProperties;
	     } Plant;
Plant Crop;

typedef struct TABLE {
	     float x;
	     float y;
	     struct TABLE *next;
	     } AFGEN;

/* Tables for Crop */
AFGEN *Roots, *Stems, *Leaves, *Storage;
AFGEN *DeltaTempSum, *SpecificLeaveArea, *SpecificStemArea, *KDiffuseTb, *EFFTb;
AFGEN *MaxAssimRate, *FactorAssimRateTemp, *FactorGrossAssimTemp; 
AFGEN *FactorSenescence, *DeathRateStems, *DeathRateRoots;
     
/* Tables for Soil */
AFGEN VolumetricSoilMoisture;
AFGEN HydraulicConducitiy; /* currently not used */

/** Meteorological Variables  **/
int Station, Year;
float Longitude, Latitude, Altitude;
float Tmin[366], Tmax[366], Radiation[366], Rain[366];
float Windspeed[366], Vapour[366];

/* Time step */
float Delta;

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