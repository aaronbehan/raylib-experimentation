#include "raylib.h"

int main(void)
{
    const int screenWidth = 1400; 
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "raylib map generation");

    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
    float cameraSpeed = 0.5;

    // Wall greyWall = {1, GRAY};
    // Wall pinkWall = {2, PINK};
    // Wall brownWall = {3, BLUE};

    #define TILE_SIZE 1.0f
    #define NUMBER_OF_TILES 16
    #define ROWS 4  // IMPORTANT TO UPDATE THESE IF THE MAP DIMENSIONS CHANGE 
    #define COLUMNS 4

    int layout[ROWS][COLUMNS] = {
        { 1, 1, 0, 1 },
        { 0, 1, 0, 0 },
        { 0, 1, 1, 1 },
        { 1, 0, 0, 1 }
    };

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Main game loop
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
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // --------------------------------------------
                DrawGrid(20, TILE_SIZE); 
                // --------------------------------------------

                float axisZ = -1.5f;  // this is not a universally correct variable
                float axisX = -1.5f;  // DIVIDE ROW/COLUMN TOTAL BY 2 AND SUBTRACT 0.5 TO GET THIS VALUE. 

                // Draw boxes. in order to please the GPU, when we have different textures demanded by different cube types, we could print all of one type first, then proceed onto next type, und so weiter
                for (int i = 0; i < ROWS; i++) {
                    for (int j = 0; j < COLUMNS; j++) {
                        
                        if (layout[i][j] == 1) DrawCube((Vector3){axisX, 0.5f, axisZ}, TILE_SIZE, 0.5f, TILE_SIZE, BLUE);
                        axisX++;
                    }
                    axisX = -1.5;
                    axisZ++;
                }
                
            EndMode3D();

            DrawText("Hold P + directional key to push", 220, 40, 20, GRAY);

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context

    return 0;
}
