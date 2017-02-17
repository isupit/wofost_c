#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dynamic.h"
#include "mmla.h"
#include "penman.h"
#include "wofost.h"



void WatBalInitialize()
{
    /* InitSoilMoisture is the initial (at emergence) amount of water in */
    /* excess of wilting point, but not exceeding field capacity    cm   */
    
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
        WatBal.st.Moisture = limit(WatBal.ct.MoistureWP, WatBal.st.Moisture, 
            WatBal.ct.MoistureWP + InitSoilMoisture/Crop.RootDepth);
    
    /* Initial moisture amount in rooted zone */
    WatBal.st.RootZoneMoisture = WatBal.st.Moisture * Crop.RootDepth;
    
    /*  Soil evaporation, days since last rain */
    WatBal.DaysSinceLastRain = 1.;
    if (WatBal.st.Moisture <= (WatBal.ct.MoistureWP + 
            0.5*(WatBal.ct.MoistureFC - WatBal.ct.MoistureWP))) 
            WatBal.DaysSinceLastRain = 5.;
    
    /* Moisture amount between rooted zone and max.rooting depth */
    WatBal.st.MoistureLOW  = limit (0., WatBal.ct.MoistureSAT
        *(Crop.MaxRootingDepth - Crop.RootDepth), 
        Site.InitSoilMoisture + Crop.MaxRootingDepth * WatBal.ct.MoistureWP - 
            WatBal.st.RootZoneMoisture);
   
}

void WatBalRateCalulation() {
   
    float Available;
    float CMaxSoilEvap;
    float Perc1, Perc2;
    float WaterEq;
    float WELOW;
    
    /* if surface storage > 1 cm */
    if (WatBal.st.SurfaceStorage > 1.) {
        WatBal.rt.EvapWater = Evtra.MaxEvapWater;
    }
    else {
        if (WatBal.rt.Infiltration >= 1.) {
            WatBal.rt.EvapSoil = Evtra.MaxEvapSoil;
            WatBal.DaysSinceLastRain = 1.;
        }
        else {
            WatBal.DaysSinceLastRain++;
            CMaxSoilEvap = Evtra.MaxEvapSoil*(sqrt(DaysSinceLastRain) - 
                    sqrt(WatBal.DaysSinceLastRain -1));
            WatBal.rt.EvapSoil = min(Evtra.MaxEvapSoil, CMaxSoilEvap + 
                    WatBal.rt.Infiltration);
        }
    }
    
    /* Preliminary infiltration rate */
    if (WatBal.st.SurfaceStorage <= 0.1) {
        /* without surface storage */
        if (Site.InfRainDependent) WatBal.rt.Infiltration = 
               (1.-Site.NotInfiltrating * Afgen(Site.NotInfTB,Rain[Day])) * 
               Rain[Day] + WatBal.rt.Irrigation + WatBal.st.SurfaceStorage/Step;
        else
            WatBal.rt.Infiltration = (1.-Site.NotInfiltrating) * Rain[Day] + 
                WatBal.rt.Irrigation + WatBal.st.SurfaceStorage/Step;
    }
    else {
        /* surface storage, infiltration limited by maximum percolation */
        /* rate root zone*/
        Available = WatBal.st.SurfaceStorage + (Rain[Day] * 
                (1.-Site.NotInfiltrating) + WatBal.rt.Irrigation 
                 - WatBal.rt.EvapSoil) * Step;
        WatBal.rt.Infiltration = min(WatBal.ct.MaxPercolRTZ * Step, 
                Available)/Step;
    }
    
    /* equilibrium amount of soil moisture in rooted zone*/
    WaterEq = WatBal.ct.MoistureFC * Crop.RootDepth;
    
    /* percolation from rooted zone to subsoil equals amount of   */
    /* excess moisture in rooted zone (not to exceed conductivity)*/
    Perc1 = limit(0., WatBal.ct.MaxPercolRTZ, 
            (WatBal.st.RootZoneMoisture - WaterEq)/Step - 
            WatBal.rt.Transpiration - WatBal.rt.EvapSoil);
    
    /* loss of water at the lower end of the maximum root zone */
    /* equilibrium amount of soil moisture below rooted zone   */
    WELOW = WatBal.ct.MoistureFC * (Crop.MaxRootingDepth - Crop.RootDepth);
    WatBal.rt.Loss  = limit (0., WatBal.ct.MaxPercolSubS, 
            (WatBal.st.MoistureLOW - WELOW)/Step + Perc1);
    
    /* for rice water losses are limited to K0/20 */
    if (Airducts) 
        WatBal.rt.Loss = min(WatBal.rt.Loss, 0.05*WatBal.ct.K0);
    
    /* percolation not to exceed uptake capacity of subsoil */
    Perc2 = ((Crop.MaxRootingDepth - Crop.RootDepth) * WatBal.ct.MoistureSAT - 
            WatBal.st.MoistureLOW) / Step + WatBal.rt.Loss;
    WatBal.rt.Percolation = min(Perc1, Perc2);
    
    /* adjustment of the infiltration rate */
    WatBal.rt.Infiltration = min(WatBal.rt.Infiltration,
          (WatBal.ct.MoistureSAT - WatBal.st.Moisture)*Crop.RootDepth/Step + 
          WatBal.rt.Transpiration + WatBal.rt.EvapSoil + WatBal.rt.Percolation);
            
    /* rates of change in amounts of moisture W and WLOW */
    WatBal.rt.RootZoneMoisture = -WatBal.rt.Transpiration - WatBal.rt.EvapSoil -  
            WatBal.rt.Percolation + WatBal.rt.Infiltration;
    WatBal.rt.MoistureLOW = WatBal.rt.Percolation - WatBal.rt.Loss;
                  
}

void WatBalIntegration() {
    
    float PreSurfaceStorage;
    float WaterRootExt;
    
    WatBal.st.Transpiration  +=  WatBal.rt.Transpiration;
    WatBal.st.EvapWater += WatBal.rt.EvapWater;
    WatBal.st.EvapSoil  += WatBal.rt.EvapSoil;
    
    WatBal.st.Rain += Rain[Day];
    WatBal.st.Infiltration += WatBal.rt.Infiltration;
    WatBal.st.Irrigation += WatBal.rt.Irrigation;
    
    /* surface storage and runoff */
    PreSurfaceStorage = WatBal.st.SurfaceStorage + (Rain[Day] + 
            WatBal.rt.Irrigation - WatBal.rt.EvapWater - 
            WatBal.rt.Infiltration)*Step;
    WatBal.st.SurfaceStorage = min(PreSurfaceStorage, 
            Site.MaxSurfaceStorage);
    WatBal.st.Runoff += PreSurfaceStorage - WatBal.st.SurfaceStorage;
    
    /* amount of water in rooted zone */
    WatBal.st.RootZoneMoisture += WatBal.rt.RootZoneMoisture*Step;
    if (WatBal.st.RootZoneMoisture < 0.) {
           WatBal.st.EvapSoil += WatBal.st.RootZoneMoisture;
            WatBal.st.RootZoneMoisture = 0.;
    }
    
    /* total percolation and loss of water by deep leaching */
    WatBal.st.Percolation += WatBal.rt.Percolation*Step;
    WatBal.st.Loss += WatBal.rt.Loss*Step;     
    
    WatBal.st.MoistureLOW += WatBal.rt.MoistureLOW;
   
    /* change of root zone subsystem boundary                  */
    /* calculation of amount of soil moisture in new root zone */
    if (Crop.RootDepth-Crop.RootDepth_prev > 0.001) {
        
        /* water added to root zone by root growth, in cm   */
        WaterRootExt = WatBal.st.MoistureLOW *
                (Crop.RootDepth-Crop.RootDepth_prev) / 
                (Crop.MaxRootingDepth -Crop.RootDepth_prev);
        WatBal.st.MoistureLOW -= WaterRootExt;

        /* total water addition to root zone by root growth  */
        WatBal.st.WaterRootExt += WaterRootExt;

        /* amount of soil moisture in extended root zone */
        WatBal.st.RootZoneMoisture += WaterRootExt;
    }

    /* mean soil moisture content in rooted zone */
    WatBal.st.Moisture = WatBal.st.RootZoneMoisture/Crop.RootDepth;
  
   
}