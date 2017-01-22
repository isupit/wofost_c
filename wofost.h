#define NUMBER_OF_VARIABLES	46
#define NUMBER_OF_TABLES	15

typedef struct GREEN {
	float weight;
	float age;
	float area;
	struct GREEN *next;
	} Green;


typedef struct PLANT {
	     float roots;
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


AFGEN *Roots, *Stems, *Leaves, *Storage;
AFGEN *DeltaTempSum, *SpecificLeaveArea, *SpecificStemArea, *KDiffuseTb, *EFFTb;
AFGEN *MaxAssimRate, *FactorAssimRateTemp, *FactorGrossAssimTemp; 
AFGEN *FactorSenescence, *DeathRateStems, *DeathRateRoots;
     
/** Meteorological Variables  **/
int Station, Year;
float Longitude, Latitude, Altitude;
float Tmin[366], Tmax[366], Radiation[366], Rain[366];
float Windspeed[366], Vapour[366];

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
float Depnr;
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
