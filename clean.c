#include <stdio.h>
#include <stdlib.h>
#include "wofost.h"

void Clean()
{
  Green *LeaveProperties = NULL;
  
  /* Loop until the last element in the list */
  while (Crop.LeaveProperties->next)
  {
      LeaveProperties = Crop.LeaveProperties;
      Crop.LeaveProperties = Crop.LeaveProperties->next; 
      free(LeaveProperties);
  }
        
  /* Free last element in the list */
  free(Crop.LeaveProperties);
  
  Crop.LeaveProperties = NULL;
  
}
