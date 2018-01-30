#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include "wofost.h"
#include "extern.h"
#include <time.h>

int main(int argc, char *argv[]) {
    int Emergence;
    int CycleLength   = 240;

    char path[100];
    char cropfile[100];
    char soilfile[100];
    char sitefile[100];
    char management[100];
    char dateString [100];
    char station[100];

    if (argc != 8) return 0;
    if (strlen(argv[1]) + strlen(argv[2]) > 98) return 0;
    if (strlen(argv[1]) + strlen(argv[3]) > 98) return 0;
    if (strlen(argv[1]) + strlen(argv[4]) > 98) return 0;
    if (strlen(argv[1]) + strlen(argv[5]) > 98) return 0;
    if (strlen(argv[6]) > 12) return 0;
    if (strlen(argv[7]) > 12) return 0;

    strncpy(path, argv[1], 98);
    
    strncpy(cropfile, argv[1], 98);
    strncat(cropfile, argv[2], 98);

    strncpy(soilfile, argv[1], 98);
    strncat(soilfile, argv[3], 98);

    strncpy(management, argv[1], 98);
    strncat(management, argv[4], 98);

    strncpy(sitefile, argv[1], 98);
    strncat(sitefile, argv[5], 98);

    strncpy(dateString, argv[6], 98);
    strncpy(station, argv[7], 98);
    
    Emergence = 1;
    Step = 1.;
    Day = 1;
    
    GetCropData(cropfile);
    GetSoilData(soilfile);
    GetSiteData(sitefile);
    GetManagement(management);
    GetMeteoData(path, dateString, station);

    InitializeCrop(Emergence);
    InitializeWatBal();
    InitializeNutrients();
    
    while (DevelopmentStage <= DevelopStageHarvest && Day < CycleLength) {
        Temp = 0.5 * (Tmax[Day] + Tmin[Day]);
        DayTemp = 0.5 * (Tmax[Day] + Temp);
        
        printf("\n%4d-%02d-%02d",simTime.tm_year + 1900, simTime.tm_mon +1, simTime.tm_mday);
        printf(" %4d", Day);
        printf(" Stems: %7.0f", Crop.st.stems);
        printf(" Leaves: %7.0f", Crop.st.leaves);
        printf(" sto: %7.0f", Crop.st.storage);
        printf(" LAI: %7.2f", LAI);
        printf(" dvs: %7.2f", DevelopmentStage);

        Astro();
        CalcPenman();

        RateCalulationWatBal();
        RateCalcultionNutrients();
        RateCalculationCrop();

        IntegrationWatBal();
        IntegrationNutrients();
        IntegrationCrop();

        LAI = LeaveAreaIndex();
        DevelopmentStage = GetDevelopmentStage();

        Day++;
        simTime.tm_mday++;
        mktime(&simTime);
    }

    Clean();


    return 0;
}


