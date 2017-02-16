#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include "mmla.h"
#include "wofost.h"
#include "dynamic.h"
#include "extern.h"


void EulerIntegration(Plant Delta)	    
{
       float PhysAgeing;
       Green *previous;

       previous = Crop.LeaveProperties;
       
       Crop.roots    = Crop.roots   + Delta.roots;
       Crop.stems    = Crop.stems   + Delta.stems;
       Crop.leaves   = Crop.leaves  + Delta.leaves;
       Crop.storage  = Crop.storage + Delta.storage;
       Crop.LAIExp   = Crop.LAIExp  + Delta.LAIExp;
       
       Crop.RootDepth_prev = Crop.RootDepth;
       Crop.RootDepth = Crop.RootDepth + Delta.RootDepth;

       /* go to the end of the list */
       while (Crop.LeaveProperties->next) 
              Crop.LeaveProperties = Crop.LeaveProperties->next;
    
       Crop.LeaveProperties->next = Delta.LeaveProperties; 
       Crop.LeaveProperties = previous;
       
       PhysAgeing = max(0., (Temp - TempBaseLeaves)/(35.- TempBaseLeaves));
       
       /* update the leave age for each age class */
       while (Crop.LeaveProperties->next) {
          Crop.LeaveProperties->age = Crop.LeaveProperties->age + PhysAgeing;
	  Crop.LeaveProperties      = Crop.LeaveProperties->next;}
	  
       /* return to beginning of the linked list */
       Crop.LeaveProperties = previous;	  
       
       //free(Delta);
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
 
Plant Growth(float NewPlantMaterial)
{
	Plant Delta;
        float shoots, FractionRoots;
	float DeathRoots, DeathStems;
		
	FractionRoots = Afgen(Roots, &DevelopmentStage);
	DeathRoots    = Crop.roots*Afgen(DeathRateRoots, &DevelopmentStage);
	Delta.roots   = NewPlantMaterial*FractionRoots - DeathRoots;
	
	shoots        = NewPlantMaterial*(1-FractionRoots);
	    
	DeathStems    = Crop.stems*Afgen(DeathRateStems, &DevelopmentStage);	
	Delta.stems   = shoots*Afgen(Stems, &DevelopmentStage)-DeathStems;
	
	Delta.storage = shoots*Afgen(Storage, &DevelopmentStage);
	
	Delta.leaves          = shoots*Afgen(Leaves, &DevelopmentStage);
	Delta.LeaveProperties = LeaveGrowth(Crop.LAIExp, Delta.leaves, &Delta.LAIExp);	
	Delta.leaves          = Delta.leaves - DyingLeaves();
	
        Delta.RootDepth = min(Crop.MaxRootingDepth-Crop.RootDepth,
                MaxIncreaseRoot*Step);
        
	return Delta;
}	
	

float RespirationRef(float TotalAssimilation)
{
      float respiration; 
      float TempRef = 25.;

      respiration  = RelRespiLeaves*Crop.leaves;
      respiration  = respiration + RelRespiStorage*Crop.storage;
      respiration  = respiration + RelRespiRoots*Crop.roots;	
      respiration  = respiration + RelRespiStems*Crop.stems;
      respiration  = respiration * Afgen(FactorSenescence, &DevelopmentStage);
      respiration  = respiration * pow(Q10, 0.1*(Temp-TempRef));
      
      return (min(respiration, TotalAssimilation));
}

Plant Initialize(int Emergence)
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
       
       Crop.roots     = InitialDryWeight*FractionRoots;
       Crop.RootDepth = InitRootingDepth;
       Crop.stems     = InitialShootWeight*Afgen(Stems, &DevelopmentStage);                   
       Crop.leaves    = InitialShootWeight*Afgen(Leaves, &DevelopmentStage);
       Crop.storage   = InitialShootWeight*Afgen(Storage, &DevelopmentStage);
       
       Crop.MaxRootingDepth = max(InitRootingDepth,min(MaxRootingDepth,
            WatBal.SoilMaxRootingDepth));

       LAIEmergence  = Crop.leaves*Afgen(SpecificLeaveArea, &DevelopmentStage); 
       
       Crop.LAIExp = LAIEmergence;
      
       LAI = LAIEmergence + Crop.stems*Afgen(SpecificStemArea, &DevelopmentStage) +
	     Crop.storage*SpecificPodArea;

       Crop.LeaveProperties         = malloc(sizeof (Green));
       Crop.LeaveProperties->age    = 0.;
       Crop.LeaveProperties->weight = Crop.leaves;
       Crop.LeaveProperties->area   = Afgen(SpecificLeaveArea, &DevelopmentStage);
       
       return Crop;
}       


Plant RateCalculationCrop()
{
       Plant Delta;

       float TotalAssimilation;
       float Maintenance, GrossAssimilation, GrossGrowth;    
       
       /* set Delta to 0 */
       Delta.roots = Delta.leaves = Delta.stems = Delta.storage = Delta.LAIExp =0.;      
       Delta.LeaveProperties = NULL;
      
       /* assimilation */
       GrossAssimilation = DailyTotalAssimilation(Astro());

       /* correction for low minimum temperatures */
       TotalAssimilation = Correct(GrossAssimilation);

       /* respiration */
       Maintenance = RespirationRef(TotalAssimilation);

       /* conversion */
       GrossGrowth = Conversion(TotalAssimilation-Maintenance); 
      
       /* Growth of root, stems, leaves and storage organs */
       Delta=Growth(GrossGrowth);
             
       printf("  Dmi: %5.1f MRes: %5.1f Gass: %5.1f", GrossGrowth, Maintenance, TotalAssimilation);

       return Delta;
}

int main(void)
{
  int  Emergence, EndDay = 240;
  Plant Delta;

  Emergence = 1;

  GetCropData(); 
  GetMeteoData();
  GetSoilData();
  GetSiteData();

  Day = 0;
  Crop     = Initialize(Emergence); 
  WatBalInitialize();
  
  while (DevelopmentStage <= DevelopStageHarvest && Day < EndDay) {
   
    Temp = 0.5*(Tmax[Day] + Tmin[Day]);
    DayTemp    = 0.5*(Tmax[Day] + Temp);
    
printf("\n%4d", Day); 
printf(" Stems: %7.0f", Crop.stems);
printf(" Leaves: %7.0f", Crop.leaves);
printf(" sto: %7.0f", Crop.storage); 
printf(" LAI: %7.2f", LAI);
printf(" dvs: %7.2f", DevelopmentStage); 


    Astro();
    CalcPenman();

    Delta            = RateCalculationCrop();
    WatBalRateCalulation();
    
    EulerIntegration(Delta);
    WatBal           = WatBalIntegration();
    
    LAI              = LeaveAreaIndex();
    DevelopmentStage = GetDevelopmentStage();
    
    Day++;

}
return 0;
}           
                      	     

