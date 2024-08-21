#include "calculos.h"
#include <stdlib.h>
void avanzaTiempoCuerpos(OrbitalSim* sim){

    int i, j;
    long double norma;
    Vector3 vUnitario;

    for(i = 0; i < sim->cantidadCuerpos; i++){    

        Vector3 ai = {0,0,0};
        for(j = 0; j < sim->cantidadCuerpos; j++){  

            if(i == j){
                continue;
            }
            
            norma = (long double)(Vector3Length(Vector3Subtract(sim->cuerposCel[i]->posicion, sim->cuerposCel[j]->posicion)));
            
            if(norma!=0){
                vUnitario = Vector3Scale(Vector3Subtract(sim->cuerposCel[i]->posicion, sim->cuerposCel[j]->posicion), 1/norma);
            }else{
                exit(1);
            }

            ai = Vector3Add(ai, Vector3Scale(vUnitario, (-1) * G * sim->cuerposCel[j]->masa * (1/(norma*norma))));

        }

        sim->cuerposCel[i]->velocidad = Vector3Add(sim->cuerposCel[i]->velocidad, Vector3Scale(ai, sim->timeStep)); 

    }
    
    for(i=0;i<sim->cantidadCuerpos;i++){
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
        for(j = 0; j < sim->cantidadCuerpos; j++){  
            
            norma = (long double)(Vector3Length(Vector3Subtract(sim->cuerposCel[i+(sim->cantidadCuerpos)]->posicion, sim->cuerposCel[j]->posicion)));
            
            if(norma!=0){
                vUnitario = Vector3Scale(Vector3Subtract(sim->cuerposCel[i+(sim->cantidadCuerpos)]->posicion, sim->cuerposCel[j]->posicion), 1/norma);
            }else{
                exit(1);
            }

            ai = Vector3Add(ai, Vector3Scale(vUnitario, (-1) * G * sim->cuerposCel[j]->masa * (1/(norma*norma))));

        }

        sim->cuerposCel[i+(sim->cantidadCuerpos)]->velocidad = Vector3Add(sim->cuerposCel[i+(sim->cantidadCuerpos)]->velocidad, Vector3Scale(ai, sim->timeStep)); 

    }
    
    for(i=0;i<sim->cantidadAsteroides;i++){
        sim->cuerposCel[i+(sim->cantidadCuerpos)]->posicion = Vector3Add(sim->cuerposCel[i+(sim->cantidadCuerpos)]->posicion, Vector3Scale(sim->cuerposCel[i+(sim->cantidadCuerpos)]->velocidad, sim->timeStep));
    }

    return;
}