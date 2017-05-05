
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
extern void InitializeNutrients();
extern void IntegrationNutrients();
extern void NutritionINDX();
extern void NutrientLoss();
extern void NutrientMax();
extern void NutrientPartioning();
extern void NutrientRates();
extern void NutrientOptimum();
extern void NutrientDemand();
extern void NutrientStress();
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
extern void InitializeWatBal();
extern void RateCalulationWatBal();
extern void IntegrationWatBal();