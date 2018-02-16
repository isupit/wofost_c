#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "wofost.h"
#include "crop.h"
#include "extern.h"

/* ------------------------------------------------------------------------*/
/*  function GetCropData()                                                 */
/*  Purpose: Read the Wofost crop file and store the parameters and tables */
/* ------------------------------------------------------------------------*/

Plant GetCropData(char *cropfile)
{
  AFGEN *Table, *start;
  Plant *CROP = NULL;
  
  int i, c;
  float Variable[NR_VARIABLES_CRP], XValue, YValue;
  char x[2], xx[2],  word[NR_VARIABLES_CRP];
  FILE *fq;
  
  
 if ((fq = fopen(cropfile, "rt")) == NULL)
    {fprintf(stderr, "Cannot open input file.\n"); exit(0);}

 i=0;
  while ((c=fscanf(fq,"%s",word)) != EOF) 
  {
    if (!strcmp(word, CropParam[i])) {
        while ((c=fgetc(fq)) !='=');
	fscanf(fq,"%f",  &Variable[i]);
	i++; 
       }  
  }

  if (i!= NR_VARIABLES_CRP)
  {
    fprintf(stderr, "Something wrong with the Crop variables.\n"); 
    exit(0);
  }
 
  rewind(fq);  
  FillCropVariables(CROP = malloc(sizeof(Plant)), Variable);
 

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
            Table = Table->next; 
            Table->next = NULL;
	    Table->x = XValue;
	    Table->y = YValue;
	    
	    while ((c=fgetc(fq)) !='\n');
	    }
	    AfgenTable[i] = start;
	i++; 
       }      
  }

  fclose(fq);
  
  if (i!= NR_TABLES_CRP) 
  {
    fprintf(stderr, "Something wrong with the Crop tables.\n"); 
    exit(0);
  } 
  
  CROP->prm.DeltaTempSum         = AfgenTable[0];
  CROP->prm.SpecificLeaveArea    = AfgenTable[1];
  CROP->prm.SpecificStemArea     = AfgenTable[2];
  CROP->prm.KDiffuseTb           = AfgenTable[3];
  CROP->prm.EFFTb                = AfgenTable[4];
  CROP->prm.MaxAssimRate         = AfgenTable[5];
  CROP->prm.FactorAssimRateTemp  = AfgenTable[6];
  CROP->prm.FactorGrossAssimTemp = AfgenTable[7];
  CROP->prm.CO2AMAXTB            = AfgenTable[8];
  CROP->prm.CO2EFFTB             = AfgenTable[9];
  CROP->prm.CO2TRATB             = AfgenTable[10];
  CROP->prm.FactorSenescence     = AfgenTable[11];
  CROP->prm.Roots                = AfgenTable[12];
  CROP->prm.Leaves               = AfgenTable[13];
  CROP->prm.Stems                = AfgenTable[14];
  CROP->prm.Storage              = AfgenTable[15];
  CROP->prm.DeathRateStems       = AfgenTable[16];
  CROP->prm.DeathRateRoots       = AfgenTable[17]; 
  CROP->prm.N_MaxLeaves          = AfgenTable[18];
  CROP->prm.P_MaxLeaves          = AfgenTable[19];
  CROP->prm.K_MaxLeaves          = AfgenTable[20];
          
return *CROP;
}

