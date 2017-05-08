#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "wofost.h"
#include "crop.h"


int GetCropData()
{
  AFGEN *Table, *start;
  int i, c;
  float Variable[NR_VARIABLES_CRP], XValue, YValue;
  char x[2], xx[2],  word[NR_VARIABLES_CRP];
  FILE *fq;

 if ((fq = fopen("data//wwh107.cab", "rt")) == NULL)
    {fprintf(stderr, "Cannot open input file.\n"); return 0;}

 i=0;
  while ((c=fscanf(fq,"%s",word)) != EOF) 
  {
    if (!strcmp(word, CropParam[i])) {
        while ((c=fgetc(fq)) !='=');
	fscanf(fq,"%f",  &Variable[i]);
	i++; 
       }  
  }

  if (i!=NR_VARIABLES_CRP) return 0;
  rewind(fq);  
  FillCropVariables(Variable);
 

  i=0;
  while ((c=fscanf(fq,"%s",word)) != EOF) 
  {
    if (!strcmp(word, CropParam2[i])) {
        Table = start = malloc(sizeof(AFGEN));
	fscanf(fq,"%s %f %s  %f", x, &Table->x, xx, &Table->y);
        Table->next = NULL;				     
			       
	while ((c=fgetc(fq)) !='\n');
	while (fscanf(fq," %f %s  %f",  &XValue, xx, &YValue) > 0)  {
	    Table->next = malloc(sizeof(AFGEN));
            Table = Table->next; Table->next = NULL;
	    Table->x = XValue;
	    Table->y = YValue;
	    
	    while ((c=fgetc(fq)) !='\n');
	    }
	    AfgenTable[i] = start;
	i++; 
       }      
  }

  if (i!= NR_TABLES_CRP) return 0;
 
  
  DeltaTempSum         = AfgenTable[0];
  SpecificLeaveArea    = AfgenTable[1];
  SpecificStemArea     = AfgenTable[2];
  KDiffuseTb           = AfgenTable[3];
  EFFTb                = AfgenTable[4];
  MaxAssimRate         = AfgenTable[5];
  FactorAssimRateTemp  = AfgenTable[6];
  FactorGrossAssimTemp = AfgenTable[7];
  FactorSenescence     = AfgenTable[8];
  Roots                = AfgenTable[9];
  Leaves               = AfgenTable[10];
  Stems                = AfgenTable[11];
  Storage              = AfgenTable[12];
  DeathRateStems       = AfgenTable[13];
  DeathRateRoots       = AfgenTable[14]; 
  N_MaxLeaves          = AfgenTable[15];
  P_MaxLeaves          = AfgenTable[16];
  K_MaxLeaves          = AfgenTable[17];
          
return 1;
}

