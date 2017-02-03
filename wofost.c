#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include "mmla.h"
#include "wofost.h"
#include "dynamic.h"
#include "extern.h"


Plant EulerIntegration(Plant Delta)	    
{
       float PhysAgeing;
       Plant New;
       Green *previous;

       previous = Crop.LeaveProperties;
       
       New.roots    = Crop.roots   + Delta.roots;
       New.stems    = Crop.stems   + Delta.stems;
       New.leaves   = Crop.leaves  + Delta.leaves;
       New.storage  = Crop.storage + Delta.storage;
       New.LAIExp   = Crop.LAIExp  + Delta.LAIExp;
       New.LeaveProperties = Crop.LeaveProperties;

       while (New.LeaveProperties->next) 
              New.LeaveProperties = New.LeaveProperties->next;
    
       New.LeaveProperties->next = Delta.LeaveProperties; 
       New.LeaveProperties = previous;
       
       PhysAgeing = max(0., (Temp - TempBaseLeaves)/(35.- TempBaseLeaves));
       
       while (New.LeaveProperties->next) {
          New.LeaveProperties->age = New.LeaveProperties->age + PhysAgeing;
	  New.LeaveProperties      = New.LeaveProperties->next;}
	  
       /* return to beginning linked list */
       New.LeaveProperties = previous;	  
       
       return New;
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
        float shoots, FractionRoots, LAIExp;
	float DeathRoots, DeathStems, DeathLeaves;
		
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
	    {DeltaTempSum=limit(0, TempEffMax-TempBaseEmergence, Temp-TempBaseEmergence);
	     TempSum = TempSum + DeltaTempSum;
	     Day++;
	     }
	}	     
       
       DevelopmentStage = InitialDVS;
       
       FractionRoots      = Afgen(Roots, &DevelopmentStage);
       FractionShoots     = 1 - FractionRoots;
       InitialShootWeight = InitialDryWeight*FractionShoots;
       
       Crop.roots     = InitialDryWeight*FractionRoots;
       Crop.rootdepth = InitRootingDepth;
       Crop.stems     = InitialShootWeight*Afgen(Stems, &DevelopmentStage);                   
       Crop.leaves    = InitialShootWeight*Afgen(Leaves, &DevelopmentStage);
       Crop.storage   = InitialShootWeight*Afgen(Storage, &DevelopmentStage);

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


Soil WatBalInitialize(int Emergence)
{
    /* Check initial soil moisture. It cannot be larger than the    */
    /* soil moisture SoilMoistureSAT or smaller than SoilMoistureWP */
    
    if (InitSoilMoisture < SoilMoistureWP)  InitSoilMoisture = SoilMoistureWP;
    if (InitSoilMoisture > SoilMoistureSAT) InitSoilMoisture = SoilMoistureSAT;
    
    if (Airducts == 1) InitSoilMoisture = SoilMoistureSAT; /* initial soil moisture for a rice crop */
    SoilMoisture = limit(SoilMoistureWP, InitSoilMoisture, SoilMoistureWP +
            AvailableSoilM/Crop.rootdepth);
    
    /* Initial moisture amount in rooted zone */
    AvailRootZoneMoisture = SoilMoisture*Crop.rootdepth;
    //InitAvailMoisture     = AvailRootZoneMoisture;
    
    /*  Soil evaporation, days since last rain */
    DaysSinceLastRain = 1.;
    if (SoilMoisture <= (SoilMoistureWP+0.5*(SoilMoistureFC-SoilMoistureWP))) 
        DaysSinceLastRain=5.;
    
    
    /* Moisture amount between rooted zone and max.rooting depth */
    MoistureLOW  = limit (0., SoilMoistureSAT*(MaxRootingDepth-Crop.rootdepth), 
            AvailableSoilM + MaxRootingDepth*SoilMoistureWP - 
   
    
    /*  all summation variables of the water balance are set at zero. */
    TRAT   = 0.;
    EVST   = 0.;
    EVWT   = 0.;
    TSR    = 0.;
    RAINT  = 0.;
    WDRT   = 0.;
    TOTINF = 0.;
    TOTIRR = 0.;
    SUMSM  = 0.;
    PERCT  = 0.;
    LOSST  = 0.;

    return WatBal;
}

Soil  RateCalculationSoil()
{
    Soil DeltaWatBal;
    
    DeltaWatBal.Transpiration     = 0.;
    DeltaWatBal.EvaporationWater  = 0.;
    DeltaWatBal.EvaporationSoil   = 0.; 
    DeltaWatBal.Transpiration     = 0.;     
    DeltaWatBal.Rain              = 0.;
    DeltaWatBal.Infiltration      = 0.;
    DeltaWatBal.Percolation       = 0.;
    DeltaWatBal.Irrigation        = 0.;
    DeltaWatBal.IncreaseWaterRootGrowth = 0.;
    DeltaWatBal.Loss              = 0.;
    
    /* if surface storage > 1 cm */
    if (DeltaWatBal.SurfaceStorage > 1.) DeltaWatBal.EvaporationWater = EvaporationWaterMax;
    
    return DeltaWatBal;
}


int main(void)
{
  int  Emergence, EndDay = 240;
  Plant Delta;
  Soil  DeltaWatBal;

  Emergence = 1;

  GetCropData(); 
  GetMeteoData();
  GetSoildata()

  Day = 0;
  Crop     = Initialize(Emergence); 
  WatBal   = WatBalInitialize(Emergence);
  
  while (DevelopmentStage <= DevelopStageHarvest && Day < EndDay) {
   
    Temp = 0.5*(Tmax[Day] + Tmin[Day]);
    DayTemp    = 0.5*(Tmax[Day] + Temp);
    
printf("\n%4d", Day); 
printf(" Stems: %7.0f", Crop.stems);
printf(" Leaves: %7.0f", Crop.leaves);
printf(" sto: %7.0f", Crop.storage); 
printf(" LAI: %7.2f", LAI);
printf(" dvs: %7.2f", DevelopmentStage); 

    Delta            = RateCalculationCrop();
    DeltaWatBal      = RateCalculationSoil();
    Crop             = EulerIntegration(Delta);
    LAI              = LeaveAreaIndex();
    DevelopmentStage = GetDevelopmentStage();
    Day++;

}
return 0;
}           
                      	     

