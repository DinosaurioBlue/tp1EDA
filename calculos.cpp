#include "calculos.h"
#include <stdlib.h>

//calculos de aceleracion de los cuerpos celestes
void avanzaTiempoCuerpos(OrbitalSim* sim){
    //definicion de variables
    int i, j;
    long double norma;
    Vector3 vUnitario;
    
    for(i = 0; i < sim->cantidadCuerpos; i++){    

        Vector3 ai = {0,0,0};
        for(j = 0; j < sim->cantidadCuerpos; j++){  

            if(i == j){
                continue;
            }
            //calculo de la norma del vector posicion 
            norma = (long double)(Vector3Length(Vector3Subtract(sim->cuerposCel[i]->posicion, sim->cuerposCel[j]->posicion)));
            
            if(norma!=0){
                //calculo del versor direccion de la aceleracion
                vUnitario = Vector3Scale(Vector3Subtract(sim->cuerposCel[i]->posicion, sim->cuerposCel[j]->posicion), 1/norma);
            }else{
                exit(1);
            }
            //calculo del vector aceleracion
            ai = Vector3Add(ai, Vector3Scale(vUnitario, (-1) * G * sim->cuerposCel[j]->masa * (1/(norma*norma))));

        }
        //calculo del vector velocidad resultante
        sim->cuerposCel[i]->velocidad = Vector3Add(sim->cuerposCel[i]->velocidad, Vector3Scale(ai, sim->timeStep)); 

    }
    
    for(i=0;i<sim->cantidadCuerpos;i++){
        //calculo de la posicion resultante
        sim->cuerposCel[i]->posicion = Vector3Add(sim->cuerposCel[i]->posicion, Vector3Scale(sim->cuerposCel[i]->velocidad, sim->timeStep));
    }

    return;
}


void avanzaTiempoAsteroides(OrbitalSim* sim){

    int i, j;
    long double norma;
    Vector3 vUnitario;

    for(i = 0; i < sim->cantidadAsteroides; i++){    

        Vector3 ai = {0,0,0};
        for(j = 0; j < (sim->cantidadCuerpos + sim->cantidadAsteroides); j++){  

            if ((i + sim->cantidadCuerpos)==j){
                continue;
            }

            //calculo de la norma del vector posicion 
            norma = (long double)(Vector3Length(Vector3Subtract(sim->cuerposCel[i+(sim->cantidadCuerpos)]->posicion, sim->cuerposCel[j]->posicion)));
            
            if(norma!=0){
                //calculo del versor direccion de la aceleracion
                vUnitario = Vector3Scale(Vector3Subtract(sim->cuerposCel[i+(sim->cantidadCuerpos)]->posicion, sim->cuerposCel[j]->posicion), 1/norma);
            }else{
                exit(1);
            }
            //calculo del vector aceleracion
            ai = Vector3Add(ai, Vector3Scale(vUnitario, (-1) * G * sim->cuerposCel[j]->masa * (1/(norma*norma))));

        }
        //calculo del vector velocidad resultante
        sim->cuerposCel[i+(sim->cantidadCuerpos)]->velocidad = Vector3Add(sim->cuerposCel[i+(sim->cantidadCuerpos)]->velocidad, Vector3Scale(ai, sim->timeStep)); 

    }
    
    for(i=0;i<sim->cantidadAsteroides;i++){
        //calculo de la posicion resultante
        sim->cuerposCel[i+(sim->cantidadCuerpos)]->posicion = Vector3Add(sim->cuerposCel[i+(sim->cantidadCuerpos)]->posicion, Vector3Scale(sim->cuerposCel[i+(sim->cantidadCuerpos)]->velocidad, sim->timeStep));
    }

    return;
}