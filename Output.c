#include <stdio.h>
#include <time.h>
#include "extern.h"
#include "wofost.h"

void header(FILE *fp)
{
    fprintf(fp,"Date         Date   WST     WLV	    dWLV    WSO    dWST    LAI      DVS  WSTRESS  SOILM    INF      Rain NSTRESS  \n");
}

void Output(FILE *fp)
{
    fprintf(fp,"%4d-%02d-%02d,%4d,%10.5f,%10.5f,%10.5f,%10.5f,%10.5f,%9.4f,%9.4f,%7.2f,%7.3f,%7.2f,%7.1f,,%7.2f\n",
        simTime.tm_year + 1900, simTime.tm_mon +1, simTime.tm_mday,
        Day,
        Crop->st.stems,
        Crop->st.leaves,
        Crop->dst.leaves,
        Crop->st.storage,
        Crop->dst.stems,
        Crop->st.LAI,
        Crop->st.Development,
        WatBal->WaterStress,
        WatBal->st.Moisture,
        WatBal->rt.Infiltration,
        Rain[Day],
        Crop->NutrientStress);
   }

