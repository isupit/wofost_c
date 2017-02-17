#include <stdio.h>
#include <stdlib.h>
#include "mmla.h"
#include "dynamic.h"
#include "wofost.h"
#include "extern.h"

float LeaveAreaIndex()
{
  float LAISum = 0.;
  Green *previous;
  
  previous = Crop.LeaveProperties;
  
  while (Crop.LeaveProperties) {
    LAISum = LAISum + Crop.LeaveProperties->weight*Crop.LeaveProperties->area;
    Crop.LeaveProperties = Crop.LeaveProperties->next;
  }
  
 Crop.LeaveProperties = previous; 
 /* return Green Area Index which will be used as LAI */     
return (LAISum + Crop.st.stems*Afgen(SpecificStemArea, &DevelopmentStage) +
	     Crop.st.storage*SpecificPodArea) ;
}
