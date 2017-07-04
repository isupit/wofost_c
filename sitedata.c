#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "wofost.h"
#include "site.h"


int GetSiteData(char *sitefile)
{
  AFGEN *Table, *start;
  int i, c;
  float Variable[NR_VARIABLES_SITE], XValue, YValue;
  char x[2], xx[2],  word[NR_VARIABLES_SITE];
  FILE *fq;

 if ((fq = fopen(sitefile, "rt")) == NULL)
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

  if (i!=NR_VARIABLES_SITE) return 0;
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
	    
	    while ((c=fgetc(fq)) !='\n' || (c=fgetc(fq)) != EOF );
	    }
	    AfgenTable[i + 21] = start;
	i++; 
       }      
  }

  if (i!= NR_TABLES_SITE) return 0;
   
  NotInfTB         = AfgenTable[21];

return 1;
}

