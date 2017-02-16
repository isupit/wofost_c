#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "wofost.h"
#include "site.h"


int GetSiteData()
{
  AFGEN *FillData[15], *Table, *start;
  int i, c;
  float Variable[NUMBER_OF_VARIABLES], XValue, YValue;
  char x[2], xx[2],  word[NUMBER_OF_VARIABLES];
  FILE *fq;

 if ((fq = fopen("data//wofost.sit", "rt")) == NULL)
    {fprintf(stderr, "Cannot open input file.\n"); return 0;}

 i=0;
  while ((c=fscanf(fq,"%s",word)) != EOF) 
  {
    if (!strcmp(word, SiteParam[i])) {
        while ((c=fgetc(fq)) !='=');
	fscanf(fq,"%f",  &Variable[i]);

	i++; 
       }  
  }

  if (i!=NUMBER_OF_VARIABLES) return 0;
  rewind(fq);  
  FillSiteVariables(Variable);
 

  i=0;
  while ((c=fscanf(fq,"%s",word)) != EOF) 
  {
    if (!strcmp(word, SiteParam2[i])) {
        Table = start = malloc(sizeof(AFGEN));
	fscanf(fq,"%s %f %s  %f", x, &Table->x, xx, &Table->y);
        Table->next = NULL;				     
			       
	while ((c=fgetc(fq)) !='\n');
	while (fscanf(fq," %f %s  %f",  &XValue, xx, &YValue) > 0) {
 	    Table->next = malloc(sizeof(AFGEN));
            Table = Table->next; Table->next = NULL;
	    Table->x = XValue;
	    Table->y = YValue;
	    
	    while ((c=fgetc(fq)) !='\n');
	    }
	    FillData[i] = start;
	i++; 
       }      
  }

  if (i!= NUMBER_OF_TABLES) return 0;
   
  NotInfTB         = FillData[0];

return 1;
}

