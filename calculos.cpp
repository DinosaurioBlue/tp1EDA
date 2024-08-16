#include "calculos.h"

void avanzaTiempo(OrbitalSim* sim, int64_t tiempo) {

    int i, j;

    for (i = 0; i < sim->cantidadCuerpos; i++) {

        for (j = 0; j < sim->cantidadCuerpos; j++) {

            if (i == j)
                continue;

            Vector3 aceleracionNoCentral = Vector3Scale(Vector3Subtract(((sim->cuerposCel) + i)->posicion, ((sim->cuerposCel) + j)->posicion), G * ((sim->cuerposCel) + j)->masa /
                (Vector3Length(((sim->cuerposCel) + j)->posicion - ((sim->cuerposCel) + i)->posicion)));//aceleracion del cuerpo ""no central""


            Vector3 aceleracionCentral = Vector3Scale(Vector3Subtract(((sim->cuerposCel) + j)->posicion, ((sim->cuerposCel) + i)->posicion), G * ((sim->cuerposCel) + i)->masa /
                    (Vector3Length(((sim->cuerposCel) + j)->posicion - ((sim->cuerposCel) + i)->posicion)));//ACELERACION DEL CUERPO ""CENTRAL""


            ((sim->cuerposCel) + j)->velocidad = Vector3Add((sim->cuerposCel) + j)->velocidad, Vector3Scale(aceleracionNoCentral, sim->timeStep);

            ((sim->cuerposCel) + i)->velocidad = Vector3Add((sim->cuerposCel) + i)->velocidad, Vector3Scale(aceleracionCentral, sim->timeStep);


        }

    }
    return;

}