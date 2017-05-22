#include <stdio.h>
#include <stdlib.h>
#include "wofost.h"
#include "extern.h"

/* ----------------------------------------------------------------------------------*/
/*  function Conversion                                                              */
/*  Purpose: Convert the net assimilation products into plant dry matter kg ha-1 d-1 */
/* ----------------------------------------------------------------------------------*/

float Conversion(float NetAssimilation)
{
	float fr, root, shoots;
	
	fr    = Afgen(Roots, &DevelopmentStage);
	root  = fr/ConversionRoots;
	shoots =  Afgen(Stems, &DevelopmentStage)/ConversionStems;
	shoots += Afgen(Leaves, &DevelopmentStage)/ConversionLeaves;	
	shoots += Afgen(Storage, &DevelopmentStage)/ConversionStorage;
	
	/* conversion */
	return NetAssimilation/(shoots*(1-fr)+root);
}