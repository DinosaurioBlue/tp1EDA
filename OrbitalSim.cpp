/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

// Enables M_PI #define in Windows
#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <math.h>

#include "OrbitalSim.h"
#include "ephemerides.h"
#include "calculos.h"

#define GRAVITATIONAL_CONSTANT 6.6743E-11F
#define ASTEROIDS_MEAN_RADIUS 4E11F

/**
 * @brief Gets a uniform random value in a range
 *
 * @param min Minimum value
 * @param max Maximum value
 * @return The random value
 */
float getRandomFloat(float min, float max){
    return min + (max - min) * rand() / (float)RAND_MAX;
}

/**
 * @brief Configures an asteroid
 *
 * @param body An orbital body
 * @param centerMass The mass of the most massive object in the star system
 */
void configureAsteroid(OrbitalBody *body, float centerMass){
    // Logit distribution
    float x = getRandomFloat(0, 1);
    float l = logf(x) - logf(1 - x) + 1;

    // https://mathworld.wolfram.com/DiskPointPicking.html
    float r = ASTEROIDS_MEAN_RADIUS * sqrtf(fabsf(l));
    float phi = getRandomFloat(0, 2.0F * (float)M_PI);

    // Surprise!
    // phi = 0;

    // https://en.wikipedia.org/wiki/Circular_orbit#Velocity
    float v = sqrtf(GRAVITATIONAL_CONSTANT * centerMass / r) * getRandomFloat(0.6F, 1.2F);
    float vy = getRandomFloat(-1E2F, 1E2F);

    // Fill in with your own fields:
    // body->mass = 1E12F;  // Typical asteroid weight: 1 billion tons
    // body->radius = 2E3F; // Typical asteroid radius: 2km
    // body->color = GRAY;
    // body->position = {r * cosf(phi), 0, r * sinf(phi)};
    // body->velocity = {-v * sinf(phi), vy, v * cosf(phi)};
}

/**
 * @brief Constructs an orbital simulation
 *
 * @param float The time step
 * @return The orbital simulation
 */
OrbitalSim *constructOrbitalSim(float timeStep){

    // Definimos cantidad de cuerpos celestes
    int n = SOLARSYSTEM_BODYNUM;
    int i;

    // Creamos arreglo de punteros a OrbitalBody en el Heap
    static OrbitalBody **  SistemaSolar = (OrbitalBody**)malloc(sizeof(OrbitalBody*)*n);

    for(i = 0; i< n; i++){
        SistemaSolar[i] = (OrbitalBody*)malloc(sizeof(OrbitalBody));
    }

    // Inicializamos cada OrbitalBody usando los datos de ephemerides
    for (i=0;i<n;++i){
        (SistemaSolar[i])->nombre=(solarSystem[i]).name;
        (SistemaSolar[i])->masa=(solarSystem[i]).mass;
        (SistemaSolar[i])->radio=(solarSystem[i]).radius;
        (SistemaSolar[i])->color=(solarSystem[i]).color;
        (SistemaSolar[i])->posicion=(solarSystem[i]).position;
        (SistemaSolar[i])->velocidad=(solarSystem[i]).velocity;
    }

    static OrbitalSim sim = {timeStep,(uint8_t)n, SistemaSolar, 0};
    static OrbitalSim * p2sim = &sim;
    
    return(p2sim);

}

/**
 * @brief Destroys an orbital simulation
 */
void destroyOrbitalSim(OrbitalSim *sim){

    // Libero cada puntero
    for(int i = 0; i<SOLARSYSTEM_BODYNUM; i++){
        free(sim->cuerposCel[i]);
    }

    // Libero el puntero al arreglo de punteros
    free(sim->cuerposCel);

}

/**
 * @brief Simulates a timestep
 *
 * @param sim The orbital simulation
 */
void updateOrbitalSim(OrbitalSim *sim){
    
    // Llamamos a la funcion que modifica las velocidades y las posiciones de los cuerpos celestes
    avanzaTiempo(sim);

    // Avanzamos el contador de segundos de la simulacion
    sim->timeSince += sim->timeStep;

}
