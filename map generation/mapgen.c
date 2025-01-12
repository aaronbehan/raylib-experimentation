#include "raylib.h"
#include "structs.h"
#include "functions.h"
#include <stdlib.h>

#define TILE_SIZE 1.0f

int main(void)
{
    const int screenWidth = 1400; 
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "raylib map generation");

    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
    float cameraSpeed = 0.5;

    int numberOfWalls = 0;  // check that number of walls is as expected
    Wall *walls = CompileMapData(&numberOfWalls);

    SetTargetFPS(60);

    // Main game loop -------------------------------------------------------------
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Camera controls
        if (IsKeyDown(KEY_W)) camera.position.z -= cameraSpeed, camera.target.z -= cameraSpeed;
        if (IsKeyDown(KEY_A)) camera.position.x -= cameraSpeed, camera.target.x -= cameraSpeed;
        if (IsKeyDown(KEY_S)) camera.position.z += cameraSpeed, camera.target.z += cameraSpeed;
        if (IsKeyDown(KEY_D)) camera.position.x += cameraSpeed, camera.target.x += cameraSpeed;
        camera.position.y -= (GetMouseWheelMove()*cameraSpeed);  // CAMERA UP
        camera.target.y -= (GetMouseWheelMove()*cameraSpeed);  
        camera.position.z -= (GetMouseWheelMove()*0.6);

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawGrid(30, TILE_SIZE); 
                for (int i = 0; i < numberOfWalls; i++)
                {
                    DrawCube(walls[i].position, TILE_SIZE, 0.5f, TILE_SIZE, walls[i].colour);
                }

            EndMode3D();

            DrawFPS(10, 10);

        EndDrawing();
    }

    // De-Initialization
    free(walls);
    CloseWindow();        // Close window and OpenGL context

    return 0;
}