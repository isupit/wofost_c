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

void GetCropData(Plant *CROP, char *cropfile)
{
  AFGEN *Table[NR_TABLES_CRP], *start;
  
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
  
  //CROP = malloc(sizeof(Plant));
  FillCropVariables(CROP, Variable);

  i=0;
  while ((c=fscanf(fq,"%s",word)) != EOF) 
  {
    if (!strcmp(word, CropParam2[i])) {
        Table[i] = start= malloc(sizeof(AFGEN));
	fscanf(fq,"%s %f %s  %f", x, &Table[i]->x, xx, &Table[i]->y);
        Table[i]->next = NULL;				     
			       
	while ((c=fgetc(fq)) !='\n');
	while (fscanf(fq," %f %s  %f",  &XValue, xx, &YValue) > 0)  {
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
  
  if (i!= NR_TABLES_CRP) 
  {
    fprintf(stderr, "Something wrong with the Crop tables.\n"); 
    exit(0);
  } 
  
  CROP->prm.DeltaTempSum         = Table[0];
  CROP->prm.SpecificLeaveArea    = Table[1];
  CROP->prm.SpecificStemArea     = Table[2];
  CROP->prm.KDiffuseTb           = Table[3];
  CROP->prm.EFFTb                = Table[4];
  CROP->prm.MaxAssimRate         = Table[5];
  CROP->prm.FactorAssimRateTemp  = Table[6];
  CROP->prm.FactorGrossAssimTemp = Table[7];
  CROP->prm.CO2AMAXTB            = Table[8];
  CROP->prm.CO2EFFTB             = Table[9];
  CROP->prm.CO2TRATB             = Table[10];
  CROP->prm.FactorSenescence     = Table[11];
  CROP->prm.Roots                = Table[12];
  CROP->prm.Leaves               = Table[13];
  CROP->prm.Stems                = Table[14];
  CROP->prm.Storage              = Table[15];
  CROP->prm.DeathRateStems       = Table[16];
  CROP->prm.DeathRateRoots       = Table[17]; 
  CROP->prm.N_MaxLeaves          = Table[18];
  CROP->prm.P_MaxLeaves          = Table[19];
  CROP->prm.K_MaxLeaves          = Table[20];
  
  CROP->Emergence = 0;
  CROP->TSumEmergence = 0.;
  
  /* STATES */  
  /* Set the initial growth states to zero */
  CROP->st.roots   = 0.;
  CROP->st.stems   = 0.;
  CROP->st.leaves  = 0.;
  CROP->st.storage = 0.;
  CROP->st.LAIExp  = 0.;
  
  /* Set the initial nutrient states to zero*/
  CROP->N_st.leaves = 0.;
  CROP->N_st.stems  = 0.;
  CROP->N_st.roots  = 0.;
  CROP->N_st.storage= 0.;
  
  CROP->P_st.leaves = 0.;
  CROP->P_st.stems  = 0.;
  CROP->P_st.roots  = 0.;
  CROP->P_st.storage= 0.;
        
  CROP->K_st.leaves = 0.;
  CROP->K_st.stems  = 0.;
  CROP->K_st.roots  = 0.;
  CROP->K_st.storage= 0.;
  
  /* Set the maximum nutrient concentration to zero at initialization */
  CROP->N_st.Max_lv = 0.;
  CROP->N_st.Max_st = 0.;
  CROP->N_st.Max_ro = 0.;
  
  CROP->P_st.Max_lv = 0.;
  CROP->P_st.Max_st = 0.;
  CROP->P_st.Max_ro = 0.;
  
  CROP->K_st.Max_lv = 0.;
  CROP->K_st.Max_st = 0.;
  CROP->K_st.Max_ro = 0.;
  
  /* Set the initial optimal leave concentrations to zero */
  CROP->N_st.Optimum_lv = 0;
  CROP->N_st.Optimum_st = 0;
  
  CROP->P_st.Optimum_lv = 0;
  CROP->K_st.Optimum_st = 0;
  
  CROP->K_st.Optimum_lv = 0;
  CROP->K_st.Optimum_st = 0;
  
  /* No nutrient stress at initialization */
  CROP->NPK_Indx  = 1.;
  CROP->N_st.Indx = 1.;
  CROP->P_st.Indx = 1.;
  CROP->K_st.Indx = 1.;
  
   /* Set the initial uptake states to zero*/
  CROP->N_st.Uptake    = 0.;
  CROP->N_st.Uptake_lv = 0.;
  CROP->N_st.Uptake_st = 0.;
  CROP->N_st.Uptake_ro = 0.;
  
  CROP->P_st.Uptake    = 0.;
  CROP->P_st.Uptake_lv = 0.;
  CROP->P_st.Uptake_st = 0.;
  CROP->P_st.Uptake_ro = 0.;
  
  CROP->K_st.Uptake    = 0.;
  CROP->K_st.Uptake_lv = 0.;
  CROP->K_st.Uptake_st = 0.;
  CROP->K_st.Uptake_ro = 0.;
  
  /* No nutrient losses at initialization */
  CROP->N_st.death_lv = 0.;
  CROP->N_st.death_st = 0.;
  CROP->N_st.death_ro = 0.;
  
  CROP->P_st.death_lv = 0.;
  CROP->P_st.death_st = 0.;
  CROP->P_st.death_ro = 0.;
  
  CROP->K_st.death_lv = 0.;
  CROP->K_st.death_st = 0.;
  CROP->K_st.death_ro = 0.;
          
}

