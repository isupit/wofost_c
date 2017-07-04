#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include "wofost.h"
#include "extern.h"


int main(int argc,char *argv[])
{
   int Emergence;
   int EndDay = 242;
   
   char cropfile[100];
   char soilfile[100];
   char sitefile[100];
   char management[100];

   if (argc != 6) return 0;
   if (strlen(argv[1]) + strlen(argv[2]) > 98) return 0;
   if (strlen(argv[1]) + strlen(argv[3]) > 98) return 0;
   if (strlen(argv[1]) + strlen(argv[4]) > 98) return 0;
   if (strlen(argv[1]) + strlen(argv[5]) > 98) return 0;
   
   strcpy(cropfile,argv[1]);
   strcat(cropfile,argv[2]);

   strcpy(soilfile,argv[1]);
   strcat(soilfile,argv[3]);
   
   strcpy(management,argv[1]);
   strcat(management,argv[4]);
   
   strcpy(sitefile,argv[1]);
   strcat(sitefile,argv[5]);
             
  Emergence = 1;
  Step = 1.;

  GetCropData(cropfile); 
  GetSoilData(soilfile);
  GetSiteData(sitefile);
  GetManagement(management);
  GetMeteoData();

  Day = 1;
  InitializeCrop(Emergence); 
  InitializeWatBal();
  InitializeNutrients();
  
  while (DevelopmentStage <= DevelopStageHarvest && Day < EndDay) 
  {
    Temp = 0.5*(Tmax[Day] + Tmin[Day]);
    DayTemp = 0.5*(Tmax[Day] + Temp);
    
    printf("\n%4d", Day); 
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
    
    LAI              = LeaveAreaIndex();
    DevelopmentStage = GetDevelopmentStage();
    
    Day++;
}
  
Clean();


return 0;
}           
                      	     

