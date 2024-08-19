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
float getRandomFloat(float min, float max)
{
    return min + (max - min) * rand() / (float)RAND_MAX;
}

/**
 * @brief Configures an asteroid
 *
 * @param body An orbital body
 * @param centerMass The mass of the most massive object in the star system
 */
void configureAsteroid(OrbitalBody *body, float centerMass)
{
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
OrbitalSim *constructOrbitalSim(float timeStep)
{
    //defino cantidad de planetas
    int n = SOLARSYSTEM_BODYNUM;
    int i;

    //creo un arreglo sistema solar con memoria dinamica
    static OrbitalBody **  SistemaSolar = (OrbitalBody**)malloc(sizeof(OrbitalBody*)*n);

    for(i = 0; i< n; i++){

        SistemaSolar[i] = (OrbitalBody*)malloc(sizeof(OrbitalBody));

    }

    //inicializo cada orbitalBody
    for (i=0;i<n;++i){

        (SistemaSolar[i])->nombre=(solarSystem[i]).name;
        (SistemaSolar[i])->masa=(solarSystem[i]).mass;
        (SistemaSolar[i])->radio=(solarSystem[i]).radius;
        (SistemaSolar[i])->color=(solarSystem[i]).color;
        (SistemaSolar[i])->posicion=(solarSystem[i]).position;
        (SistemaSolar[i])->velocidad=(solarSystem[i]).velocity;

    }
    /* forma franco
    //creo un puntero a la estructura de orbital body.(futuro campo de orbital sim)
    OrbitalBody** p2SistemaSolar = &SistemaSolar;
    //creo e inicializo mi simulacion
    static OrbitalSim sim ={timeStep,(uint8_t)n,p2SistemaSolar};
    */

    static OrbitalSim sim = {timeStep,(uint8_t)n, SistemaSolar};

    //creo e inicializo un puntero a la simulacion y devuelvo el puntero
    static OrbitalSim * p2sim = &sim;
    return(p2sim);

}

/**
 * @brief Destroys an orbital simulation
 */
void destroyOrbitalSim(OrbitalSim *sim)
{
    // libero cada puntero
    for(int i = 0; i<SOLARSYSTEM_BODYNUM; i++){
        free(sim->cuerposCel[i]);
    }
    //la intencion de esta funcion es liberar memoria del heap en la que se almacena el arreglo de orbBodies
    free(sim->cuerposCel);//lo que hago es desreferenciar el campo de sim que apunta al arreglo de orbBodies

}

/**
 * @brief Simulates a timestep
 *
 * @param sim The orbital simulation
 */
void updateOrbitalSim(OrbitalSim *sim)
{
    // llamo a la funcion que modifica las velocidades y las posiciones de los cuerpos celestes
    avanzaTiempo(sim);

}
