

extern float GetDevelopmentStage();
extern float DailyTotalAssimilation();
extern float DyingLeaves();
extern float InstantAssimilation();
extern float LeaveGrowth(float LAIExp, float Newleaves);
extern float LeaveAreaIndex();
extern float Correct(float GrossAssimilation);
extern int FillCropVariables();
extern int FillSiteVariables();
extern int FillAfgenTables();
extern int GetAfgenTables();
extern int GetCropData();
extern int GetMeteoData();
extern int GetSoilData();
extern int GetSiteData();
extern int GetManagement();
extern int Astro();
extern int CalcPenman();
extern void WatBalInitialize();
extern void WatBalRateCalulation();
extern void WatBalIntegration();
extern void Clean();
