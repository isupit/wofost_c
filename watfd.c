#include <stdio.h>
#include <stdlib.h>
#include "penman.h"
#include "wofost.h"


Soil WatBalInitialize(MaxSurfaceStorage, InitSurfaceStorage, SoilMoisture)
{
    Soil WatBal;
    float MaxSurfaceStorage;
    float InitSurfaceStorage;
    float InitSoilMoisture;  /* initial (at emergence) amount of water in excess of wilting point, but not exceeding field capacity    cm */
    
    /* Set the initial rates to zero */
    WatBal.rt.Transpiration     = 0.;
    WatBal.rt.EvapWater         = 0.;
    WatBal.rt.EvapSoil          = 0.;   
    WatBal.rt.Rain              = 0.;
    WatBal.rt.Infiltration      = 0.;
    WatBal.rt.Percolation       = 0.;
    WatBal.rt.Irrigation        = 0.;
    WatBal.rt.WaterRootGrowth   = 0.;
    WatBal.rt.Loss              = 0.;
    
    /* Check initial soil moisture. It cannot be larger than the    */
    /* soil moisture SoilMoistureSAT or smaller than SoilMoistureWP */
    if (WatBal.st.Moisture <= WatBal.ct.MoistureWP)  
            WatBal.st.Moisture = WatBal.ct.MoistureWP;
    if (WatBal.st.Moisture >= WatBal.ct.MoistureSAT) 
            WatBal.st.Moisture = WatBal.ct.MoistureSAT;
    
    /* initial soil moisture for a rice crop */
    if (WatBal.ct.Airducts == 1) 
            WatBal.st.Moisture = WatBal.ct.MoistureSAT; 
    SoilMoisture = limit(WatBal.ct.MoistureWP, WatBal.st.Moisture, 
            WatBal.ct.MoistureWP + InitSoilMoisture/Crop.rootdepth);
    
    /* Initial moisture amount in rooted zone */
    WatBal.st.RootZoneMoisture = SoilMoisture*Crop.rootdepth;
    
    /*  Soil evaporation, days since last rain */
    DaysSinceLastRain = 1.;
    if (SoilMoisture <= (WatBal.ct.MoistureWP + 
            0.5*(WatBal.ct.MoistureFC-WatBal.ct.MoistureWP))) 
            DaysSinceLastRain=5.;
    
    
    /* Moisture amount between rooted zone and max.rooting depth */
    MoistureLOW  = limit (0., WatBal.ct.MoistureSAT
        *(Crop.MaxRootingDepth-Crop.rootdepth), 
        InitSoilMoisture + Crop.MaxRootingDepth*WatBal.ct.MoistureWP - WatBal.st.RootZoneMoisture);
   
    
    /*  all summation variables of the water balance are set at zero. */
    TotTranspiration   = 0.;
    TotEvapSoil        = 0.;
    TotEvapWater       = 0.;
    TotRunOff          = 0.;
    ToTRain            = 0.;
    TotWaterRootGrowth = 0.;
    TotInfilatration   = 0.;
    TotIrrigation      = 0.;
    TotSoilMSeason     = 0.;
    TotPercolation     = 0.;
    TotLoss            = 0.;

    return WatBal;
}


Soil WatBalRateCalulation() {
    
    
    /* if surface storage > 1 cm */
    if (WatBal.st.SurfaceStorage > 1.) 
        WatBal.rt.EvaporationWater = Evtra.MaxEvapWater;
    else {
        if (WatBal.rt.Infiltration >= 1) {
            SoilEvap = Evtra.MaxEvapSoil;
            DaysSinceLastRain = 1.;
        }
        else {
            DaysSinceLastRain++;
            MaxSoilEvap = Evtra.MaxEvapSoil*(sqrt(DaysSinceLastRain) - sqrt(DaysSinceLastRain -1));
            SoilEvap = min(Evtra.MaxEvapSoil, MaxSoilEvap + WatBal.rt.Infiltration);
        }
    }
    return WatBal;
}