#include <stdio.h>
#include <stdlib.h>
#include "wofost.h"
#include "extern.h"

/* -----------------------------------------------------------------------*/
/*  function LeaveAreaIndex()                                             */
/*  Purpose: Calculation of the LAI of the LeaveProperties ha ha-1 struct */
/* -----------------------------------------------------------------------*/
float LeaveAreaIndex()
{
  float LAISum = 0.;
  Green *LeaveProperties;
  
  /* Store the start address */
  LeaveProperties = Crop.LeaveProperties;
  
  /* Loop until the last element in the list */
  while (Crop.LeaveProperties) {
    LAISum += Crop.LeaveProperties->weight*Crop.LeaveProperties->area;
    Crop.LeaveProperties = Crop.LeaveProperties->next;
  }
  
  /* Restore the start address */
 Crop.LeaveProperties = LeaveProperties; 
 
 /* Return Green Area Index which will be used as LAI */     
return (LAISum + Crop.st.stems * Afgen(SpecificStemArea, &DevelopmentStage) +
	     Crop.st.storage * SpecificPodArea) ;
}
