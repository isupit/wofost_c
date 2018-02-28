/* original: Penman.for from I.G.A.M. Noy and C.A. van Diepen, */
/* van Kraalingen, and Allard de Wit, Sep 2011                 */

#include <stdio.h>
#include <math.h>
#include "penman.h"
#include "wofost.h"
#include "extern.h"

/* ---------------------------------------------------------------*/
/*  function sweaf()                                              */
/*  Purpose: Calculates the Soil Water Easily Available Fraction  */
/* ---------------------------------------------------------------*/ 
float sweaf(){
    float sweaf; 
    sweaf = 1./(0.76 + 1.5 * Penman.ET0) - (5.-Crop->prm.CropGroupNumber ) * 0.10;
    if  (Crop->prm.CropGroupNumber < 3.) {
        sweaf = sweaf + (Penman.ET0 - 0.6)/(Crop->prm.CropGroupNumber *
                (Crop->prm.CropGroupNumber + 3.));
    }
    return limit(0.10, 0.95, sweaf);

}
    
/* -----------------------------------------------------------------*/
/*  function EvapTra()                                              */
/*  Purpose: Calculates the water stress and the transpiration rate */
/* -----------------------------------------------------------------*/     
void EvapTra() {   
    float CriticalSoilMoisture;
    float KDiffuse;
    float MaxReductionOxygenStress;
    float MoistureStress;
    float OxygenStress;
    float SoilMoistureAeration;
    float SoilWatDepletion;
    
    KDiffuse = Afgen(Crop->prm.KDiffuseTb, &(Crop->DevelopmentStage));      
    Evtra.MaxEvapWater = Penman.E0 * exp(-0.75 * KDiffuse * Crop->st.LAI);
    Evtra.MaxEvapSoil  = max(0., Penman.ES0 * exp(-0.75 * KDiffuse * Crop->st.LAI));
    Evtra.MaxTranspiration = max(0.0001,  
                             Penman.ET0 * Afgen(Crop->prm.CO2TRATB, &CO2) *
                             (1.-exp(-0.75 * KDiffuse * Crop->st.LAI)));
       
    SoilWatDepletion = sweaf();
    CriticalSoilMoisture = (1. - SoilWatDepletion)*
            (WatBal->ct.MoistureFC-WatBal->ct.MoistureWP) + WatBal->ct.MoistureWP;
    
    /* Transpiration reduction in case of water shortage */
    MoistureStress = limit(0.,1.,(WatBal->st.Moisture - WatBal->ct.MoistureWP)/
            (CriticalSoilMoisture - WatBal->ct.MoistureWP));
    
    if (!Crop->prm.Airducts) 
    {
        /* Critical soil moisture content for aeration */
        SoilMoistureAeration = WatBal->ct.MoistureSAT - WatBal->ct.CriticalSoilAirC;
        
        /* Count days since start oxygen shortage (up to 4 days) */
        if (WatBal->SoilMoisture >= SoilMoistureAeration) {
            Crop->DaysOxygenStress = min(Crop->DaysOxygenStress++, 4.);
        }
        else 
        {
            Crop->DaysOxygenStress = 0.;
        }
        
        /* Maximum reduction reached after 4 days */
        MaxReductionOxygenStress = limit (0.,1.,(WatBal->ct.MoistureSAT - WatBal->SoilMoisture)/
                (WatBal->ct.MoistureSAT - SoilMoistureAeration));
        
        OxygenStress   = MaxReductionOxygenStress + 
                (1.-Crop->DaysOxygenStress/4.)*(1.-MaxReductionOxygenStress);        
    }
    else 
    {
        OxygenStress = 1.;
    }
    
    WatBal->WaterStress = MoistureStress * OxygenStress;
    //WatBal->WaterStress = 1.;
     
    WatBal->rt.Transpiration = WatBal->WaterStress * Evtra.MaxTranspiration;
    }
