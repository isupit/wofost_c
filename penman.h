
#ifndef PENMAN_H
#define PENMAN_H

typedef struct ETP {
    float E0;
    float ES0;
    float ET0;
    float ETC;
} Etp;

Etp Penman;

extern float min(float a, float b);
extern float max(float a, float b);
extern float limit(float a, float b, float c);

typedef struct EVP {
    float MaxEvapWater;
    float MaxEvapSoil;
    float MaxTranspiration;
} EVP;

EVP Evtra;

#define PSYCON  0.67    /* psychrometric instrument constant (mbar/Celsius-1)*/
#define REFCFW  0.05    /* albedo for a water surface                        */
#define REFCFS  0.15    /* albedo for a soil surface                         */
#define REFCFC  0.25    /* albedo for a  canopy                              */
#define LHVAP   2.45e6  /* latent heat of evaporation of water (J/kg=J/mm)   */
#define STBC    4.9e3   /* Stefan Boltzmann constant (J/m2/d/K4) */

#endif	// PENMAN_H

