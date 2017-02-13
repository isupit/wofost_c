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
    if (WatBal.ct.Airducts) 
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
    WatBal.st.MoistureLOW  = limit (0., WatBal.ct.MoistureSAT
        *(Crop.MaxRootingDepth-Crop.rootdepth), 
        InitSoilMoisture + Crop.MaxRootingDepth*WatBal.ct.MoistureWP - 
            WatBal.st.RootZoneMoisture);
   
    
    /*  all summation variables of the water balance are set at zero. */
    WatBal.st.Transpiration   = 0.;
    WatBal.st.EvapSoil        = 0.;
    Watbal.st.EvapWater       = 0.;
    TotRunOff          = 0.;
    ToTRain            = 0.;
    TotWaterRootGrowth = 0.;
    Watbal.st.Infilatration   = 0.;
    TotIrrigation      = 0.;
    TotSoilMSeason     = 0.;
    WatBal.st.Infiltration    = 0.
    WatBal.st.Percolation     = 0.;
    WatBal.st.Loss            = 0.;

    return WatBal;
}


Soil WatBalRateCalulation() {
    /* if surface storage > 1 cm */
    if (WatBal.st.SurfaceStorage > 1.) 
        WatBal.rt.WaterEvap = Evtra.MaxEvapWater;
    else {
        if (WatBal.rt.Infiltration >= 1) {
            WatBal.rt.SoilEvap = Evtra.MaxEvapSoil;
            DaysSinceLastRain = 1.;
        }
        else {
            DaysSinceLastRain++;
            MaxSoilEvap = Evtra.MaxEvapSoil*(sqrt(DaysSinceLastRain) - 
                    sqrt(DaysSinceLastRain -1));
            WatBal.rt.SoilEvap = min(Evtra.MaxEvapSoil, MaxSoilEvap + 
                    WatBal.rt.Infiltration);
        }
    }
    
    /* Preliminary infiltration rate */
    if (WatBal.st.SurfaceStorage <= 0.1) {
        /* without surface storage */
        if (FlagFixInfiltration) WatBal.rt.Infiltration = 
                (1.-NotInfiltrating*Afgen(NotInfTB,Rain[Day])) * Rain[Day] + 
                WatBal.rt.Irrigation + WatBal.st.SurfaceStorage/Delta;
        else
            WatBal.rt.Infiltration = (1.-NotInfiltrating) * Rain[Day] + 
                WatBal.rt.Irrigation + WatBal.st.SurfaceStorage/Delta;
    }
    else {
        /* surface storage, infiltration limited by maximum percolation */
        /* rate root zone*/
        Available = WatBal.st.SurfaceStorage + (Rain[Day] * (1.-NotInfiltrating)
                + WatBal.rt.Irrigation - SoilEvap) * Delta;
        WatBal.rt.Infiltration = min(WatBal.ct.MaxPercolRTZ*Delta, 
                Available)/Delta;
    }
    
    /* equilibrium amount of soil moisture in rooted zone*/
    WaterEq = WatBal.ct.MoistureFC * Crop.rootdepth;
    
    /* percolation from rooted zone to subsoil equals amount of   */
    /* excess moisture in rooted zone (not to exceed conductivity)*/
    PERC1 = limit(0., WatBal.ct.MaxPercolRTZ, 
            (WatBal.st.RootZoneMoisture - WaterEq)/Delta - 
            WatBal.rt.Transpiration - SoilEvap);
    
    /* loss of water at the lower end of the maximum root zone */
    /* equilibrium amount of soil moisture below rooted zone   */
    WELOW = WatBal.ct.MoistureFC * (Crop.MaxRootingDepth - Crop.rootdepth);
    WatBal.rt.Loss  = limit (0., WatBal.ct.MaxPercolSubS, 
            (WatBal.st.MoistureLOW - WELOW)/Delta + PERC1);
    
    /* for rice water losses are limited to K0/20 */
    if (WatBal.ct.Airducts) 
        WatBal.rt.Loss = min (WatBal.rt.Loss, 0.05*WatBal.ct.K0);
    
    /* percolation not to exceed uptake capacity of subsoil */
    PERC2 = ((Crop.MaxRootingDepth - Crop.rootdepth) * WatBal.ct.MoistureSAT - 
            WatBal.st.MoistureLOW) / Delta + WatBal.rt.Loss;
    WatBal.rt.Percolation = min (PERC1,PERC2);
    
    /* adjustment of infiltration rate */
    WatBal.rt.Infiltration = min (WatBal.rt.Infiltration,
            (WatBal.ct.MoistureSAT - SoilMoisture)*Crop.rootdepth/Delta + 
            WatBal.rt.Transpiration + WatBal.rt.SoilEvap + WatBal.rt.Percolation);
            
    /* rates of change in amounts of moisture W and WLOW */
    WatBal.rt.RootZoneMoisture = -WatBal.rt.Transpiration - WatBal.rt.SoilEvap -  
            WatBal.rt.Percolation + WatBal.rt.Infiltration;
    WatBal.rt.MoistureLOW = WatBal.rt.Percolation - WatBal.rt.Loss;
                   
    return WatBal;
}

Soil WatBalIntegration() {
    
    WatBal.st.Transpiration = WatBal.st.Transpiration + WatBal.rt.Transpiration;
    WatBal.st.EvapWater = WatBal.st.EvapWater + WatBal.rt.EvapWater;
    WatBal.st.EvapSoil  = WatBal.st.EvapSoil +  WatBal.rt.EvapSoil;
    
    WatBal.st.Infiltration = WatBal.st.Infiltration + WatBal.rt.Infiltration;
    WatBal.st.Rain = WatBal.st.Rain + Rain[day];
    
    /* surface storage and runoff */
    SSPRE = SS + (RAIN+RIRR-EVW-RIN)*Delta;
    WatBal.st.SurfaceStorage    = min (SSPRE, SSMAX);
    WatBal.st.Runoff   = WatBal.st.Runoff + (SSPRE-WatBal.st.SurfaceStorage);
    
    return WatBal;
}