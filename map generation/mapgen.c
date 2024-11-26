#include "raylib.h"

typedef struct Wall { 
    int id; 
    Color color;
} Wall;

int main(void)
{
    const int screenWidth = 1400; 
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "raylib map generation");

    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
    float cameraSpeed = 0.5;

    // Wall attributes
    Wall greyWall = {1, GRAY};
    Wall pinkWall = {2, PINK};

    #define TILE_SIZE 1.0f
    #define NUMBER_OF_TILES 432
    #define ROWS 18  // IMPORTANT TO UPDATE THESE IF THE MAP DIMENSIONS CHANGE 
    #define COLUMNS 24

    // WALLS = 1, EMPTY SPACE = 0
    int layout[ROWS][COLUMNS] = {
        //0, 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23
        { 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2 },  // 0
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },  // 1
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },  // 2
        { 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 3
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 4
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 5
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 6
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 7
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 8
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 9
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 10
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 11
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 12
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 13
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 14
        { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 16
        { 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2 }   // 17
    };

    int sumOfWalls = 0;

    // Determining how many walls have been generated
    for (int row = 0; row < ROWS; row++) {
        for (int column = 0; column < COLUMNS; column++) {
            if (layout[row][column] > 0) sumOfWalls ++;  // Walls will have values higher than 1 and this accounts for that fact
        }
    }

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

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
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // --------------------------------------------
                DrawGrid(20, TILE_SIZE); 
                // --------------------------------------------

                // finds the correct coordinate based on array size 
                float axisZ = ((ROWS * -1) / 2) + 0.5;  
                float axisX = ((COLUMNS * -1) / 2) + 0.5;

                // Draw boxes. in order to please the GPU, when we have different textures demanded by different cube types, we could print all of one type first, then proceed onto next type, und so weiter
                for (int i = 0; i < ROWS; i++) {
                    for (int j = 0; j < COLUMNS; j++) {
                        
                        if (layout[i][j] == greyWall.id) DrawCube((Vector3){axisX, 0.5f, axisZ}, TILE_SIZE, 0.5f, TILE_SIZE, greyWall.color);
                        else if (layout[i][j] == pinkWall.id) DrawCube((Vector3){axisX, 0.5f, axisZ}, TILE_SIZE, 0.5f, TILE_SIZE, pinkWall.color);
                        axisX++;   
                    }
                    axisX = ((COLUMNS * -1) / 2) + 0.5;
                    axisZ++;
                }
                
            EndMode3D();

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context

    return 0;
}
