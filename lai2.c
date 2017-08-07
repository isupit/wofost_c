#include <stdio.h>
#include <stdlib.h>
#include "wofost.h"
#include "extern.h"

/* -------------------------------------------------------------------------*/
/*  function LeaveAreaIndex()                                               */
/*  Purpose: Calculation of the LAI (ha ha-1) of the LeaveProperties struct */
/* -------------------------------------------------------------------------*/
float LeaveAreaIndex()
{
    float LAISum = 0.;
    Green *LeaveProperties;

    /* Store the initial address */
    LeaveProperties = Crop.LeaveProperties;

    /* Loop until the last element in the list */
    while (Crop.LeaveProperties) {
      LAISum += Crop.LeaveProperties->weight*Crop.LeaveProperties->area;
      Crop.LeaveProperties = Crop.LeaveProperties->next;
    }

    /* Restore the initial address */
    Crop.LeaveProperties = LeaveProperties; 
 
    /* Return Green Area Index which will be used as LAI */     
    return (LAISum + Crop.st.stems * Afgen(SpecificStemArea, &DevelopmentStage) +
	     Crop.st.storage * SpecificPodArea) ;
}
