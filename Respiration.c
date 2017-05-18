#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include "wofost.h"
#include "extern.h"


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

float RespirationRef(float TotalAssimilation)
{
      float respiration; 
      float TempRef = 25.;

      respiration  = RelRespiLeaves*Crop.st.leaves;
      respiration  += RelRespiStorage*Crop.st.storage;
      respiration  += RelRespiRoots*Crop.st.roots;	
      respiration  += RelRespiStems*Crop.st.stems;
      respiration  *= Afgen(FactorSenescence, &DevelopmentStage);
      respiration  *= pow(Q10, 0.1*(Temp-TempRef));
      
      return (min(respiration, TotalAssimilation));
}
