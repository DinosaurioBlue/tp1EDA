/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#ifndef ORBITALSIM_H
#define ORBITALSIM_H

#include <stdint.h>
#include <raylib.h>
#include <raymath.h>


/**
 * @brief Orbital body definition
 */
struct OrbitalBody
{
    
    const char* nombre;
    float masa;
    float radio; 
    Color color;
    Vector3 posicion;
    Vector3 velocidad;

};

/**
 * @brief Orbital simulation definition
 */
struct OrbitalSim
{
    
    float timeStep;
    uint16_t cantidadCuerpos;
    uint16_t cantidadAsteroides;
    OrbitalBody** cuerposCel; 
    float timeSince;                    //segundos desde el 2022

};

OrbitalSim *constructOrbitalSim(float timeStep);
void destroyOrbitalSim(OrbitalSim *sim);

void sumarAsteroides(OrbitalSim *sim, int cantidadAsteroides);

void updateOrbitalSim(OrbitalSim *sim);

#endif
