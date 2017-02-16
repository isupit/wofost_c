

extern float GetDevelopmentStage();
extern float DailyTotalAssimilation();
extern float DyingLeaves();
extern float InstantAssimilation();
extern Green *LeaveGrowth(float LAIExp, float Newleaves, float *GrowthExpLAI);
extern float LeaveAreaIndex();
extern float Correct(float GrossAssimilation);
extern int FillVariables();
extern int FillAfgenTables();
extern int GetAfgenTables();
extern int GetCropData();
extern int GetMeteoData();
extern int GetSoilData();
extern int GetSiteData();
extern int Astro();
extern int CalcPenman();
extern Soil WatBalInitialize();
extern Soil WatBalRateCalulation();
extern Soil WatBalIntegration();
