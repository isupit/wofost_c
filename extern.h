
/* Crop growth */
extern float GetDevelopmentStage();
extern float DailyTotalAssimilation();
extern float DyingLeaves();
extern float InstantAssimilation();
extern float LeaveGrowth(float LAIExp, float Newleaves);
extern float LeaveAreaIndex();
extern float Correct(float GrossAssimilation);
extern int Astro();
extern int CalcPenman();
extern void Clean();


/* Nutrients */
extern void NutrientsInitialize();
extern void NutrientsIntegration();
extern float NutritionINDX();
extern void NutrientLoss();
extern void NutrientMax();
extern void NutrientPartioning();
extern void NutrientRates();
extern void NutrientOptimum();
extern void NutrientDemand();
extern void SoilNutrients();
extern void Translocation();
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
extern void WatBalInitialize();
extern void WatBalRateCalulation();
extern void WatBalIntegration();