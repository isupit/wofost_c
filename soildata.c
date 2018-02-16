#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "wofost.h"
#include "soil.h"


Soil GetSoilData(char *soilfile)
{
  AFGEN *Table, *start;
  int i, c;
  float Variable[100], XValue, YValue;
  char x[2], xx[2],  word[100];
  FILE *fq;

 if ((fq = fopen(soilfile, "rt")) == NULL)
    {fprintf(stderr, "Cannot open input file.\n"); return 0;}

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
  FillSoilVariables(SOIL = malloc(sizeof(Soil)), Variable);
 

  i=0;
  while ((c=fscanf(fq,"%s",word)) != EOF) 
  {
    if (!strcmp(word, SoilParam2[i])) 
    {
        Table = start = malloc(sizeof(AFGEN));
	fscanf(fq,"%s %f %s  %f", x, &Table->x, xx, &Table->y);
        Table->next = NULL;				     
			       
	while ((c=fgetc(fq)) !='\n');
	while (fscanf(fq," %f %s  %f",  &XValue, xx, &YValue) > 0)  
        {
	    Table->next = malloc(sizeof(AFGEN));
            Table = Table->next; 
            Table->next = NULL;
	    Table->x = XValue;
	    Table->y = YValue;
	    
	    while ((c=fgetc(fq)) !='\n');
	    }
	    AfgenTable[i + 21] = start;
	i++; 
       }      
  }
  
  fclose(fq);

 if (i!= NR_TABLES_SOIL) 
 {
    fprintf(stderr, "Something wrong with the Soil tables.\n"); 
    exit(0);
 }
 
  SOIL->VolumetricSoilMoisture = AfgenTable[21];
  SOIL->HydraulicConductivity  = AfgenTable[22];
  

return *SOIL;
}

