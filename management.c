#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "wofost.h"
#include "manage.h"
       
Management GetManagement(char *management)
{
  AFGEN *Table, *start;
  Management *MNG = NULL;
  
  int i, c;
  float Variable[100], XValue, YValue;
  char x[2], xx[2],  word[100];
  FILE *fq;

 if ((fq = fopen(management, "rt")) == NULL) {
     fprintf(stderr, "Cannot open input file.\n"); 
     exit(0);
 }

 i=0;
  while ((c=fscanf(fq,"%s",word)) != EOF && i < 12 ) {
    if (!strcmp(word, ManageParam[i])) {
        while ((c=fgetc(fq)) !='=');
	fscanf(fq,"%f",  &Variable[i]);
	i++; 
       }  
  }

  if (i != NR_VARIABLES_MANAGEMENT) {
    fprintf(stderr, "Something wrong with the Site variables.\n"); 
    exit(0);
  }
 
  rewind(fq);  
  FillManageVariables(MNG = malloc(sizeof(Management)), Variable);
 

  i=0;
  while ((c=fscanf(fq,"%s",word)) != EOF) 
  {
    if (!strcmp(word, ManageParam2[i])) {
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
	    AfgenTable[i + 24] = start;
	i++; 
       }      
  }

  fclose(fq);
  
  if (i!= NR_TABLES_MANAGEMENT){
    fprintf(stderr, "Something wrong with the Management tables.\n"); 
    exit(0);
  }
      
 
  MNG->N_Fert_table   = AfgenTable[24];
  MNG->P_Fert_table   = AfgenTable[25];
  MNG->K_Fert_table   = AfgenTable[26];
  MNG->N_Uptake_frac  = AfgenTable[27];
  MNG->P_Uptake_frac  = AfgenTable[28];
  MNG->K_Uptake_frac  = AfgenTable[29];
  MNG->Irrigation     = AfgenTable[30];
  
  return *MNG;
  }