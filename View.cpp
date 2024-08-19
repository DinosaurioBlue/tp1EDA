/**
 * @brief Implements an orbital simulation view
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

#include <time.h>

#include "View.h"
#include "raymath.h"
#include "raylib.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define SATURNO 6

/**
 * @brief Converts a timestamp (number of seconds since 1/1/2022)
 *        to an ISO date ("YYYY-MM-DD")
 *
 * @param timestamp the timestamp
 * @return The ISO date (a raylib string)
 */
const char *getISODate(float timestamp){

    // Timestamp epoch: 1/1/2022
    struct tm unichEpochTM = {0, 0, 0, 1, 0, 122};

    // Convert timestamp to UNIX timestamp (number of seconds since 1/1/1970)
    time_t unixEpoch = mktime(&unichEpochTM);
    time_t unixTimestamp = unixEpoch + (time_t)timestamp;

    // Returns ISO date
    struct tm *localTM = localtime(&unixTimestamp);
    return TextFormat("%04d-%02d-%02d",
                      1900 + localTM->tm_year, localTM->tm_mon + 1, localTM->tm_mday);
}

/**
 * @brief Constructs an orbital simulation view
 *
 * @param fps Frames per second for the view
 * @return The view
 */
View *constructView(int fps){

    View *view = new View();

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "EDA Orbital Simulation");
    SetTargetFPS(fps);
    DisableCursor();

    view->camera.position = {10.0f, 10.0f, 10.0f};
    view->camera.target = {0.0f, 0.0f, 0.0f};
    view->camera.up = {0.0f, 1.0f, 0.0f};
    view->camera.fovy = 45.0f;
    view->camera.projection = CAMERA_PERSPECTIVE;

    return view;
}

/**
 * @brief Destroys an orbital simulation view
 *
 * @param view The view
 */
void destroyView(View *view){
    
    CloseWindow();

    delete view;
}

/**
 * @brief Should the view still render?
 *
 * @return Should rendering continue?
 */
bool isViewRendering(View *view){
    return !WindowShouldClose();
}

/**
 * Renders an orbital simulation
 *
 * @param view The view
 * @param sim The orbital sim
 */
void renderView(View *view, OrbitalSim *sim){

    UpdateCamera(&view->camera, CAMERA_FREE);

    BeginDrawing();

    ClearBackground(BLACK);
    BeginMode3D(view->camera);

    {
        int i;
        for(i=0; i<sim->cantidadCuerpos; i++){
            // NO ANDA BIEN AUN LA DE RINGS
            if(i == SATURNO){
                DrawSphereEx(Vector3Scale(sim->cuerposCel[i]->posicion, 1E-11), 0.005*logf(sim->cuerposCel[i]->radio), 1, 1,sim->cuerposCel[i]->color);
            }else{
                DrawSphere(Vector3Scale(sim->cuerposCel[i]->posicion, 1E-11), 0.005*logf(sim->cuerposCel[i]->radio), sim->cuerposCel[i]->color);
            }

        }

        for(i=0; i<sim->cantidadAsteroides; i++){
            
            DrawSphere(Vector3Scale(sim->cuerposCel[(sim->cantidadCuerpos)+i]->posicion, 1E-11), 0.005*logf(sim->cuerposCel[(sim->cantidadCuerpos)+i]->radio), sim->cuerposCel[(sim->cantidadCuerpos)+i]->color);

        }

        
    }

    DrawGrid(10, 10.0f);
    EndMode3D();
    
    {
        DrawText(getISODate(sim->timeSince), 10, 10, 30, WHITE);
    }
    
    EndDrawing();
}
