#include "calculos.h"

void avanzaTiempo(OrbitalSim* sim) {

    int i, j;
    long double norma;
    Vector3 vUnitario;

    for(i = 0; i < sim->cantidadCuerpos; i++){    

        Vector3 ai = {0,0,0};
        for(j = 0; j < sim->cantidadCuerpos; j++){  

            if (i == j){
                continue;
            }
            
            norma = (long double)(Vector3Length(Vector3Subtract(sim->cuerposCel[i]->posicion, sim->cuerposCel[j]->posicion)));
            vUnitario = Vector3Scale(Vector3Subtract(sim->cuerposCel[i]->posicion, sim->cuerposCel[j]->posicion), 1/norma);

            ai = Vector3Add(ai, Vector3Scale(vUnitario, (-1) * G * sim->cuerposCel[j]->masa * (1/(norma*norma))));

            /*  LICHA                                                    
            Vector3 aceleracionNoCentral = Vector3Scale(Vector3Subtract(sim->cuerposCel[i]->posicion, (sim->cuerposCel[j]->posicion), G * sim->cuerposCel[j]->masa /
                (Vector3Length(Vector3Subtract(((sim->cuerposCel) + j)->posicion, ((sim->cuerposCel) + i)->posicion))));//aceleracion del cuerpo ""no central""


            Vector3 aceleracionCentral = Vector3Scale(Vector3Subtract(((sim->cuerposCel) + j)->posicion, ((sim->cuerposCel) + i)->posicion), G * ((sim->cuerposCel) + i)->masa /
                (Vector3Length(((sim->cuerposCel) + j)->posicion - ((sim->cuerposCel) + i)->posicion)));//ACELERACION DEL CUERPO ""CENTRAL""
            */

        }
    
        sim->cuerposCel[i]->velocidad = Vector3Add(sim->cuerposCel[i]->velocidad, Vector3Scale(ai, sim->timeStep)); 
    }

    
    for(i=0;i<sim->cantidadCuerpos;i++)
    {
        sim->cuerposCel[i]->posicion = Vector3Add(sim->cuerposCel[i]->posicion, Vector3Scale(sim->cuerposCel[i]->velocidad, sim->timeStep));
    }


    return;

}