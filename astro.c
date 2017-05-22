#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "astro.h"
#include "extern.h"
#include "wofost.h"

#define  ANGLE  -4.0
#define  PI     3.1415926
#define  RAD	0.0174533

/* ---------------------------------------------------------------------*/
/*  function Astro()                                                    */
/*  Purpose: Calculation of the astronomical parameters used in Wofost  */
/* ---------------------------------------------------------------------*/

int Astro()
{
    float Declination, SolarConstant, AOB, DSinB;
    float FractionDiffuseRad;
    float AngotRadiation;
    
    if (Latitude > 67. || Latitude < 0.) return 0;  

    /* Remember this crap is written in C: Day start at 0 not at 1!!!! */
    Declination    = -asin(sin(23.45*RAD)*cos(2.*PI*(Day+11.)/365.));
    SolarConstant  = 1370.*(1.+0.033*cos(2.*PI*(float)(Day+1)/365.));
  
    SinLD = sin(RAD*Latitude)*sin(Declination);
    CosLD = cos(RAD*Latitude)*cos(Declination);
    AOB   = SinLD/CosLD;

    Daylength    = 12.0*(1.+2.*asin(AOB)/PI);
    PARDaylength = 12.0*(1.+2.*asin((-sin(ANGLE*RAD)+SinLD)/CosLD)/PI);
    
     /* integrals of sine of solar height */
     DSinB  = 3600.*(Daylength*SinLD+(24./PI)*CosLD*sqrt(1.-AOB*AOB));
     DSinBE = 3600.*(Daylength*(SinLD+0.4*(SinLD*SinLD + CosLD*CosLD*0.5))+
		 12.*CosLD*(2.+3.*0.4*SinLD)*sqrt(1.-AOB*AOB)/PI);

     /*  extraterrestrial radiation and atmospheric transmission */
     AngotRadiation  = SolarConstant*DSinB;
     AtmosphTransm   = Radiation[Day]/AngotRadiation;

     if (AtmosphTransm > 0.75)
        FractionDiffuseRad = 0.23;
  
     if (AtmosphTransm <= 0.75 && AtmosphTransm > 0.35)
        FractionDiffuseRad = 1.33-1.46 * AtmosphTransm;
  
     if (AtmosphTransm <= 0.35 && AtmosphTransm > 0.07) 
        FractionDiffuseRad = 1.-2.3*pow((AtmosphTransm-0.07), 2.);
  
     if (AtmosphTransm < 0.07)  
        FractionDiffuseRad = 1.0;
     
     DiffRadPP = 0.5 * FractionDiffuseRad * AtmosphTransm * SolarConstant;

     return 1;
}
