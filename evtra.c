/* original: Penman.for from I.G.A.M. Noy and C.A. van Diepen, 
   van Kraalingen, and Allard de Wit, Sep 2011
*/

#include <stdio.h>
#include <math.h>
#include "penman.h"
#include "wofost.h"

Evtra Evtra()
{
    float KDiffuse;
    float ET0;
    float Lai;
    float MaxTranspiration;

    Lai = LeaveAreaIndex();
    
    KDiffuse = Afgen(KDiffuseTb, &DevelopmentStage)        
    Evtra.MaxEvapWater = Penman.E0 * exp(-0.75 * KDiffuse * Lai);
    Evtra.MaxEvapSoil  = max(Penman.ES0 * exp(-0.75 * KDiffuse * Lai);
    MaxTranspiration = max(0.0001,  Penman.ET0*(1.-exp(-0.75 * KDiffuse * Lai)));
    
    /*crop specific correction on potential transpiration rate */
    ET0 = CorrectionTransp * Penman.ET0;
    
    SoilWatDepletion = SWEAF (ET0,DEPNR)
    MCR = (1.-SWDEP)*(SMFCF-SMW)+SMW
*   reduction in transpiration in case of water shortage
    RFWS = LIMIT (0.,1.,(SM-SMW)/(SMCR-SMW))
    
     Evtra.EO  = 
     Evtra.ESO = 
     Evtra.ET0 = 

     return 1;

}