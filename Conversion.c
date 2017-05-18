#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include "wofost.h"
#include "extern.h"

/* ---------------------------------------------------------------------------*/
/*  function Conversion                                                       */
/*  Purpose: Convert the net assimilation  into plant matter                  */
/* ---------------------------------------------------------------------------*/

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