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
       float FractionRoots, FractionShoots, InitialShootWeight;
       float DeltaTempSum, TempSum=0;
             
       if (!Emergence)
    	{
	while (TempSum < TSumEmergence) 
	    {DeltaTempSum=limit(0, TempEffMax-TempBaseEmergence, 
                    Temp-TempBaseEmergence);
	     TempSum += DeltaTempSum;
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