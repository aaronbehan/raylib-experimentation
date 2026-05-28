#include <stdlib.h>

#include "include/raylib.h"
#include "include/raymath.h"

#include "header.h"

#define TILE_SIZE 8
#define CAMERA_MOVE_MODIFIER 25  // This is a necessary value to track in order to synchronise mouse position with camera position

// Global Variables -----


// ----------------------

int* returnMapData(int rows, int columns) 
{
    int *mapData = calloc((rows * columns), sizeof(int));  

    for (int i = (rows * columns / 2); i < rows * columns; i++)
    {
        mapData[i] = 1;
    }
    
    return mapData;
}


void UpdateGameplayScreen(void)
{
    Camera2D camera = 
    {
    .target = (Vector2){0, 0},
    .zoom = 1.0f
    };

    int rows = 160;
    int columns = 240;

    int* map = returnMapData(rows, columns);

    Vector2 mousePosition = { 0 };
    Vector2 cameraMouseDifference = { 0, 0 };
    float wheel = 0;

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        mousePosition.x = GetMousePosition().x + cameraMouseDifference.x;
        mousePosition.y = GetMousePosition().y + cameraMouseDifference.y;

        wheel = GetMouseWheelMove();

        if (IsKeyPressed(KEY_W)) {
            camera.target.y = camera.target.y - CAMERA_MOVE_MODIFIER; 
            cameraMouseDifference.y = cameraMouseDifference.y - CAMERA_MOVE_MODIFIER;
        };
        if (IsKeyPressed(KEY_A)) {
            camera.target.x = camera.target.x - CAMERA_MOVE_MODIFIER; 
            cameraMouseDifference.x = cameraMouseDifference.x - CAMERA_MOVE_MODIFIER;

        };
        if (IsKeyPressed(KEY_S)) {
            camera.target.y = camera.target.y + CAMERA_MOVE_MODIFIER;
            cameraMouseDifference.y = cameraMouseDifference.y + CAMERA_MOVE_MODIFIER;

        };
        if (IsKeyPressed(KEY_D)) {
            camera.target.x = camera.target.x + CAMERA_MOVE_MODIFIER;
            cameraMouseDifference.x = cameraMouseDifference.x + CAMERA_MOVE_MODIFIER;

        };
        if (IsKeyPressed(KEY_SPACE)) {
            camera.target = (Vector2){ 0 }; 
            cameraMouseDifference = (Vector2){ 0 };

        };

        if (wheel != 0)
        {
            // Get the world point that is under the mouse
            //Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

            // Set the offset to where the mouse is
            //camera.offset = GetMousePosition();

            // Set the target to match, so that the camera maps the world space point
            // under the cursor to the screen space point under the cursor at any zoom
            //camera.target = mouseWorldPos;

            // Zoom increment
            // Uses log scaling to provide consistent zoom speed
            float scale = 0.5f*wheel;
            camera.zoom = Clamp(expf(logf(camera.zoom)+scale), 0.125f, 64.0f);
        }

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode2D(camera);

            DrawText("hello", 20, 20, 20, BLACK);


            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < columns; j++)
                {
                    if (map[i * columns + j] == 1) 
                    {
                        {DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE / 2, TILE_SIZE / 2, MAROON);};
                        if (CheckCollisionPointRec(mousePosition, (Rectangle){j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE}))
                        {
                            DrawRectangleLines(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, GREEN);
                            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
                            {
                                map[i * columns + j] = 0;
                            };
                        }
                    }
                }
            }
            EndMode2D();
            
            DrawFPS(10, 10);
            DrawText(TextFormat("mouseposition= x%fy%f", mousePosition.x, mousePosition.y), 30, 140, 30, LIGHTGRAY);
            DrawText(TextFormat("cameramousedifference= x%fy%f", cameraMouseDifference.x, cameraMouseDifference.y), 30, 170, 30, LIGHTGRAY);

        EndDrawing();
    }

    // Free memory
    free(map);
}
