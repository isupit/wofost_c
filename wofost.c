#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include "mmla.h"
#include "wofost.h"
#include "dynamic.h"
#include "extern.h"


void EulerIntegration()	    
{
       float PhysAgeing;
       Green *LeaveProperties;

       LeaveProperties = Crop.LeaveProperties;
       
       Crop.st.roots    = Crop.st.roots   + Crop.rt.roots;
       Crop.st.stems    = Crop.st.stems   + Crop.rt.stems;
       Crop.st.leaves   = Crop.st.leaves  + Crop.rt.leaves;
       Crop.st.storage  = Crop.st.storage + Crop.rt.storage;
       Crop.st.LAIExp   = Crop.st.LAIExp  + Crop.rt.LAIExp;

       /* Go to the end of the list */
       //while (Crop.LeaveProperties->next) 
       //       Crop.LeaveProperties = Crop.LeaveProperties->next;
       
       PhysAgeing = max(0., (Temp - TempBaseLeaves)/(35.- TempBaseLeaves));
       
       /* Update the leave age for each age class */
       while (Crop.LeaveProperties->next) {
          Crop.LeaveProperties->age = Crop.LeaveProperties->age + PhysAgeing;
	  Crop.LeaveProperties      = Crop.LeaveProperties->next;}
	  
       /* Return to beginning of the linked list */
       Crop.LeaveProperties = LeaveProperties;	  
       
}       	     

float Conversion(float NetAssimilation)
{
	float fr, root, shoot;
	
	fr    = Afgen(Roots, &DevelopmentStage);
	root  = fr/ConversionRoots;
	shoot = Afgen(Stems, &DevelopmentStage)/ConversionStems;
	shoot = shoot + Afgen(Leaves, &DevelopmentStage)/ConversionLeaves;	
	shoot = shoot + Afgen(Storage, &DevelopmentStage)/ConversionStorage;
	
	/* conversion */
	return NetAssimilation/(shoot*(1-fr)+root);
}
 
void Growth(float NewPlantMaterial)
{
        float shoots, FractionRoots;
	float DeathRoots, DeathStems;
		
	FractionRoots = Afgen(Roots, &DevelopmentStage);
	DeathRoots    = Crop.st.roots*Afgen(DeathRateRoots, &DevelopmentStage);
	Crop.rt.roots   = NewPlantMaterial*FractionRoots - DeathRoots;
	
	shoots        = NewPlantMaterial*(1-FractionRoots);
	    
	DeathStems    = Crop.st.stems*Afgen(DeathRateStems, &DevelopmentStage);	
	Crop.rt.stems = shoots*Afgen(Stems, &DevelopmentStage)-DeathStems;
	
	Crop.rt.storage = shoots*Afgen(Storage, &DevelopmentStage);
	
	Crop.rt.leaves = shoots*Afgen(Leaves, &DevelopmentStage);
	Crop.rt.LAIExp = LeaveGrowth(Crop.st.LAIExp, Crop.rt.leaves);	
	Crop.rt.leaves = Crop.rt.leaves - DyingLeaves();
	
        Crop.RootDepth_prev = Crop.RootDepth;
        Crop.RootDepth = min(Crop.MaxRootingDepth-Crop.RootDepth,
                MaxIncreaseRoot*Step);
}	
	

float RespirationRef(float TotalAssimilation)
{
      float respiration; 
      float TempRef = 25.;

      respiration  = RelRespiLeaves*Crop.st.leaves;
      respiration  = respiration + RelRespiStorage*Crop.st.storage;
      respiration  = respiration + RelRespiRoots*Crop.st.roots;	
      respiration  = respiration + RelRespiStems*Crop.st.stems;
      respiration  = respiration * Afgen(FactorSenescence, &DevelopmentStage);
      respiration  = respiration * pow(Q10, 0.1*(Temp-TempRef));
      
      return (min(respiration, TotalAssimilation));
}

void Initialize(int Emergence)
{
       float FractionRoots, FractionShoots, InitialShootWeight;
       float DeltaTempSum, TempSum=0;
             
       if (!Emergence)
    	{
	while (TempSum < TSumEmergence) 
	    {DeltaTempSum=limit(0, TempEffMax-TempBaseEmergence, 
                    Temp-TempBaseEmergence);
	     TempSum = TempSum + DeltaTempSum;
	     Day++;
	     }
	}	     
       
       DevelopmentStage = InitialDVS;
       
       FractionRoots      = Afgen(Roots, &DevelopmentStage);
       FractionShoots     = 1 - FractionRoots;
       InitialShootWeight = InitialDryWeight*FractionShoots;
       
       Crop.st.roots     = InitialDryWeight*FractionRoots;
       Crop.RootDepth    = InitRootingDepth;
       Crop.st.stems     = InitialShootWeight*Afgen(Stems, &DevelopmentStage);                   
       Crop.st.leaves    = InitialShootWeight*Afgen(Leaves, &DevelopmentStage);
       Crop.st.storage   = InitialShootWeight*Afgen(Storage, &DevelopmentStage);
       
       Crop.MaxRootingDepth = max(InitRootingDepth,min(MaxRootingDepth,
            Site.SoilLimRootDepth));

       LAIEmergence  = Crop.st.leaves*Afgen(SpecificLeaveArea, &DevelopmentStage); 
       
       Crop.st.LAIExp = LAIEmergence;
      
       LAI = LAIEmergence + Crop.st.stems*Afgen(SpecificStemArea, &DevelopmentStage) +
	     Crop.st.storage*SpecificPodArea;

       Crop.LeaveProperties         = malloc(sizeof (Green));
       Crop.LeaveProperties->age    = 0.;
       Crop.LeaveProperties->weight = Crop.st.leaves;
       Crop.LeaveProperties->area   = Afgen(SpecificLeaveArea, &DevelopmentStage);
       Crop.LeaveProperties->next   = NULL;
       
}       


void RateCalculationCrop()
{
       float TotalAssimilation;
       float Maintenance, GrossAssimilation, GrossGrowth;    
       
       /* set rates to 0 */
       Crop.rt.roots   = 0.;
       Crop.rt.leaves  = 0.;
       Crop.rt.stems   = 0.;
       Crop.rt.storage = 0.;
       Crop.rt.LAIExp  = 0.;      
       //Crop.LeaveProperties = NULL;
      
       /* assimilation */
       GrossAssimilation = DailyTotalAssimilation(Astro());

       /* correction for low minimum temperatures */
       TotalAssimilation = Correct(GrossAssimilation);

       /* respiration */
       Maintenance = RespirationRef(TotalAssimilation);

       /* conversion */
       GrossGrowth = Conversion(TotalAssimilation-Maintenance); 
      
       /* Growth of root, stems, leaves and storage organs */
       Growth(GrossGrowth);
             
       printf("  Dmi: %5.1f MRes: %5.1f Gass: %5.1f", GrossGrowth, Maintenance, TotalAssimilation);
}

int main(void)
{
  int  Emergence, EndDay = 240;
  
  Emergence = 1;

  GetCropData(); 
  GetMeteoData();
  GetSoilData();
  GetSiteData();

  Day = 0;
  Initialize(Emergence); 
  WatBalInitialize();
  
  while (DevelopmentStage <= DevelopStageHarvest && Day < EndDay) {
   
    Temp = 0.5*(Tmax[Day] + Tmin[Day]);
    DayTemp    = 0.5*(Tmax[Day] + Temp);
    
printf("\n%4d", Day); 
printf(" Stems: %7.0f", Crop.st.stems);
printf(" Leaves: %7.0f", Crop.st.leaves);
printf(" sto: %7.0f", Crop.st.storage); 
printf(" LAI: %7.2f", LAI);
printf(" dvs: %7.2f", DevelopmentStage); 


    Astro();
    CalcPenman();

    RateCalculationCrop();
    WatBalRateCalulation();
    
    EulerIntegration();
    WatBalIntegration();
    
    LAI              = LeaveAreaIndex();
    DevelopmentStage = GetDevelopmentStage();
    
    Day++;

}
  
Clean();


return 0;
}           
                      	     

