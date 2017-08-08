#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "wofost.h"
#include "manage.h"
       
int GetManagement(char *management)
{
  AFGEN *Table, *start;
  int i, c;
  float Variable[100], XValue, YValue;
  char x[2], xx[2],  word[100];
  FILE *fq;

 if ((fq = fopen(management, "rt")) == NULL)
    {fprintf(stderr, "Cannot open input file.\n"); return 0;}

 i=0;
  while ((c=fscanf(fq,"%s",word)) != EOF && i < 12 ) 
  {
    if (!strcmp(word, ManageParam[i])) {
        while ((c=fgetc(fq)) !='=');
	fscanf(fq,"%f",  &Variable[i]);
	i++; 
       }  
  }

  if (i != NR_VARIABLES_MANAGEMENT) return 0;
  rewind(fq);  
  FillManageVariables(Variable);
 

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
	    AfgenTable[i + 22] = start;
	i++; 
       }      
  }

  fclose(fq);
  
  if (i!= NR_TABLES_MANAGEMENT) return 0;
 
  N_Fert_table   = AfgenTable[22];
  P_Fert_table   = AfgenTable[23];
  K_Fert_table   = AfgenTable[24];
  N_Uptake_frac  = AfgenTable[25];
  P_Uptake_frac  = AfgenTable[26];
  K_Uptake_frac  = AfgenTable[27];
  Irrigation     = AfgenTable[28];
  

return 1;
}

