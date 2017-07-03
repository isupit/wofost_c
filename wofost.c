#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include "wofost.h"
#include "extern.h"


int main(int argc,char *argv[])
{
   while(argc--)
    {
        printf("%s %d\n",argv[argc], strlen(argv[argc]));
    } 
    
    
  int  Emergence, EndDay = 242;
  char cropfile[100];
  
  Emergence = 1;
  Step = 1.;

  GetCropData(); 
  GetMeteoData();
  GetSoilData();
  GetSiteData();
  GetManagement();

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
                      	     

