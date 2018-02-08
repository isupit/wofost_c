#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "wofost.h"
#include "extern.h"

/* ---------------------------------------------------------------------------*/
/*  function InitializeCrop                                                   */
/*  Purpose: Set the initial crop state and leave variables                   */
/* ---------------------------------------------------------------------------*/ 
void InitializeCrop(int Emergence)
{
       float FractionRoots;
       float FractionShoots; 
       float InitialShootWeight;
       float DeltaTempSum;
       float TempSum=0;
             
       if (!Emergence)
    	{
	while (TempSum < Crop.prm.TSumEmergence) 
	    {DeltaTempSum = limit(0, Crop.prm.TempEffMax-Crop.prm.TempBaseEmergence, 
                    Temp-Crop.prm.TempBaseEmergence);
	     TempSum += DeltaTempSum;
	     Day++;
             simTime.tm_mday++;
	     }
	}	     
       
       DevelopmentStage = Crop.prm.InitialDVS;
       
       FractionRoots      = Afgen(Crop.prm.Roots, &DevelopmentStage);
       FractionShoots     = 1 - FractionRoots;
       InitialShootWeight = Crop.prm.InitialDryWeight * FractionShoots;
       
       Crop.st.roots     = Crop.prm.InitialDryWeight * FractionRoots;
       Crop.RootDepth    = Crop.prm.InitRootingDepth;
       Crop.st.stems     = InitialShootWeight * Afgen(Crop.prm.Stems, &DevelopmentStage);                   
       Crop.st.leaves    = InitialShootWeight * Afgen(Crop.prm.Leaves, &DevelopmentStage);
       Crop.st.storage   = InitialShootWeight * Afgen(Crop.prm.Storage, &DevelopmentStage);
       
       Crop.MaxRootingDepth = max(Crop.prm.InitRootingDepth,min(Crop.prm.MaxRootingDepth,
            Site.SoilLimRootDepth));

       Crop.prm.LAIEmergence  = Crop.st.leaves * Afgen(Crop.prm.SpecificLeaveArea, &DevelopmentStage); 
       
       Crop.st.LAIExp = Crop.prm.LAIEmergence;
      
       LAI = Crop.prm.LAIEmergence + Crop.st.stems * Afgen(Crop.prm.SpecificStemArea, &DevelopmentStage) +
	     Crop.st.storage*Crop.prm.SpecificPodArea;

       Crop.LeaveProperties         = malloc(sizeof (Green));
       Crop.LeaveProperties->age    = 0.;
       Crop.LeaveProperties->weight = Crop.st.leaves;
       Crop.LeaveProperties->area   = Afgen(Crop.prm.SpecificLeaveArea, &DevelopmentStage);
       Crop.LeaveProperties->next   = NULL;   
}  