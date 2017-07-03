#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include "wofost.h"
#include "extern.h"

/* ---------------------------------------------------------------------------*/
/*  function RespirationRef(float TotalAssimilation)                          */
/*  Purpose: Calculation of the crop respiration rate (kg ha-1 d-1). Note     */
/*  that the respiration rate can not exceed the total assimilation rate.     */
/* ---------------------------------------------------------------------------*/

float RespirationRef(float TotalAssimilation)
{
      float respiration; 
      float TempRef = 25.;

      respiration  = RelRespiLeaves * Crop.st.leaves;
      respiration  += RelRespiStorage * Crop.st.storage;
      respiration  += RelRespiRoots * Crop.st.roots;	
      respiration  += RelRespiStems * Crop.st.stems;
      respiration  *= Afgen(FactorSenescence, &DevelopmentStage);
      respiration  *= pow(Q10, 0.1 * (Temp-TempRef));
      
      return (min(respiration, TotalAssimilation));
}
