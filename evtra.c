/* original: Penman.for from I.G.A.M. Noy and C.A. van Diepen, */
/* van Kraalingen, and Allard de Wit, Sep 2011                 */

#include <stdio.h>
#include <math.h>
#include "penman.h"
#include "wofost.h"
#include "mmla.h"
#include "dynamic.h"

extern float LeaveAreaIndex();

float sweaf(){
    float sweaf; 
    sweaf = 1./(0.76 + 1.5 * Penman.ET0) - (5.-CropGroupNumber ) * 0.10;
    if  (CropGroupNumber < 3.) {
        sweaf = sweaf + (Penman.ET0 - 0.6)/(CropGroupNumber *
                (CropGroupNumber + 3.));
    }
    return limit(0.10, 0.95, sweaf);
}

void EvapTra() {   
    float CriticalSoilMoisture;
    float KDiffuse;
    float Lai;
    float MaxReductionOxygenStress;
    float MoistureStress;
    float OxygenStress;
    float SoilMoistureAeration;
    float SoilWatDepletion;

    Lai = LeaveAreaIndex();
    
    KDiffuse = Afgen(KDiffuseTb, &DevelopmentStage);      
    Evtra.MaxEvapWater = Penman.E0 * exp(-0.75 * KDiffuse * Lai);
    Evtra.MaxEvapSoil  = max(0., Penman.ES0 * exp(-0.75 * KDiffuse * Lai));
    Evtra.MaxTranspiration = max(0.0001,  
            Penman.ET0 * (1.-exp(-0.75 * KDiffuse * Lai)));
       
    SoilWatDepletion = sweaf(CropGroupNumber);
    CriticalSoilMoisture = (1. - SoilWatDepletion)*
            (WatBal.ct.MoistureFC-WatBal.ct.MoistureWP) + WatBal.ct.MoistureWP;
    
    /* Transpiration reduction in case of water shortage */
    MoistureStress = limit(0.,1.,(WatBal.st.Moisture - WatBal.ct.MoistureWP)/
            (CriticalSoilMoisture - WatBal.ct.MoistureWP));
    
    if (!Airducts) {
        /* Critical soil moisture content for aeration */
        SoilMoistureAeration = WatBal.ct.MoistureSAT - WatBal.ct.CriticalSoilAirC;
        
        /* Count days since start oxygen shortage (up to 4 days)*/
        if (SoilMoisture >= SoilMoistureAeration) {
            DaysOxygenStress = min(DaysOxygenStress++, 4.);
        }
        else {
            DaysOxygenStress = 0.;
        }
        
        /* Maximum reduction reached after 4 days */
        MaxReductionOxygenStress = limit (0.,1.,(WatBal.ct.MoistureSAT-SoilMoisture)/
                (WatBal.ct.MoistureSAT - SoilMoistureAeration));
        OxygenStress   = MaxReductionOxygenStress + 
                (1.-DaysOxygenStress/4.)*(1.-MaxReductionOxygenStress);        
    }
    else 
    {
        OxygenStress = 1.;
    }
    
    WatBal.WaterStress = MoistureStress * OxygenStress;
     
    WatBal.rt.Transpiration = WatBal.WaterStress * Evtra.MaxTranspiration;
    
}