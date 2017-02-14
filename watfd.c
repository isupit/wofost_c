#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "penman.h"
#include "wofost.h"


Soil WatBalInitialize(MaxSurfaceStorage, InitSurfaceStorage, SoilMoisture)
{
    Soil WatBal;
    float MaxSurfaceStorage;
    float InitSurfaceStorage;
    float InitSoilMoisture;  /* initial (at emergence) amount of water in excess of wilting point, but not exceeding field capacity    cm */
    float SoilMoisture;
    
    /* Set the initial rates to zero */
    
    WatBal.rt.EvapWater         = 0.;
    WatBal.rt.EvapSoil          = 0.;   
    WatBal.rt.Infiltration      = 0.;
    WatBal.rt.Irrigation        = 0.;
    WatBal.rt.Loss              = 0.;
    WatBal.rt.Moisture          = 0.;
    WatBal.rt.MoistureLOW       = 0.;
    WatBal.rt.Percolation       = 0.;
    WatBal.rt.RootZoneMoisture  = 0.;
    WatBal.rt.Runoff            = 0.;
    WatBal.rt.SurfaceStorage    = 0.;
    WatBal.rt.Transpiration     = 0.;
    WatBal.rt.WaterRootExt      = 0.;
    
    /*  set state variables of the water balance are set to zero. */   
    WatBal.st.EvapWater         = 0.;
    WatBal.st.EvapSoil          = 0.;
    WatBal.st.Infiltration      = 0.;
    WatBal.st.Irrigation        = 0.;
    WatBal.st.Loss              = 0.;
    WatBal.st.Moisture          = 0.;
    WatBal.st.MoistureLOW       = 0.;
    WatBal.st.Percolation       = 0.;
    WatBal.st.Rain              = 0.;
    WatBal.st.RootZoneMoisture  = 0.;
    WatBal.st.Runoff            = 0.;
    WatBal.st.SurfaceStorage    = 0.;
    WatBal.st.Transpiration     = 0.;
    WatBal.st.WaterRootExt      = 0.;
    
    /* Check initial soil moisture. It cannot be larger than the    */
    /* soil moisture SoilMoistureSAT or smaller than SoilMoistureWP */
    if (WatBal.st.Moisture <= WatBal.ct.MoistureWP)  
            WatBal.st.Moisture = WatBal.ct.MoistureWP;
    if (WatBal.st.Moisture >= WatBal.ct.MoistureSAT) 
            WatBal.st.Moisture = WatBal.ct.MoistureSAT;
    
    /* initial soil moisture for a rice crop */
    if (Airducts) WatBal.st.Moisture = WatBal.ct.MoistureSAT; 
    SoilMoisture = limit(WatBal.ct.MoistureWP, WatBal.st.Moisture, 
            WatBal.ct.MoistureWP + InitSoilMoisture/Crop.rootdepth);
    
    /* Initial moisture amount in rooted zone */
    WatBal.st.RootZoneMoisture = SoilMoisture*Crop.rootdepth;
    
    /*  Soil evaporation, days since last rain */
    WatBal.DaysSinceLastRain = 1.;
    if (SoilMoisture <= (WatBal.ct.MoistureWP + 
            0.5*(WatBal.ct.MoistureFC-WatBal.ct.MoistureWP))) 
            WatBal.DaysSinceLastRain=5.;
    
    /* Moisture amount between rooted zone and max.rooting depth */
    WatBal.st.MoistureLOW  = limit (0., WatBal.ct.MoistureSAT
        *(Crop.MaxRootingDepth-Crop.rootdepth), 
        InitSoilMoisture + Crop.MaxRootingDepth*WatBal.ct.MoistureWP - 
            WatBal.st.RootZoneMoisture);
    
    return WatBal;
}


Soil WatBalRateCalulation() {
   
    float Available;
    float MaxSoilEvap;
    float Perc1, Perc2;
    float WaterEq;
    float WELOW;
    
    /* if surface storage > 1 cm */
    if (WatBal.st.SurfaceStorage > 1.) 
        WatBal.rt.WaterEvap = Evtra.MaxEvapWater;
    else {
        if (WatBal.rt.Infiltration >= 1) {
            WatBal.rt.SoilEvap = Evtra.MaxEvapSoil;
            WatBal.DaysSinceLastRain = 1.;
        }
        else {
            WatBal.DaysSinceLastRain++;
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
                WatBal.rt.Irrigation + WatBal.st.SurfaceStorage/Step;
        else
            WatBal.rt.Infiltration = (1.-NotInfiltrating) * Rain[Day] + 
                WatBal.rt.Irrigation + WatBal.st.SurfaceStorage/Step;
    }
    else {
        /* surface storage, infiltration limited by maximum percolation */
        /* rate root zone*/
        Available = WatBal.st.SurfaceStorage + (Rain[Day] * (1.-NotInfiltrating)
                + WatBal.rt.Irrigation - SoilEvap) * Step;
        WatBal.rt.Infiltration = min(WatBal.ct.MaxPercolRTZ*Step, 
                Available)/Step;
    }
    
    /* equilibrium amount of soil moisture in rooted zone*/
    WaterEq = WatBal.ct.MoistureFC * Crop.RootDepth;
    
    /* percolation from rooted zone to subsoil equals amount of   */
    /* excess moisture in rooted zone (not to exceed conductivity)*/
    Perc1 = limit(0., WatBal.ct.MaxPercolRTZ, 
            (WatBal.st.RootZoneMoisture - WaterEq)/Step - 
            WatBal.rt.Transpiration - SoilEvap);
    
    /* loss of water at the lower end of the maximum root zone */
    /* equilibrium amount of soil moisture below rooted zone   */
    WELOW = WatBal.ct.MoistureFC * (Crop.MaxRootingDepth - Crop.RootDepth);
    WatBal.rt.Loss  = limit (0., WatBal.ct.MaxPercolSubS, 
            (WatBal.st.MoistureLOW - WELOW)/Step + Perc1);
    
    /* for rice water losses are limited to K0/20 */
    if (Airducts) 
        WatBal.rt.Loss = min (WatBal.rt.Loss, 0.05*WatBal.ct.K0);
    
    /* percolation not to exceed uptake capacity of subsoil */
    Perc2 = ((Crop.MaxRootingDepth - Crop.RootDepth) * WatBal.ct.MoistureSAT - 
            WatBal.st.MoistureLOW) / Step + WatBal.rt.Loss;
    WatBal.rt.Percolation = min (Perc1,Perc2);
    
    /* adjustment of infiltration rate */
    WatBal.rt.Infiltration = min (WatBal.rt.Infiltration,
            (WatBal.ct.MoistureSAT - SoilMoisture)*Crop.RootDepth/Step + 
            WatBal.rt.Transpiration + WatBal.rt.SoilEvap + WatBal.rt.Percolation);
            
    /* rates of change in amounts of moisture W and WLOW */
    WatBal.rt.RootZoneMoisture = -WatBal.rt.Transpiration - WatBal.rt.SoilEvap -  
            WatBal.rt.Percolation + WatBal.rt.Infiltration;
    WatBal.rt.MoistureLOW = WatBal.rt.Percolation - WatBal.rt.Loss;
                   
    return WatBal;
}

Soil WatBalIntegration() {
    
    float PreSurfaceStorage;
    float WaterRootExt;
    
    WatBal.st.Transpiration = WatBal.st.Transpiration + WatBal.rt.Transpiration;
    WatBal.st.EvapWater = WatBal.st.EvapWater + WatBal.rt.EvapWater;
    WatBal.st.EvapSoil  = WatBal.st.EvapSoil +  WatBal.rt.EvapSoil;
    
    WatBal.st.Infiltration = WatBal.st.Infiltration + WatBal.rt.Infiltration;
    WatBal.st.Rain = WatBal.st.Rain + Rain[day];
    
    /* surface storage and runoff */
    PreSurfaceStorage = WatBal.st.SurfaceStorage + (Rain[Day] + 
            WatBal.rt.Irrigation - WatBal.rt.EvapWater - 
            WatBal.rt.Infiltration)*Step;
    WatBal.st.SurfaceStorage = min(PreSurfaceStorage, 
            WatBal.ct.MaxSurfaceStorage);
    WatBal.st.Runoff = WatBal.st.Runoff + 
            (PreSurfaceStorage - WatBal.st.SurfaceStorage);
    
    /* amount of water in rooted zone */
    WatBal.st.RootZoneMoisture = WatBal.st.RootZoneMoisture + 
            WatBal.rt.RootZoneMoisture*Step;
    if (WatBal.st.RootZoneMoisture < 0.) {
           WatBal.st.EvapSoil = WatBal.st.EvapSoil + WatBal.st.RootZoneMoisture
            WatBal.st.RootZoneMoisture = 0.;
    }
    
    /* total percolation and loss of water by deep leaching */
    WatBal.st.Percolation = WatBal.st.Percolation + WatBal.rt.Percolation*Step;
    WatBal.st.Loss = WatBal.st.Loss + WatBal.rt.Loss*Step;     
   
    /* change of root zone subsystem boundary                  */
    /* calculation of amount of soil moisture in new root zone */
    if (Crop.RootDepth-Crop.RootDepth_prev > 0.001) {
        
        /* water added to root zone by root growth, in cm   */
        WaterRootExt = WatBal.st.MoistureLOW *
                (Crop.RootDepth-Crop.RootDepth_prev) / 
                (Crop.MaxRootingDepth -Crop.RootDepth_prev);
        WatBal.st.MoistureLOW = WatBal.st.MoistureLOW - WaterRootExt;

        /* total water addition to root zone by root growth  */
        WatBal.st.WaterRootExt = WatBal.st.WaterRootExt + WaterRootExt;

        /* amount of soil moisture in extended root zone */
        WatBal.st.RootZoneMoisture = WatBal.st.RootZoneMoisture + WaterRootExt;
    }

    /* mean soil moisture content in rooted zone */
    WatBal.st.Moisture = WatBal.st.RootZoneMoisture/Crop.RootDepth;
  
    
    return WatBal;
}