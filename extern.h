/* General global parameters */
int Day;
float DevelopmentStage;
float LAI;
float Temp;
float DayTemp;

/* General help functions */
extern float max(float a, float b);
extern float min(float a, float b);
extern float Afgen();
extern float limit(float a, float b, float c);
extern float notnul(float x);
extern float insw(float x1, float x2, float x3);


/* Crop growth */
extern int Astro();
extern int CalcPenman();

extern void Clean();
extern void RateCalculationCrop();
extern void Growth(float NewPlantMaterial);
extern void IntegrationCrop();
extern void InitializeCrop(int Emergence);

extern float GetDevelopmentStage();
extern float DailyTotalAssimilation();
extern float DyingLeaves();
extern float InstantAssimilation();
extern float LeaveGrowth(float LAIExp, float Newleaves);
extern float LeaveAreaIndex();
extern float Correct(float GrossAssimilation);
extern float RespirationRef(float TotalAssimilation);
extern float Conversion(float NetAssimilation);


/* Nutrients */
extern void CropNutrientRates();
extern void InitializeNutrients();
extern void IntegrationNutrients();
extern void NutritionINDX();
extern void NutrientLoss();
extern void NutrientMax();
extern void NutrientPartioning();
extern void NutrientRates();
extern void NutrientOptimum();
extern void NutrientDemand();
extern void SoilNutrientRates();
extern void NutrientTranslocation();
extern void RateCalcultionNutrients();


/* Read data */
extern int GetAfgenTables();
extern int GetCropData();
extern int GetMeteoData();
extern int GetSoilData();
extern int GetSiteData();
extern int GetManagement();
extern int FillCropVariables();
extern int FillSoilVariables();
extern int FillManageVariables();
extern int FillSiteVariables();
extern int FillAfgenTables();


/* Water balance */
extern void InitializeWatBal();
extern void RateCalulationWatBal();
extern void IntegrationWatBal();
extern void EvapTra();