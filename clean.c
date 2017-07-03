#include <stdio.h>
#include <stdlib.h>
#include "wofost.h"

/* ---------------------------------------------------------------*/
/*  function Clean()                                              */
/*  Purpose: free all the allocated memory and set them to NULL   */
/* ---------------------------------------------------------------*/ 

void Clean()
{
  int i;  
    
  Green *LeaveProperties = NULL;
  
  /* Loop until the last element in the list and free each node */
  while (Crop.LeaveProperties->next)
  {
      LeaveProperties = Crop.LeaveProperties;
      Crop.LeaveProperties = Crop.LeaveProperties->next; 
      
      LeaveProperties = NULL;
      free(LeaveProperties);
  }
        
  /* Free the last node */
  Crop.LeaveProperties = NULL;
  free(Crop.LeaveProperties);
  
  
  /* Define temporary node */
  AFGEN *head  = NULL;  
    
  for (i = 0; i < NUMBER_OF_TABLES; i++)
  {
      while (AfgenTable[i]->next)
      {
          head = AfgenTable[i];
          AfgenTable[i] = AfgenTable[i]->next;
          
          /* free and clearing the node */
          head = NULL;
          free(head);
      }
      
      /* free and clearing the last node */
      AfgenTable[i] = NULL;
      free(AfgenTable[i]);
  }
  
}
