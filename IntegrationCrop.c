#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "wofost.h"
#include "extern.h"

/* ---------------------------------------------------------------------------*/
/*  function IntegrationCrop                                                  */
/*  Purpose: Establish the crop state variables by integration of the crop    */
/*  rate variables and update the crop leave classes                          */
/* ---------------------------------------------------------------------------*/

void IntegrationCrop()	    
{
    float PhysAgeing;
    Green *LeaveProperties;
    
    Crop.st.roots    += Crop.rt.roots;
    Crop.st.stems    += Crop.rt.stems;
    Crop.st.leaves   += Crop.rt.leaves;
    Crop.st.storage  += Crop.rt.storage;
    Crop.st.LAIExp   += Crop.rt.LAIExp;

    /* Establish the age increase */
    PhysAgeing = max(0., (Temp - TempBaseLeaves)/(35.- TempBaseLeaves));
    
    /* Store the initial address */
    LeaveProperties = Crop.LeaveProperties;
    
    /* Update the leave age for each age class */
    while (Crop.LeaveProperties->next) {
       Crop.LeaveProperties->age = Crop.LeaveProperties->age + PhysAgeing;
    Crop.LeaveProperties      = Crop.LeaveProperties->next;}
  
    /* Return to beginning of the linked list */
    Crop.LeaveProperties = LeaveProperties;	 
   
}       	     
