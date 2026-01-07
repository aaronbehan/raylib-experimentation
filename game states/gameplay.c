#include "stdio.h"
#include "stdlib.h"

#include "raylib.h"
#include "header.h"

// Global variables
Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };


Camera3D* initCamera(void) 
{
    Camera3D *camera = malloc(sizeof *camera);
    if (!camera) return NULL;
    camera->position = (Vector3){ 0.0f, 10.0f, 10.0f };  // Camera position
    camera->target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera->up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera->fovy = 45.0f;                                // Camera field-of-view Y
    camera->projection = CAMERA_PERSPECTIVE;             // Camera mode type

    return camera;
}


int return_five() 
{
    int variable = 5;
    return variable;
}


void UpdateGameplayScreen(void) 
{
    // Camera
    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };


    cubePosition.x = 3.0f;

    while (!WindowShouldClose())
    {

        if (IsKeyDown(KEY_W)) cubePosition.x -= 0.01f;

        BeginDrawing();

        ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
                DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);

                DrawGrid(10, 1.0f);

            EndMode3D();

            DrawText(TextFormat("Variable ==== %d", 155), 20, 35, 15, BLACK);
        
            DrawFPS(10, 10);

        EndDrawing();
    }
    // once the trigger for changing game state is triggered:     free(camera); along with everything else that must be freed
}
