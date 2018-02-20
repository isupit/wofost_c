#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include "wofost.h"
#include "extern.h"
#include <time.h>

int main() {
    FILE *ifp;
    FILE **file;
    SimUnit *Grid = NULL;
    SimUnit *initial = NULL;
       
    int Emergence;
    int Start;
    int CycleLength   = 240;
    int count;

    char path[100];
    char cropfile[100];
    char soilfile[100];
    char sitefile[100];
    char management[100];
    char dateString [100];
    char place[15];
    
    char cf[100], sf[100], mf[100], site[100];
  
    Emergence = 1;
    Step = 1.;
    Day = 1;
    
    
    ifp = fopen("list.txt", "r");

    if (ifp == NULL) 
    {
        fprintf(stderr, "Can't open input list.txt\n");
        exit(1);
    }
    
    count = 0;
    while (fscanf(ifp,"%7s %10s %7s %12s %10s %10s %2s %d %d" ,
            path, cf, sf, mf, site, dateString, place, &Start, &Emergence)
            != EOF) 
    {    
        strncpy(cropfile, path, 98);
        strncat(cropfile, cf, 98);

        strncpy(soilfile, path, 98);
        strncat(soilfile, sf, 98);

        strncpy(management, path, 98);
        strncat(management, mf, 98);

        strncpy(sitefile, path, 98);
        strncat(sitefile, site, 98);

        /*printf("path: %7s\n", path);
        printf("cropfile: %10s\n",cropfile);
        printf("soilfile: %7s\n",soilfile);
        printf("management: %12s\n", management);
        printf("sitefile: %10s\n", sitefile);
        printf("dateString: %10s\n", dateString);
        printf("place: %2s\n", place);
        printf("Start: %2d\n", Start);
        printf("Emergence: %2d\n", Emergence);*/
        
        /* count the number of output files */
        /* number is the index number of the list of file pointers */

        
        if (initial == NULL) 
        {
            Grid = initial = (SimUnit *) malloc(sizeof(SimUnit));
            Grid->crp  = GetCropData(cropfile);
            Grid->ste  = GetSiteData(sitefile);
            Grid->mng  = GetManagement(management);        
            Grid->soil = GetSoilData(soilfile); 
            Grid->start = Start;
            Grid->file = count++;
            strcpy(Grid->name,cf);
            Grid->emergence = Emergence;
            Grid->next = NULL;   
        }
        else 
        {
            Grid->next = (SimUnit *) malloc(sizeof(SimUnit));
            Grid = Grid->next;
            Grid->crp  = GetCropData(cropfile);
            Grid->ste  = GetSiteData(sitefile);
            Grid->mng  = GetManagement(management);
            Grid->soil = GetSoilData(soilfile);
            Grid->start = Start;
            Grid->file = count++;
            strcpy(Grid->name,cf);
            Grid->emergence = Emergence;            
            Grid->next = NULL;
        }
    }
    
    GetMeteoData(path, dateString, place);
    
    /* allocate memory for the file pointers */
    file = malloc(sizeof(*file) * --count);
    
    /* open the output files */
    Grid = initial;
    while (Grid->next)
    {
        file[Grid->file] = fopen(Grid->name, "r");
        Grid = Grid->next;
    }
    
    
    for (Day = 1; Day <365; Day++)
    {
        count = 0; 
        
        /* Go back to the beginning of the list */
        Grid = initial;
        
        while (Grid->next);
        {
            Crop      = Grid->crp;
            WatBal    = Grid->soil;
            Mng       = Grid->mng;
            Site      = Grid->ste;
            Start     = Grid->start;
            Emergence = Grid->emergence;

            if (Day >= Start && Crop.Emergence == 0)
            {
                InitializeCrop(&Emergence);
            }

            if (Crop.DevelopmentStage <= Crop.prm.DevelopStageHarvest && Day < CycleLength) 
            {
                Temp = 0.5 * (Tmax[Day] + Tmin[Day]);
                DayTemp = 0.5 * (Tmax[Day] + Temp);

                fprintf(file[count++],"\n%4d-%02d-%02d,%4d,%7.0f,%7.0f,%7.0f,%7.2f,%7.2f",
                        simTime.tm_year + 1900, simTime.tm_mon +1, simTime.tm_mday,
                        Day,Crop.st.stems,Crop.st.leaves,Crop.st.storage,
                        Crop.st.LAI,Crop.DevelopmentStage);
               

                Astro();
                CalcPenman();

                RateCalulationWatBal();
                RateCalcultionNutrients();
                RateCalculationCrop();

                Crop.st.LAI = LeaveAreaIndex();
                Crop.DevelopmentStage = GetDevelopmentStage();

                IntegrationWatBal();
                IntegrationNutrients();
                IntegrationCrop();

                simTime.tm_mday++;
                mktime(&simTime);
            }
            Grid = Grid->next;
        }
    
        /* Go back to the beginning of the list */
        Grid  = initial;
        
        /* close the output files */
        count = 0;
        while (Grid->next)
        {
            fclose(file[count++]);
            Grid = Grid->next;
        }
    }
    
    
//Clean();



    return 0;
}


