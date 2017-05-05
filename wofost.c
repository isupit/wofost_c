#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include "mmla.h"
#include "wofost.h"
#include "dynamic.h"
#include "extern.h"


void IntegrationCrop()	    
{
    float PhysAgeing;
    Green *LeaveProperties;
    
    Crop.st.roots    = + Crop.rt.roots;
    Crop.st.stems    = + Crop.rt.stems;
    Crop.st.leaves   = + Crop.rt.leaves;
    Crop.st.storage  = + Crop.rt.storage;
    Crop.st.LAIExp   = + Crop.rt.LAIExp;

    /* Establish the age increase */
    PhysAgeing = max(0., (Temp - TempBaseLeaves)/(35.- TempBaseLeaves));
    
    /* Store the initial address */
    LeaveProperties = Crop.LeaveProperties;
    
    /* Update the leave age for each age class */
    while (Crop.LeaveProperties->next) {
       Crop.LeaveProperties->age = Crop.LeaveProperties->age + PhysAgeing;
    Crop.LeaveProperties      = Crop.LeaveProperties->next;}
  
    /* Return to beginning of the linked list */
    Crop.LeaveProperties = LeaveProperties;	 
           
    /* Actual N amount in various living organs and total living N amount(kg N ha-1) */
    Crop.N_st.leaves  =+ Crop.N_rt.leaves;
    Crop.N_st.stems   =+ Crop.N_rt.stems;
    Crop.N_st.roots   =+ Crop.N_rt.roots;
    Crop.N_st.storage =+ Crop.N_rt.storage;
    
    /* Actual P amount in various living organs and total living P amount(kg N ha-1) */
    Crop.P_st.leaves  =+ Crop.P_rt.leaves;
    Crop.P_st.stems   =+ Crop.P_rt.stems;
    Crop.P_st.roots   =+ Crop.P_rt.roots;
    Crop.P_st.storage =+ Crop.P_rt.storage;
    
    /* Actual K amount in various living organs and total living k amount(kg N ha-1) */
    Crop.K_st.leaves  =+ Crop.K_rt.leaves;
    Crop.K_st.stems   =+ Crop.K_rt.stems;
    Crop.K_st.roots   =+ Crop.K_rt.roots;
    Crop.K_st.storage =+ Crop.K_rt.storage;

}       	     

float Conversion(float NetAssimilation)
{
	float fr, root, shoots;
	
	fr    = Afgen(Roots, &DevelopmentStage);
	root  = fr/ConversionRoots;
	shoots =  Afgen(Stems, &DevelopmentStage)/ConversionStems;
	shoots =+ Afgen(Leaves, &DevelopmentStage)/ConversionLeaves;	
	shoots =+ Afgen(Storage, &DevelopmentStage)/ConversionStorage;
	
	/* conversion */
	return NetAssimilation/(shoots*(1-fr)+root);
}
 
void Growth(float NewPlantMaterial)
{
    float shoots, factor, flv;
    float Fraction_ro, Fraction_lv, Fraction_st, Fraction_so;
        
        
    /* Water stress is more severe as compared to Nitrogen stress and */
    /* partitioning will follow the original assumptions of LINTUL2   */     
        
    if (WatBal.WaterStress < Crop.N_st.Indx)
    {
        factor = max(1., 1./(WatBal.WaterStress + 0.5));
        Fraction_ro = min(0.6, Afgen(Roots, &DevelopmentStage) * factor);
        Fraction_lv = Afgen(Leaves, &DevelopmentStage);
        Fraction_st = Afgen(Stems, &DevelopmentStage);
        Fraction_so = Afgen(Storage, &DevelopmentStage);
    }
    else
    {
        flv = Afgen(Leaves, &DevelopmentStage);
        factor = exp(-N_lv_partitioning * ( 1. - Crop.N_st.Indx));
        
        Fraction_lv = flv * factor;
        Fraction_ro = Afgen(Roots, &DevelopmentStage);
        Fraction_st = Afgen(Stems, &DevelopmentStage) - flv - Fraction_lv;
        Fraction_so = Afgen(Storage, &DevelopmentStage);
    }
                
    Crop.drt.roots = Crop.st.roots*Afgen(DeathRateRoots, &DevelopmentStage);
    Crop.rt.roots  = NewPlantMaterial*Fraction_ro - Crop.drt.roots;
	
    shoots         = NewPlantMaterial*(1-Fraction_ro);
	    
    Crop.drt.stems = Crop.st.stems*Afgen(DeathRateStems, &DevelopmentStage);	
    Crop.rt.stems  = shoots*Fraction_st - Crop.drt.stems;
	
    Crop.rt.storage = shoots * Fraction_so;
	
    Crop.drt.leaves = DyingLeaves(); 
    Crop.rt.leaves  = shoots * Fraction_lv;
    Crop.rt.LAIExp  = LeaveGrowth(Crop.st.LAIExp, Crop.rt.leaves);	
    Crop.rt.leaves  = Crop.rt.leaves -  Crop.drt.leaves;
	
    Crop.RootDepth_prev = Crop.RootDepth;
    Crop.RootDepth = min(Crop.MaxRootingDepth-Crop.RootDepth,
                MaxIncreaseRoot*Step);
}	
	

float RespirationRef(float TotalAssimilation)
{
      float respiration; 
      float TempRef = 25.;

      respiration  = RelRespiLeaves*Crop.st.leaves;
      respiration  =+ RelRespiStorage*Crop.st.storage;
      respiration  =+ RelRespiRoots*Crop.st.roots;	
      respiration  =+ RelRespiStems*Crop.st.stems;
      respiration  = respiration * Afgen(FactorSenescence, &DevelopmentStage);
      respiration  = respiration * pow(Q10, 0.1*(Temp-TempRef));
      
      return (min(respiration, TotalAssimilation));
}

void InitializeCrop(int Emergence)
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
       float Stress;
       
       /* Set rates to 0 */
       Crop.rt.roots   = 0.;
       Crop.rt.leaves  = 0.;
       Crop.rt.stems   = 0.;
       Crop.rt.storage = 0.;
       Crop.rt.LAIExp  = 0.;      
      
       /* Assimilation */
       GrossAssimilation = DailyTotalAssimilation(Astro());
       
       /* Stress: either nutrient shortage or water shortage */
       Stress = min(Crop.NutrientStress, WatBal.WaterStress);

       /* Correction for low minimum temperatures and stress factors */
       TotalAssimilation = Stress * Correct(GrossAssimilation);       

       /* Respiration */
       Maintenance = RespirationRef(TotalAssimilation);

       /* Conversion */
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
  GetManagement();

  Day = 0;
  InitializeCrop(Emergence); 
  InitializeWatBal();
  InitializeNutrients();
  
  while (DevelopmentStage <= DevelopStageHarvest && Day < EndDay) 
  {
   
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
                      	     

