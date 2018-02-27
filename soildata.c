#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "wofost.h"
#include "soil.h"


Soil GetSoilData(char *soilfile)
{
  AFGEN *Table[NR_TABLES_SOIL], *start;
  Soil *SOIL = NULL;
  
  int i, c;
  float Variable[100], XValue, YValue;
  char x[2], xx[2],  word[100];
  FILE *fq;

 if ((fq = fopen(soilfile, "rt")) == NULL)
 {
     fprintf(stderr, "Cannot open input file.\n"); 
     exit(0);
 }

 i=0;
  while ((c=fscanf(fq,"%s",word)) != EOF && i < 12 ) 
  {
    if (!strcmp(word, SoilParam[i])) {
        while ((c=fgetc(fq)) !='=');
	fscanf(fq,"%f",  &Variable[i]);

	i++; 
       }  
  }

 if (i != NR_VARIABLES_SOIL) 
 {
    fprintf(stderr, "Something wrong with the Soil variables.\n"); 
    exit(0);
 }
 
  rewind(fq);  
  
  SOIL = malloc(sizeof(Soil));
  FillSoilVariables(SOIL, Variable);
 

  i=0;
  while ((c=fscanf(fq,"%s",word)) != EOF) 
  {
    if (!strcmp(word, SoilParam2[i])) 
    {
        Table[i] = start= malloc(sizeof(AFGEN));
	fscanf(fq,"%s %f %s  %f", x, &Table[i]->x, xx, &Table[i]->y);
        Table[i]->next = NULL;				     
			       
	while ((c=fgetc(fq)) !='\n');
	while (fscanf(fq," %f %s  %f",  &XValue, xx, &YValue) > 0)  
        {
	    Table[i]->next = malloc(sizeof(AFGEN));
            Table[i] = Table[i]->next; 
            Table[i]->next = NULL;
	    Table[i]->x = XValue;
	    Table[i]->y = YValue;
	    
	    while ((c=fgetc(fq)) !='\n');
	    }
        /* Go back to beginning of the table */
        Table[i] = start;
	i++; 
       }      
  }
  
  fclose(fq);

 if (i!= NR_TABLES_SOIL) 
 {
    fprintf(stderr, "Something wrong with the Soil tables.\n"); 
    exit(0);
 }
 
  SOIL->VolumetricSoilMoisture = Table[0];
  SOIL->HydraulicConductivity  = Table[1];
  
  //free(SOIL);
  
return *SOIL;
}

