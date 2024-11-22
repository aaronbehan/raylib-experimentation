#include "raylib.h"
// I could feed in the player coordinates as arg 1, then have a super sized bounding box/sphere to detect other bounding boxes which could 
// prioritise which bounding boxes I need to check collisions for

int main(void)
{
    const int screenWidth = 1400;
    const int screenHeight = 850;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - box collisions");
    // InitAudioDevice();
    // Sound sound = LoadSound("stringpluck.wav");

    // Define the camera to look into our 3d world 
    // { {X,Y,Z}, {}, {}, Distance away?, ? }
    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    Vector3 playerPos = { 0.0f, 1.0f, -4.0f }; // X = West and East. Y = Up and Down. Z = North and South. what if we just turned this into the bounding box :|
    Vector3 playerSize = { 1.0f, 2.0f, 1.0f };
    float playerSpeed = 0.2f;

    Color playerColor = GREEN;

    Vector3 enemyBoxPos = { -4.0f, 1.0f, 0.0f };
    Vector3 enemyBoxSize = { 2.0f, 2.0f, 2.0f };

    Vector3 oldPlayerPos = { 0, 0, 0 };
    bool collision = false;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        oldPlayerPos = playerPos;

        // Move player 
        if (IsKeyDown(KEY_RIGHT)) playerPos.x += playerSpeed;
        if (IsKeyDown(KEY_LEFT)) playerPos.x -= playerSpeed;
        if (IsKeyDown(KEY_DOWN)) playerPos.z += playerSpeed;
        if (IsKeyDown(KEY_UP)) playerPos.z -= playerSpeed;
        if (IsKeyDown(KEY_COMMA)) playerPos.y += playerSpeed;
        if (IsKeyDown(KEY_PERIOD)) playerPos.y -= playerSpeed;
        // else if (IsKeyPressed(KEY_SPACE)) PlaySound(sound);

        collision = false;
  
        // Check collisions player vs enemy-box
        if (CheckCollisionBoxes(
            (BoundingBox){(Vector3){ playerPos.x - playerSize.x/2,
                                     playerPos.y - playerSize.y/2,
                                     playerPos.z - playerSize.z/2 },
                          (Vector3){ playerPos.x + playerSize.x/2,
                                     playerPos.y + playerSize.y/2,
                                     playerPos.z + playerSize.z/2 }},
            (BoundingBox){(Vector3){ enemyBoxPos.x - enemyBoxSize.x/2,
                                     enemyBoxPos.y - enemyBoxSize.y/2,
                                     enemyBoxPos.z - enemyBoxSize.z/2 },
                          (Vector3){ enemyBoxPos.x + enemyBoxSize.x/2,
                                     enemyBoxPos.y + enemyBoxSize.y/2,
                                     enemyBoxPos.z + enemyBoxSize.z/2 }})) collision = true;

        if (collision) 
        {
            playerColor = RED;
            playerPos = oldPlayerPos;
            // PlaySound(sound);
            if (IsKeyDown(KEY_P)) // HOLD P TO PUSH OBJECT
            {
                if (IsKeyDown(KEY_RIGHT)) playerPos.x += playerSpeed/2, enemyBoxPos.x += playerSpeed/2;
                else if (IsKeyDown(KEY_LEFT)) playerPos.x -= playerSpeed/2, enemyBoxPos.x -= playerSpeed/2;
                else if (IsKeyDown(KEY_DOWN)) playerPos.z += playerSpeed/2, enemyBoxPos.z += playerSpeed/2;
                else if (IsKeyDown(KEY_UP)) playerPos.z -= playerSpeed/2, enemyBoxPos.z -= playerSpeed/2;
                else if (IsKeyDown(KEY_COMMA)) playerPos.y += playerSpeed/2, enemyBoxPos.y += playerSpeed/2;
                else if (IsKeyDown(KEY_PERIOD)) playerPos.y -= playerSpeed/2, enemyBoxPos.y -= playerSpeed/2;
            }
        }
        else playerColor = GREEN;

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // Draw enemy-box
                DrawCube(enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, GRAY);
                DrawCubeWires(enemyBoxPos, enemyBoxSize.x, enemyBoxSize.y, enemyBoxSize.z, DARKGRAY);

                // Draw player
                DrawCubeV(playerPos, playerSize, playerColor);

                DrawGrid(10, 1.0f);        // Draw a grid

            EndMode3D();

            DrawText("Hold P + directional key to push", 220, 40, 20, GRAY);

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context
    // CloseAudioDevice();

    return 0;
}
