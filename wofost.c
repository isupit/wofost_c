#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include "wofost.h"
#include "extern.h"
#include <time.h>

int main() {
    FILE *ifp;
    FILE **file;
    
    SimUnit *Grid = NULL;
    SimUnit *initial = NULL;
    SimUnit *GridHead = NULL;
    
    Green *LeaveProperties = NULL;
    
    int Emergence;
    int Start;
    int CycleLength   = 240;
    int count;

    char path[100];
    char cropfile[100];
    char soilfile[100];
    char sitefile[100];
    char management[100];
    char dateString [100];
    char place[15];
    char name[100];
    
    char cf[100], sf[100], mf[100], site[100];
  
    Step = 1.;    
    
    ifp = fopen("list.txt", "r");

    if (ifp == NULL) 
    {
        fprintf(stderr, "Can't open input list.txt\n");
        exit(1);
    }
    
    count = 0;
    while (fscanf(ifp,"%7s %10s %7s %12s %10s %10s %2s %d %d" ,
            path, cf, sf, mf, site, dateString, place, &Start, &Emergence)
            != EOF) 
    {    
        strncpy(cropfile, path, 98);
        strncat(cropfile, cf, 98);

        strncpy(soilfile, path, 98);
        strncat(soilfile, sf, 98);

        strncpy(management, path, 98);
        strncat(management, mf, 98);

        strncpy(sitefile, path, 98);
        strncat(sitefile, site, 98);

        /*printf("path: %7s\n", path);
        printf("cropfile: %10s\n",cropfile);
        printf("soilfile: %7s\n",soilfile);
        printf("management: %12s\n", management);
        printf("sitefile: %10s\n", sitefile);
        printf("dateString: %10s\n", dateString);
        printf("place: %2s\n", place);
        printf("Start: %2d\n", Start);
        printf("Emergence: %2d\n", Emergence);*/
        
        /* count the number of output files */
        /* number is the index number of the list of file pointers */

        
        if (initial == NULL) 
        {
            Grid = initial =  malloc(sizeof(SimUnit));
            Grid->crp  = malloc(sizeof(Plant)); 
            Grid->ste  = malloc(sizeof(Field));
            Grid->mng  = malloc(sizeof(Management));
            Grid->soil = malloc(sizeof(Soil)); 
            Grid->start = Start;
            Grid->file = count++;
            strcpy(Grid->name,cf);
            Grid->emergence = Emergence;
            Grid->next = NULL; 
            
            GetCropData(Grid->crp, cropfile);
            GetSiteData(Grid->ste, sitefile);
            GetManagement(Grid->mng, management);
            GetSoilData(Grid->soil, soilfile);
        }
        else 
        {
            Grid->next = malloc(sizeof(SimUnit));
            Grid = Grid->next;
            Grid->crp  = malloc(sizeof(Plant));
            Grid->ste  = malloc(sizeof(Field));
            Grid->mng  = malloc(sizeof(Management));
            Grid->soil = malloc(sizeof(Soil)); 
            Grid->start = Start;            // Start day (=daynumber)
            Grid->file  = count++;          // number of elements in Grid carousel
            strcpy(Grid->name,cf);          // Crop file name
            Grid->emergence = Emergence;    // Start the simulations at emergence (1) or at sowing (0)
            Grid->next = NULL;
            
            GetCropData(Grid->crp, cropfile);
            GetSiteData(Grid->ste, sitefile);
            GetManagement(Grid->mng, management);
            GetSoilData(Grid->soil, soilfile);
        }
    }
    
    /* Set Grid back to the initial address */
    Grid = initial;   
    
    /* Get the meteodata */
    GetMeteoData(path, dateString, place);
    
    /* allocate memory for the file pointers */
    file = malloc(sizeof(*file) * --count);
    
    /* open the output files */
    while (Grid)
    {
        memcpy(name, Grid->name, strlen(Grid->name)-4);
        file[Grid->file] = fopen(name, "w");
        Grid = Grid->next;
    }
    
    
    for (Day = 1; Day <365; Day++)
    {        
        /* Go back to the beginning of the list */
        Grid = initial;
        
        Temp = 0.5 * (Tmax[Day] + Tmin[Day]);
        DayTemp = 0.5 * (Tmax[Day] + Temp);
        
        Astro();
        CalcPenman();
        
        while (Grid)
        {
            /* Get data, states and rates from the Grid structure and */
            /* put them in the place holders */
            Crop      = Grid->crp;
            WatBal    = Grid->soil;
            Mng       = Grid->mng;
            Site      = Grid->ste;
            Start      = Grid->start;
            Emergence  = Grid->emergence;

            
            if (Day >= Start && Crop->Emergence == 0)
            {
                InitializeCrop(&Emergence);
            }
            
            if (Day >= Start && Crop->Emergence == 1)
            {      
                
                if (Crop->DevelopmentStage <= Crop->prm.DevelopStageHarvest && Crop->GrowthDay < CycleLength) 
                {
                    /* Rate calculations */
                    //RateCalulationWatBal();
                    //RateCalcultionNutrients();
                    //RateCalculationCrop();

                    /* State calculations */
                    //Crop->st.LAI = LeaveAreaIndex();
                    //Crop->DevelopmentStage = GetDevelopmentStage();
                    
                    //IntegrationWatBal();
                    //IntegrationNutrients();
                    //IntegrationCrop();
                    
                    //fprintf(file[Grid->file],"\n%4d-%02d-%02d,%4d,%7.0f,%7.0f,%7.0f,%7.2f,%7.2f",
                    //    simTime.tm_year + 1900, simTime.tm_mon +1, simTime.tm_mday,
                    //    Day,Crop->st.stems,Crop->st.leaves,Crop->st.storage,
                    //    Crop->st.LAI,Crop->DevelopmentStage);
                    
                    /* Update the number of days that the crop has grown*/
                    Crop->GrowthDay++;
                }
            }

            /* Store the daily calculations in the Grid structure */
            Grid->crp  = Crop;
            Grid->soil = WatBal;
            Grid->mng  = Mng;
            Grid->ste  = Site;
            Grid = Grid->next;
        }
    
    /* Update time */
    simTime.tm_mday++;
    mktime(&simTime);
    
    }
     
 /* Go back to the beginning of the list */
 Grid = initial;
 
while (Grid)
{
    fclose(file[Grid->file]);
   
    while(Grid->crp->prm.DeltaTempSum)
    {
        head = Grid->crp->prm.DeltaTempSum;
        Grid->crp->prm.DeltaTempSum = Grid->crp->prm.DeltaTempSum->next;
        free(head);
    }
    free(Grid->crp->prm.DeltaTempSum);
    Grid->crp->prm.DeltaTempSum = NULL;
    
    
    while(Grid->crp->prm.SpecificLeaveArea)
    {
        head = Grid->crp->prm.SpecificLeaveArea;
        Grid->crp->prm.SpecificLeaveArea = Grid->crp->prm.SpecificLeaveArea->next;
        free(head);
    }
    free(Grid->crp->prm.SpecificLeaveArea);
    Grid->crp->prm.SpecificLeaveArea = NULL;
    
    
    while(Grid->crp->prm.SpecificStemArea)
    {
        head = Grid->crp->prm.SpecificStemArea;
        Grid->crp->prm.SpecificStemArea = Grid->crp->prm.SpecificStemArea->next;
        free(head);
    }
    free(Grid->crp->prm.SpecificStemArea);
    Grid->crp->prm.SpecificStemArea = NULL;
    
    
    while(Grid->crp->prm.KDiffuseTb)
    {
        head = Grid->crp->prm.KDiffuseTb;
        Grid->crp->prm.KDiffuseTb = Grid->crp->prm.KDiffuseTb->next;
        free(head);
    }
    free(Grid->crp->prm.KDiffuseTb);
    Grid->crp->prm.KDiffuseTb = NULL;
    
    while(Grid->crp->prm.EFFTb)
    {
        head = Grid->crp->prm.EFFTb;
        Grid->crp->prm.EFFTb = Grid->crp->prm.EFFTb->next;
        free(head);
    }
    free(Grid->crp->prm.EFFTb);
    Grid->crp->prm.EFFTb = NULL;
    
    while(Grid->crp->prm.MaxAssimRate)
    {
        head = Grid->crp->prm.MaxAssimRate;
        Grid->crp->prm.MaxAssimRate = Grid->crp->prm.MaxAssimRate->next;
        free(head);
    }
    free(Grid->crp->prm.MaxAssimRate);
    Grid->crp->prm.MaxAssimRate = NULL;
    
    while(Grid->crp->prm.FactorAssimRateTemp)
    {
        head = Grid->crp->prm.FactorAssimRateTemp;
        Grid->crp->prm.FactorAssimRateTemp = Grid->crp->prm.FactorAssimRateTemp->next;
        free(head);
    }
    free(Grid->crp->prm.FactorAssimRateTemp);
    Grid->crp->prm.FactorAssimRateTemp = NULL; 

    while(Grid->crp->prm.FactorGrossAssimTemp)
    {
        head = Grid->crp->prm.FactorGrossAssimTemp;
        Grid->crp->prm.FactorGrossAssimTemp = Grid->crp->prm.FactorGrossAssimTemp->next;
        free(head);
    }
    free(Grid->crp->prm.FactorGrossAssimTemp);
    Grid->crp->prm.FactorGrossAssimTemp = NULL;
    
    
     while(Grid->crp->prm.CO2AMAXTB)
    {
        head = Grid->crp->prm.CO2AMAXTB;
        Grid->crp->prm.CO2AMAXTB = Grid->crp->prm.CO2AMAXTB->next;
        free(head);
    }
    free(Grid->crp->prm.CO2AMAXTB);
    Grid->crp->prm.CO2AMAXTB = NULL;
    

    while(Grid->crp->prm.CO2EFFTB)
    {
        head = Grid->crp->prm.CO2EFFTB;
        Grid->crp->prm.CO2EFFTB = Grid->crp->prm.CO2EFFTB->next;
        free(head);
    }
    free(Grid->crp->prm.CO2EFFTB);
    Grid->crp->prm.CO2EFFTB = NULL;
    
    
     while(Grid->crp->prm.CO2TRATB)
    {
        head = Grid->crp->prm.CO2TRATB;
        Grid->crp->prm.CO2TRATB = Grid->crp->prm.CO2TRATB->next;
        free(head);
    }
    free(Grid->crp->prm.CO2TRATB);
    Grid->crp->prm.CO2TRATB = NULL;
    

    while(Grid->crp->prm.FactorSenescence)
    {
        head = Grid->crp->prm.FactorSenescence;
        Grid->crp->prm.FactorSenescence = Grid->crp->prm.FactorSenescence->next;
        free(head);
    }
    free(Grid->crp->prm.FactorSenescence);
    Grid->crp->prm.FactorSenescence = NULL;
    
    
     while(Grid->crp->prm.Roots)
    {
        head = Grid->crp->prm.Roots;
        Grid->crp->prm.Roots = Grid->crp->prm.Roots->next;
        free(head);
    }
    free(Grid->crp->prm.Roots);
    Grid->crp->prm.Roots = NULL;


    while(Grid->crp->prm.Leaves)
    {
        head = Grid->crp->prm.Leaves;
        Grid->crp->prm.Leaves = Grid->crp->prm.Leaves->next;
        free(head);
    }
    free(Grid->crp->prm.Leaves);
    Grid->crp->prm.Leaves = NULL;
    
    
     while(Grid->crp->prm.Stems)
    {
        head = Grid->crp->prm.Stems;
        Grid->crp->prm.Stems = Grid->crp->prm.Stems->next;
        free(head);
    }
    free(Grid->crp->prm.Stems);
    Grid->crp->prm.Stems = NULL;
    
    
    while(Grid->crp->prm.Storage)
    {
        head = Grid->crp->prm.Storage;
        Grid->crp->prm.Storage = Grid->crp->prm.Storage->next;
        free(head);
    }
    free(Grid->crp->prm.Storage);
    Grid->crp->prm.Storage = NULL;
    
    
    while(Grid->crp->prm.DeathRateStems)
    {
        head = Grid->crp->prm.DeathRateStems;
        Grid->crp->prm.DeathRateStems = Grid->crp->prm.DeathRateStems->next;
        free(head);
    }
    free(Grid->crp->prm.DeathRateStems);
    Grid->crp->prm.DeathRateStems = NULL;
    
 
    while(Grid->crp->prm.DeathRateRoots)
    {
        head = Grid->crp->prm.DeathRateRoots;
        Grid->crp->prm.DeathRateRoots = Grid->crp->prm.DeathRateRoots->next;
        free(head);
    }
    free(Grid->crp->prm.DeathRateRoots);
    Grid->crp->prm.DeathRateRoots = NULL;
    
    
     while(Grid->crp->prm.N_MaxLeaves)
    {
        head = Grid->crp->prm.N_MaxLeaves;
        Grid->crp->prm.N_MaxLeaves = Grid->crp->prm.N_MaxLeaves->next;
        free(head);
    }
    free(Grid->crp->prm.N_MaxLeaves);
    Grid->crp->prm.N_MaxLeaves = NULL;
    
    
     while(Grid->crp->prm.P_MaxLeaves)
    {
        head = Grid->crp->prm.P_MaxLeaves;
        Grid->crp->prm.P_MaxLeaves = Grid->crp->prm.P_MaxLeaves->next;
        free(head);
    }
    free(Grid->crp->prm.P_MaxLeaves);
    Grid->crp->prm.P_MaxLeaves = NULL;

    
     while(Grid->crp->prm.K_MaxLeaves)
    {
        head = Grid->crp->prm.K_MaxLeaves;
        Grid->crp->prm.K_MaxLeaves = Grid->crp->prm.K_MaxLeaves->next;
        free(head);
    }
    free(Grid->crp->prm.K_MaxLeaves);
    Grid->crp->prm.K_MaxLeaves = NULL;
    
    
      while(Grid->soil->VolumetricSoilMoisture)
    {
        head = Grid->soil->VolumetricSoilMoisture;
        Grid->soil->VolumetricSoilMoisture = Grid->soil->VolumetricSoilMoisture->next;
        free(head);
    }
    free(Grid->soil->VolumetricSoilMoisture);
    Grid->soil->VolumetricSoilMoisture = NULL;
    
    
     while(Grid->soil->HydraulicConductivity)
    {
        head = Grid->soil->HydraulicConductivity;
        Grid->soil->HydraulicConductivity = Grid->soil->HydraulicConductivity->next;
        free(head);
    }
    free(Grid->soil->HydraulicConductivity);
    Grid->soil->HydraulicConductivity = NULL;

    
    while(Grid->mng->N_Fert_table)
    {
        head = Grid->mng->N_Fert_table;
        Grid->mng->N_Fert_table = Grid->mng->N_Fert_table->next;
        free(head);
    }
    free(Grid->mng->N_Fert_table);
    Grid->mng->N_Fert_table = NULL;
    
  
    while(Grid->mng->P_Fert_table)
    {
        head = Grid->mng->P_Fert_table;
        Grid->mng->P_Fert_table = Grid->mng->P_Fert_table->next;
        free(head);
    }
    free(Grid->mng->P_Fert_table);
    Grid->mng->P_Fert_table = NULL;
    
    while(Grid->mng->K_Fert_table)
    {
        head = Grid->mng->K_Fert_table;
        Grid->mng->K_Fert_table = Grid->mng->K_Fert_table->next;
        free(head);
    }
    free(Grid->mng->K_Fert_table);
    Grid->mng->K_Fert_table = NULL;
    

    while(Grid->mng->N_Uptake_frac)
    {
        head = Grid->mng->N_Uptake_frac;
        Grid->mng->N_Uptake_frac = Grid->mng->N_Uptake_frac->next;
        free(head);
    }
    free(Grid->mng->N_Uptake_frac);
    Grid->mng->N_Uptake_frac = NULL;
    
  
    while(Grid->mng->P_Uptake_frac)
    {
        head = Grid->mng->P_Uptake_frac;
        Grid->mng->P_Uptake_frac = Grid->mng->P_Uptake_frac->next;
        free(head);
    }
    free(Grid->mng->P_Uptake_frac);
    Grid->mng->P_Uptake_frac = NULL;
    
    
    while(Grid->mng->K_Uptake_frac)        
    {
        head = Grid->mng->K_Uptake_frac;
        Grid->mng->K_Uptake_frac = Grid->mng->K_Uptake_frac->next;
        free(head);
    }
    free(Grid->mng->K_Uptake_frac);
    Grid->mng->K_Uptake_frac = NULL;
    
    
     while(Grid->mng->Irrigation)        
    {
        head = Grid->mng->Irrigation;
        Grid->mng->Irrigation = Grid->mng->Irrigation->next;
        free(head);
    }
    free(Grid->mng->Irrigation);
    Grid->mng->Irrigation = NULL;
    
    
    while(Grid->ste->NotInfTB)        
    {
        head = Grid->ste->NotInfTB;
        Grid->ste->NotInfTB = Grid->ste->NotInfTB->next;
        free(head);
    }
    free(Grid->ste->NotInfTB);
    Grid->ste->NotInfTB =  NULL;
    
   
    /* Loop until the last element in the list and free each node */
    while (Grid->crp->LeaveProperties)
    {
        LeaveProperties = Grid->crp->LeaveProperties;
        Grid->crp->LeaveProperties = Grid->crp->LeaveProperties->next; 

        free(LeaveProperties);
        LeaveProperties = NULL;
    }

    /* Free the last node */
    free(Grid->crp->LeaveProperties);
    Grid->crp->LeaveProperties = NULL;
    Grid = Grid->next;
}

 Grid = initial;
 while (Grid)
 {
    GridHead = Grid;
    free( Grid->crp);
    free(Grid->mng);
    free(Grid->soil);
    free(Grid->ste);
    Grid = Grid->next;
    free(GridHead);
 }
 free(Grid);
 
 free(file);
 
 Grid= initial=NULL;
 
/* At last all used memory have to be freed */ 
//Clean();

    return 0;
}


