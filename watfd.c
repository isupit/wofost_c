#include <stdio.h>
#include <stdlib.h>
#include "wofost.h"


Soil WatBalInitialize()
{
    Soil WatBal;
    
    /* Set the initial rates to zero */
    WatBal.rt.Transpiration     = 0.;
    WatBal.rt.EvaporationWater  = 0.;
    WatBal.rt.EvaporationSoil   = 0.;   
    WatBal.rt.Rain              = 0.;
    WatBal.rt.Infiltration      = 0.;
    WatBal.rt.Percolation       = 0.;
    WatBal.rt.Irrigation        = 0.;
    WatBal.rt.IncreaseWaterRootGrowth = 0.;
    WatBal.rt.Loss              = 0.;
    
    
    /* if surface storage > 1 cm */
    if (WatBal.st.SurfaceStorage > 1.) WatBal.rt.EvaporationWater = WatBal.ct.EvapWaterMax;
    
    
    /* Check initial soil moisture. It cannot be larger than the    */
    /* soil moisture SoilMoistureSAT or smaller than SoilMoistureWP */
    
    if (WatBal.st.Moisture <= WatBal.ct.MoistureWP)  WatBal.st.Moisture = WatBal.ct.MoistureWP;
    if (WatBal.st.Moisture >= WatBal.ct.MoistureSAT) WatBal.st.Moisture = WatBal.ct.MoistureSAT;
    
    /* initial soil moisture for a rice crop */
    if (WatBal.ct.Airducts == 1) WatBal.st.Moisture = WatBal.ct.MoistureSAT; 
    SoilMoisture = limit(SoilMoistureWP, WatBal.st.Moisture, WatBal.ct.MoistureWP +
            AvailableSoilM/Crop.rootdepth);
    
    /* Initial moisture amount in rooted zone */
    AvailRootZoneMoisture = SoilMoisture*Crop.rootdepth;
    //InitAvailMoisture     = AvailRootZoneMoisture;
    
    /*  Soil evaporation, days since last rain */
    DaysSinceLastRain = 1.;
    if (SoilMoisture <= (SoilMoistureWP+0.5*(SoilMoistureFC-SoilMoistureWP))) 
        DaysSinceLastRain=5.;
    
    
    /* Moisture amount between rooted zone and max.rooting depth */
    MoistureLOW  = limit (0., SoilMoistureSAT*(MaxRootingDepth-Crop.rootdepth), 
            AvailableSoilM + MaxRootingDepth*SoilMoistureWP - 
   
    
    /*  all summation variables of the water balance are set at zero. */
    TRAT   = 0.;
    EVST   = 0.;
    EVWT   = 0.;
    TSR    = 0.;
    RAINT  = 0.;
    WDRT   = 0.;
    TOTINF = 0.;
    TOTIRR = 0.;
    SUMSM  = 0.;
    PERCT  = 0.;
    LOSST  = 0.;

    return WatBal;
}