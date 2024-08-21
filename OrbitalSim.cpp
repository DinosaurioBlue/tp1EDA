/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

// Enables M_PI #define in Windows

/*@float A lo largo del tp el tipo "float" fue utilizado para representar masas, posiciones y velocidades. 
*Utilizamos este tipo ya que solo ocupan 4 bytes, ademas de presentar un rango de ±1,5 x 10^-45 a ±3,4 x 10^38, y tiene una aproximacion de alrededor de 6 digitos */





//complejidad algoritmica
/*
*suponiendo que la cantidad de planetas son n.
*El calculo de las aceleraciones se hace mediante 2 for anidados. sus contadores recorren desde 0 hasta n-1,
*por lo tanto es O(n^2).
*
*supongamos que la cantidad de asteroides son m. Nosotros programamos las interacciones de cada asteroide respecto a cada planeta, pero no de cdada planeta
*respecto a cada asteroide. Los calculos de actualizacion de los asteroides depende de la cantidad de planetas y asteroides,
*siendo un algoritmo de O(m * n).
*/

#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <math.h>

#include "OrbitalSim.h"
#include "ephemerides.h"
#include "calculos.h"

#define GRAVITATIONAL_CONSTANT 6.6743E-11F
#define ASTEROIDS_MEAN_RADIUS 4E11F

#define CANTIDAD_ASTEROIDES 1000
#define SEGUNDOS_INICIALES 0

#define SISTEMA_SOLAR
//#define ALPHA_CENTAURI

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

    // Surprise! : Easter Egg?
    // phi = 0;
    

    // https://en.wikipedia.org/wiki/Circular_orbit#Velocity
    float v = sqrtf(GRAVITATIONAL_CONSTANT * centerMass / r) * getRandomFloat(0.6F, 1.2F);
    float vy = getRandomFloat(-1E2F, 1E2F);

    // Fill in with your own fields:
    body->masa = 1E12F;  // Typical asteroid weight: 1 billion tons
    body->radio = 2E3F; // Typical asteroid radius: 2km
    body->color = GRAY;
    body->posicion = {r * cosf(phi), 0, r * sinf(phi)};
    body->velocidad = {-v * sinf(phi), vy, v * cosf(phi)};
}

/**
 * @brief Constructs an orbital simulation
 *
 * @param float The time step
 * @return The orbital simulation
 */
OrbitalSim *constructOrbitalSim(float timeStep){

    // Definimos cantidad de cuerpos celestes
    int i;

    #ifdef SISTEMA_SOLAR

    // Creamos arreglo de punteros a OrbitalBody en el Heap
    static OrbitalBody **SistemaSolar = (OrbitalBody**)malloc(sizeof(OrbitalBody*)*(SOLARSYSTEM_BODYNUM));

    for(i = 0; i < SOLARSYSTEM_BODYNUM; i++){
        SistemaSolar[i] = (OrbitalBody*)malloc(sizeof(OrbitalBody));
    }

    // Inicializamos cada OrbitalBody usando los datos de ephemerides
    for (i = 0; i < SOLARSYSTEM_BODYNUM; ++i){
        (SistemaSolar[i])->nombre=(solarSystem[i]).name;
        (SistemaSolar[i])->masa=(solarSystem[i]).mass;
        (SistemaSolar[i])->radio=(solarSystem[i]).radius;
        (SistemaSolar[i])->color=(solarSystem[i]).color;
        (SistemaSolar[i])->posicion=(solarSystem[i]).position;
        (SistemaSolar[i])->velocidad=(solarSystem[i]).velocity;
    }

    static OrbitalSim sim = {timeStep,(uint8_t)(SOLARSYSTEM_BODYNUM), CANTIDAD_ASTEROIDES, SistemaSolar, SEGUNDOS_INICIALES};

    #endif

    #ifdef ALPHA_CENTAURI

    static OrbitalBody **SistemaSolar = (OrbitalBody**)malloc(sizeof(OrbitalBody*)*(ALPHACENTAURISYSTEM_BODYNUM));

    for(i = 0; i < ALPHACENTAURISYSTEM_BODYNUM; i++){
        SistemaSolar[i] = (OrbitalBody*)malloc(sizeof(OrbitalBody));
    }

    for(i = 0; i < ALPHACENTAURISYSTEM_BODYNUM; i++){

        SistemaSolar[i]->nombre = alphaCentauriSystem[i].name;
        SistemaSolar[i]->masa = alphaCentauriSystem[i].mass;
        SistemaSolar[i]->radio = alphaCentauriSystem[i].radius;
        SistemaSolar[i]->color = alphaCentauriSystem[i].color;
        SistemaSolar[i]->posicion = alphaCentauriSystem[i].position;
        SistemaSolar[i]->velocidad = alphaCentauriSystem[i].velocity;

    }

    static OrbitalSim sim = {timeStep,(uint16_t)(ALPHACENTAURISYSTEM_BODYNUM), (uint16_t)CANTIDAD_ASTEROIDES, SistemaSolar, SEGUNDOS_INICIALES};

    #endif

    static OrbitalSim * p2sim = &sim;

    sumarAsteroides(p2sim, CANTIDAD_ASTEROIDES);
    
    return(p2sim);

}

/**
 * @brief Destroys an orbital simulation
 */
void destroyOrbitalSim(OrbitalSim *sim){

    // Libero cada puntero
    for(int i = 0; i < ((sim->cantidadAsteroides) + (sim->cantidadCuerpos)); i++){
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
    avanzaTiempoCuerpos(sim);
    avanzaTiempoAsteroides(sim);

    // Avanzamos el contador de segundos de la simulacion
    sim->timeSince += sim->timeStep;

}

void sumarAsteroides(OrbitalSim *sim, int cantidadAsteroides){
    
    // Reasigno el lugar en el heap para la nueva cantidad de punteros a 
    // orbital bodys que hay.

    sim->cuerposCel = (OrbitalBody**)realloc(sim->cuerposCel, ((sim->cantidadCuerpos)+cantidadAsteroides)*sizeof(OrbitalBody*));

    int i;
    for(i = 0; i<cantidadAsteroides; i++){
        
        sim->cuerposCel[(sim->cantidadCuerpos)+i] = (OrbitalBody*)malloc(sizeof(OrbitalBody));
        configureAsteroid(sim->cuerposCel[(sim->cantidadCuerpos)+i], sim->cuerposCel[0]->masa);

    }

}
