#include <stdio.h>
#include <stdlib.h>
#include "wofost.h"

void Clean()
{
  int i;  
    
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
  
  /* Clean Afgen Tables*/
  AFGEN *table = NULL;
  AFGEN *base  = NULL;  
    
  for (i = 0; i < NUMBER_OF_TABLES; i++)
  {
    *table = AfgenTable[i];  
    while (table->next)
    {
      base  = table;
      table = table->next; 
      free(base);
    }
    free(table);
    //*AfgenTable[i] = NULL;
  }
  
}
