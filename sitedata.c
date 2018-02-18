#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "wofost.h"
#include "site.h"


Field GetSiteData(char *sitefile)
{
  AFGEN *Table, *start;
  Field *SITE = NULL;
  
  int i, c;
  float Variable[NR_VARIABLES_SITE], XValue, YValue;
  char x[2], xx[2],  word[NR_VARIABLES_SITE];
  FILE *fq;

 if ((fq = fopen(sitefile, "rt")) == NULL) {
     fprintf(stderr, "Cannot open input file.\n"); 
     exit(0);
 }

 i=0;
 while ((c=fscanf(fq,"%s",word)) != EOF) {
    if (!strcmp(word, SiteParam[i])) {
        while ((c=fgetc(fq)) !='=');
        
	fscanf(fq,"%f",  &Variable[i]);

	i++; 
      }  
 }

  if (i != NR_VARIABLES_SITE) {
      fprintf(stderr, "Something wrong with the Site variables.\n");
      exit(0);
  }
  rewind(fq);  

  FillSiteVariables(SITE = malloc(sizeof(Field)), Variable);
 

  i=0;
  while ((c=fscanf(fq,"%s",word)) != EOF) {
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
	    AfgenTable[i + 23] = start;
	i++; 
       }      
  }
  
  fclose(fq);

  if (i != NR_TABLES_SITE)
  {
    fprintf(stderr, "Something wrong with the Site tables.\n"); 
    exit(0);
  } 
   
  SITE->NotInfTB = AfgenTable[23];

  return *SITE;
}

