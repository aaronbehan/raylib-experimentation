#include <stddef.h>  // for NULL definition
#include <stdlib.h>
#include "raylib.h"
#include "structs.h"
#include "functions.h"
#include "math.h"

// player can get stuck on the corner of a cube if it collides in the correct location. still unsure of cause

#define TILE_SIZE 1.0f

void resolveCollision(Unit *unit, Vector3 obstacle) // should only be accessed during a collision
{
    // make a child struct? the parent struct covers both unit and wall and has two members: position and size. that way this function can work for both enemy and wall collision

    if (unit->position.z < obstacle.z - 1) // player above
    {
        unit->position.z -= unit->speed;
    }
    if (unit->position.z > obstacle.z + 1) // player below
    {
        unit->position.z += unit->speed;
    }
    if (unit->position.x < obstacle.x - 1) // player on left
    {
        unit->position.x -= unit->speed;
    } 
    if (unit->position.x > obstacle.x + 1) // player on right
    {
        unit->position.x += unit->speed;
    }
}

int main(void)
{
    const int screenWidth = 1400;
    const int screenHeight = 850;

    InitWindow(screenWidth, screenHeight, "quadtree based collision detection");

    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    Unit playerUnit1 = { { -4.0f, 1.0f, -4.0f }, { 0 }, { 1.0f, 2.0f, 1.0f }, { 0 }, 0.1f, 0.5f, 40, 40, 6, false, 0, false, 0, RED};

    Wall wall = {{ 0.0f, 1.0f, 0.0f }, 1, 20, false, GRAY};

    bool collision = false;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Move player 
        if (IsKeyDown(KEY_RIGHT)) playerUnit1.position.x += playerUnit1.speed;
        if (IsKeyDown(KEY_LEFT)) playerUnit1.position.x -= playerUnit1.speed;
        if (IsKeyDown(KEY_DOWN)) playerUnit1.position.z += playerUnit1.speed;
        if (IsKeyDown(KEY_UP)) playerUnit1.position.z -= playerUnit1.speed;

        collision = false;
  
        // Check collisions player vs enemy-box
        if (CheckCollisionBoxes(
            (BoundingBox){(Vector3){ playerUnit1.position.x - playerUnit1.size.x/2,
                                     playerUnit1.position.y - playerUnit1.size.y/2,
                                     playerUnit1.position.z - playerUnit1.size.z/2 },
                          (Vector3){ playerUnit1.position.x + playerUnit1.size.x/2,
                                     playerUnit1.position.y + playerUnit1.size.y/2,
                                     playerUnit1.position.z + playerUnit1.size.z/2 }},
            (BoundingBox){(Vector3){ wall.position.x - 2.0f/2,
                                     wall.position.y - 2.0f/2,
                                     wall.position.z - 2.0f/2,},
                          (Vector3){ wall.position.x + 2.0f/2,
                                     wall.position.y + 2.0f/2,
                                     wall.position.z + 2.0f/2,}})) collision = true;

        if (collision) 
        {
            playerUnit1.colour = RED;
            resolveCollision(&playerUnit1, wall.position);
        }
        else playerUnit1.colour = GREEN;

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // Draw enemy-box
                DrawCube(wall.position, 2.0f, 2.0f, 2.0f, GRAY);
                DrawCubeWires(wall.position, 2.0f/2, 2.0f/2, 2.0f/2, DARKGRAY);

                // Draw player
                DrawCubeV(playerUnit1.position, playerUnit1.size, playerUnit1.colour);

                DrawGrid(10, TILE_SIZE);  // i think... 

            EndMode3D();

            DrawFPS(10, 10);
            DrawText(TextFormat("player pos X = %f  \n\nY= %f  \n\nZ = %f", playerUnit1.position.x, playerUnit1.position.y, playerUnit1.position.z), 30, 30, 30, DARKGRAY);
            DrawText(TextFormat("wall coordinates X = %f  \n\nY= %f  \n\nZ = %f", wall.position.x, wall.position.x, wall.position.x), 30, 300, 30, DARKGRAY);



        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
