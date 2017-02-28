#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "wofost.h"
#include "crop.h"


int GetCropData()
{
  AFGEN *FillData[15], *Table, *start;
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
	    FillData[i] = start;
	i++; 
       }      
  }

  if (i!= NR_TABLES_CRP) return 0;
 
  
  DeltaTempSum         = FillData[0];
  SpecificLeaveArea    = FillData[1];
  SpecificStemArea     = FillData[2];
  KDiffuseTb           = FillData[3];
  EFFTb                = FillData[4];
  MaxAssimRate         = FillData[5];
  FactorAssimRateTemp  = FillData[6];
  FactorGrossAssimTemp = FillData[7];
  FactorSenescence     = FillData[8];
  Roots                = FillData[9];
  Leaves               = FillData[10];
  Stems                = FillData[11];
  Storage              = FillData[12];
  DeathRateStems       = FillData[13];
  DeathRateRoots       = FillData[14]; 

return 1;
}

