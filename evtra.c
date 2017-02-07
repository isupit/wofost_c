/* original: Penman.for from I.G.A.M. Noy and C.A. van Diepen, 
   van Kraalingen, and Allard de Wit, Sep 2011
*/

#include <stdio.h>
#include <math.h>
#include "penman.h"
#include "wofost.h"

float sweaf(CropGroupNumber){
    float sweaf; 
    sweaf = 1./(0.76+1.5*Penman.ET0) - (5.-CropGroupNumber )*0.10;
    if  (CropGroupNumber < 3.) {
        sweaf = sweaf + (Penman.ET0 - 0.6)/(CropGroupNumber*
                (CropGroupNumber + 3.));
    }
    return limit(0.10, 0.95, sweaf);
}

float Evtra() {   
    float CriticalSoilMoisture;
    float ET0;
    float KDiffuse;
    float Lai;
    float MaxReductionOxygenStress;
    float MaxTranspiration;
    float ReductionMoisture;
    float ReductionOxygenStress;
    float SoilMoistureAeration;
    float SoilWatDepletion;
    float test;

    Lai = LeaveAreaIndex();
    
    KDiffuse = Afgen(KDiffuseTb, &DevelopmentStage);      
    Evtra.MaxEvapWater = Penman.E0 * exp(-0.75 * KDiffuse * Lai);
    Evtra.MaxEvapSoil  = max(Penman.ES0 * exp(-0.75 * KDiffuse * Lai);
    MaxTranspiration = max(0.0001,  Penman.ET0*(1.-exp(-0.75 * KDiffuse * Lai)));
    
    /*crop specific correction on potential transpiration rate */
    ET0 = CorrectionTransp * Penman.ET0;
    
    SoilWatDepletion = sweaf(CropGroupNumber);
    CriticalSoilMoisture = (1. - SoilWatDepletion)*
            (WatBal.ct.MoistureFC-WatBal.ct.MoistureWP) + WatBal.ct.MoistureWP;
    
/*   reduction in transpiration in case of water shortage */
    ReductionMoisture = limit(0.,1.,(SM-WatBal.ct.MoistureWP)/
            (CriticalSoilMoisture-WatBal.ct.MoistureWP));
    
    if (!Airducts) {
        /* critical soil moisture content for aeration */
        SoilMoistureAeration = WatBal.ct.MoistureSAT - WatBal.ct.CriticalSoilAirC;
        
        /* count days since start oxygen shortage (up to 4 days)*/
        if (SoilMoisture >= SoilMoistureAeration) {
            DaysOxygenStress = min(DaysOxygenStress++, 4.);
        }
        else {
            DaysOxygenStress = 0.;
        }
        
        /* maximum reduction reached after 4 days */
        MaxReductionOxygenStress = limit (0.,1.,(WatBal.ct.MoistureSAT-SoilMoisture)/
                (WatBal.ct.MoistureSAT - SoilMoistureAeration));
        ReductionOxygenStress   = MaxReductionOxygenStress + 
                (1.-DaysOxygenStress/4.)*(1.-MaxReductionOxygenStress);        
    }
    else {
        ReductionOxygenStress = 1.;
    }
     
    WatBal.rt.Transpiration = ReductionMoisture * ReductionOxygenStress * MaxTranspiration;

    return 1;

}