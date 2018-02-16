#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include "wofost.h"
#include "extern.h"
#include <time.h>

int main() {
    FILE *ifp;
    SimUnit *Grid = NULL;
    SimUnit *start = NULL;
       
    int Emergence;
    int Start;
/*   int CycleLength   = 240;*/

    char path[100];
    char cropfile[100];
    char soilfile[100];
    char sitefile[100];
    char management[100];
    char dateString [100];
 //   char station[100];
    char place[15];
    
    char cf[100], sf[100], mf[100], site[100];
  
    Emergence = 1;
    Step = 1.;
    Day = 1;
    
    
    ifp = fopen("list.txt", "r");

    if (ifp == NULL) {
        fprintf(stderr, "Can't open input list.txt\n");
        exit(1);
    }
    
    
    while (fscanf(ifp,"%7s %10s %7s %12s %10s %10s %2s %d %d" ,
            path, cf, sf, mf, site, dateString, place, &Start, &Emergence)
            != EOF) {
        
        strncpy(cropfile, path, 98);
        strncat(cropfile, cf, 98);

        strncpy(soilfile, path, 98);
        strncat(soilfile, sf, 98);

        strncpy(management, path, 98);
        strncat(management, mf, 98);

        strncpy(sitefile, path, 98);
        strncat(sitefile, site, 98);

        printf("path: %7s\n", path);
        printf("cropfile: %10s\n",cropfile);
        printf("soilfile: %7s\n",soilfile);
        printf("management: %12s\n", management);
        printf("sitefile: %10s\n", sitefile);
        printf("dateString: %10s\n", dateString);
        printf("place: %2s\n", place);
        printf("Start: %2d\n", Start);
        printf("Emergence: %2d\n", Emergence);
        
        if ( start == NULL) {
            Grid = start = (SimUnit *) malloc(sizeof(SimUnit));
            Grid->crp  = GetCropData(cropfile);
            Grid->ste  = GetSiteData(sitefile);
            Grid->mng  = GetManagement(management);        
            Grid->soil = GetSoilData(soilfile); 
            Grid->next = NULL;        
        }
        else {
            Grid->next = (SimUnit *) malloc(sizeof(SimUnit));
            Grid = Grid->next;
            Grid->crp  = GetCropData(cropfile);
            Grid->ste  = GetSiteData(sitefile);
            Grid->mng  = GetManagement(management);
            Grid->soil = GetSoilData(soilfile);
            
            Grid->next = NULL;
        }
    }
    
    /* Go back to the beginning of the list */
    Grid = start;
    
    
 /*   GetSoilData(soilfile);
 *   GetSiteData(sitefile);
 *   GetManagement(management);
 *   GetMeteoData(path, dateString, station);
 */
    
    return 0;
}


