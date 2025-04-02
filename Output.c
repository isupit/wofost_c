#include <stdio.h>
#include "extern.h"
#include "wofost.h"

void header(FILE *fp)
{
    fprintf(fp,"                Year   Day    DVS       WLV        WST          WSO         WRT         LAI     WSTRESS  SOILM    INF      Rain   NNI        PNI        KNI        NPKI  \n");
}

void Output(FILE *fp)
{
    fprintf(fp,"%7.2f,%7.2f,%4d, %4d, %7.5f,%11.5f,%11.5f,%11.5f,%11.4f,%10.4f,%7.2f,%7.3f,%7.2f,%7.1f,%10.5f,%10.5f,%10.5f,%10.5f\n",
        Meteo->lat,
        Meteo->lon,
        MeteoYear[Day],
        MeteoDay[Day],
        Crop->st.Development,
        Crop->st.leaves,
        Crop->st.stems,
        Crop->st.storage,
        Crop->st.roots,
        Crop->st.LAI,
        WatBal->WaterStress,
        WatBal->st.Moisture,
        WatBal->rt.Infiltration,
        Rain[Day],
        Crop->N_st.Indx,
        Crop->P_st.Indx,
        Crop->K_st.Indx,
        Crop->NPK_Indx);
}

