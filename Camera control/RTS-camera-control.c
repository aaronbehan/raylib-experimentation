#include "raylib.h"

int main(void)
{
    int screenWidth = 1400;
    int screenHeight = 850;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(screenWidth, screenHeight, "raylib example");

    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
    float cameraSpeed = 0.2;
    float screenControlBorder = 100;

    Vector2 mousePos;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        screenWidth = GetScreenWidth();  // ALLOWS THE BELOW ELEMENTS TO ADJUST DIMENSIONS/POSITION AFTER SCREEN RESIZING
        screenHeight = GetScreenHeight();

        mousePos = GetMousePosition();

        // Camera controls
        if (mousePos.y <= screenControlBorder) camera.position.z -= cameraSpeed, camera.target.z -= cameraSpeed;  // Up
        if (mousePos.x <= screenControlBorder) camera.position.x -= cameraSpeed, camera.target.x -= cameraSpeed;  // Left 
        if (mousePos.y >= (screenHeight - screenControlBorder)) camera.position.z += cameraSpeed, camera.target.z += cameraSpeed;  // Down  will probably need to adjust screenControlBorder for this one so that player can click buttons without screen scroll
        if (mousePos.x >= (screenWidth - screenControlBorder)) camera.position.x += cameraSpeed, camera.target.x += cameraSpeed;  // Right
        camera.position.y -= (GetMouseWheelMove()*cameraSpeed);  // CAMERA UP
        camera.target.y -= (GetMouseWheelMove()*cameraSpeed);  
        camera.position.z -= (GetMouseWheelMove()*0.6);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawGrid(10, 1.0f);        // Draw a grid

            EndMode3D();

            DrawFPS(10, 10);
            // DrawText(TextFormat("Mouse pos X = %f\nMouse pos Y = %f", mousePos.x, mousePos.y), 20, 20, 20, BLACK);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context

    return 0;
}
