#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "wofost.h"
#include "extern.h"

/* ---------------------------------------------------------------------------*/
/*  function InitializeCrop                                                     */
/*  Purpose: Set the initial crop state and leave variables, the water balance, */
/*  and the nutrients-----------------------------------------------------------*/ 

void InitializeCrop(int *Emergence)
{
        float FractionRoots;
        float FractionShoots; 
        float InitialShootWeight;
        float DeltaTempSum;
         
        /*  Emergence has not taken place yet*/
        if (!*Emergence)
    	{
            DeltaTempSum = limit(0, Crop->prm.TempEffMax-Crop->prm.TempBaseEmergence, 
                    Temp-Crop->prm.TempBaseEmergence);
	    Crop->TSumEmergence += DeltaTempSum;
	    if (Crop->TSumEmergence >= Crop->prm.TSumEmergence)
            {
                *Emergence = 1;
	    }
	}	     
       
        /*  Emergence has occurred */
        if (*Emergence)
        {
            /* Initialize the crop states */
            Crop->DevelopmentStage = Crop->prm.InitialDVS;

            FractionRoots      = Afgen(Crop->prm.Roots, &(Crop->DevelopmentStage));
            FractionShoots     = 1 - FractionRoots;
            InitialShootWeight = Crop->prm.InitialDryWeight * FractionShoots;

            Crop->st.roots     = Crop->prm.InitialDryWeight * FractionRoots;
            Crop->RootDepth    = Crop->prm.InitRootingDepth;
            Crop->st.stems     = InitialShootWeight * Afgen(Crop->prm.Stems, &(Crop->DevelopmentStage));                   
            Crop->st.leaves    = InitialShootWeight * Afgen(Crop->prm.Leaves, &(Crop->DevelopmentStage));
            Crop->st.storage   = InitialShootWeight * Afgen(Crop->prm.Storage, &(Crop->DevelopmentStage));

            Crop->prm.MaxRootingDepth = max(Crop->prm.InitRootingDepth, min(Crop->prm.MaxRootingDepth,
                 Site->SoilLimRootDepth));

            Crop->prm.LAIEmergence  = Crop->st.leaves * Afgen(Crop->prm.SpecificLeaveArea, &(Crop->DevelopmentStage)); 

            Crop->st.LAIExp = Crop->prm.LAIEmergence;

            Crop->st.LAI = Crop->prm.LAIEmergence + Crop->st.stems * 
                   Afgen(Crop->prm.SpecificStemArea, &(Crop->DevelopmentStage)) +
                   Crop->st.storage*Crop->prm.SpecificPodArea;

            Crop->LeaveProperties         = malloc(sizeof (Green));
            Crop->LeaveProperties->age    = 0.;
            Crop->LeaveProperties->weight = Crop->st.leaves;
            Crop->LeaveProperties->area   = Afgen(Crop->prm.SpecificLeaveArea, &(Crop->DevelopmentStage));
            Crop->LeaveProperties->next   = NULL;
            
            
            Crop->Emergence = 1;
            Crop->GrowthDay = 1;
            
            /* Initialize the water balance */
            InitializeWatBal();
            
            /* Initialize the crop nutrients */
            InitializeNutrients();
        }
}  