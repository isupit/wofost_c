#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wofost.h"
#include "extern.h"

/* ---------------------------------------------------------------------------*/
/*  function Growth(float NewPlantMaterial)                                   */
/*  Purpose: Establish growth rates of the plant organs (kg ha-1 d-1) and     */
/*  rooting depth (cm)                                                        */
/* ---------------------------------------------------------------------------*/

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
        Fraction_st = Afgen(Stems, &DevelopmentStage) + flv - Fraction_lv;
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
    Crop.RootDepth = min(Crop.MaxRootingDepth - Crop.RootDepth,
                MaxIncreaseRoot*Step);
}	
