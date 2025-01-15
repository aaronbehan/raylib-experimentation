#include <stdlib.h>
#include <stddef.h>  // for NULL definition
#include <math.h>
#include "raylib.h"
#include "structs.h"
#include "functions.h"

#define TILE_SIZE 1.0f
#define CENTRE_OF_MAP 0.0f

BoundingBox defineCollisionEdges(Unit unit) // unused but not writing this off just yet
{
    BoundingBox bb = { 0 };

    bb.min = (Vector3){unit.position.x - unit.size.x / 2, unit.position.y - unit.size.y / 2, unit.position.z - unit.size.z / 2};
    bb.max = (Vector3){unit.position.x + unit.size.x / 2, unit.position.y + unit.size.y / 2, unit.position.z + unit.size.z / 2};

    return bb;
}

void resolveCollision(Unit *unit, BoundingBox obstacle)
{
    // Calculate the half-size of the unit (assuming centered bounding box)
    float unitHalfSizeX = unit->size.x / 2.0f;
    float unitHalfSizeZ = unit->size.z / 2.0f;

    // Calculate overlap depths for each axis
    float overlapX = fminf(obstacle.max.x - (unit->position.x - unitHalfSizeX), 
                           (unit->position.x + unitHalfSizeX) - obstacle.min.x);
    float overlapZ = fminf(obstacle.max.z - (unit->position.z - unitHalfSizeZ), 
                           (unit->position.z + unitHalfSizeZ) - obstacle.min.z);

    // Resolve based on smallest overlap
    if (overlapX < overlapZ)
    {
        // Resolve X axis
        if (unit->position.x < (obstacle.min.x + obstacle.max.x) / 2.0f) // Unit is on the left
        {
            unit->position.x = obstacle.min.x - unitHalfSizeX * 1.101f;  // this additional calculation seems to reduce wall catching
        }
        else // Unit is on the right
        {
            unit->position.x = obstacle.max.x + unitHalfSizeX * 1.101f;
        }
    }
    else
    {
        // Resolve Z axis
        if (unit->position.z < (obstacle.min.z + obstacle.max.z) / 2.0f) // Unit is above
        {
            unit->position.z = obstacle.min.z - unitHalfSizeZ * 1.101f;
        }
        else // Unit is below
        {
            unit->position.z = obstacle.max.z + unitHalfSizeZ * 1.101f;
        }
    }
}

int main() 
{
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "raylib quadtree based collision");

    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
    float cameraSpeed = 0.5;

    Unit player = { { -4.0f, 1.0f, -4.0f }, { 0 }, { 1.0f, 2.0f, 1.0f }, { 0 }, 0.1f, 0.5f, 40, 40, 6, false, 0, false, 0, GREEN};

    Vector3 quadtreePos = { 0.0f, 1.0f, 0.0f };
    float quadtreeSize = 30.0f;

    int numberOfWalls = 0;  // check that number of walls is as expected
    Wall *walls = CompileMapData(&numberOfWalls);

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
        // Player controls
        if (IsKeyDown(KEY_RIGHT)) player.position.x += player.speed;
        if (IsKeyDown(KEY_LEFT)) player.position.x -= player.speed;
        if (IsKeyDown(KEY_DOWN)) player.position.z += player.speed;
        if (IsKeyDown(KEY_UP)) player.position.z -= player.speed;

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                //                                 X              Z              halfwdth
                Boundary boundary = createBoundary(CENTRE_OF_MAP, CENTRE_OF_MAP, quadtreeSize / 2); // Create a boundary of 30x30
                QuadtreeNode* root = createQuadtreeNode(boundary, 4); // Root node with capacity of 4

                // INSERT WALLS INTO QUADTREE. I understand now that i don't just have to put vectors into the wall. i have to insert entire instances of structs. otherwise how do i link a wall to its health member or texture after you pull it back out using range? how do i link an enemy to its attack/speed?
                for (int i = 0; i < numberOfWalls; i++) {  // after inserting walls, put the enemies in. when checking collisions, only check collisions in quadtrees
                    if (insert(root, walls[i].position)) // we put in a pointer to a QuadtreeNode and a Point(on the graph)
                    {
                        DrawCube(walls[i].position, 1.0f, 1.0f, 1.0f, walls[i].colour);
                    }
                }

                // QUERY points within a region ----------------------------------------------------------------------------------------------------
                Boundary range = createBoundary(player.position.x, player.position.z, 2); // Query range (centered at 0,0 with halfWidth of 2)
                Vector3 foundPoints[30];  // if this number is breached, segmentation fault occurs in queryRange
                int foundCount = 0;

                // feed in Quadtree root node and a Boundary range. then feed empty array of points. then pointer to an int. 
                queryRange(root, range, foundPoints, &foundCount);

                for (int i = 0; i < foundCount; i++) 
                {
                    BoundingBox wallLimits = (BoundingBox){(Vector3){foundPoints[i].x - 0.5, foundPoints[i].y - 0.5, foundPoints[i].z - 0.5},
                                                            (Vector3){foundPoints[i].x + 0.5, foundPoints[i].y + 0.5, foundPoints[i].z + 0.5}};

                    if (CheckCollisionBoxes(
                        (BoundingBox){(Vector3){player.position.x - player.size.x/2, player.position.y - player.size.y/2, player.position.z - player.size.z/2},
                                      (Vector3){player.position.x + player.size.x/2, player.position.y + player.size.y/2, player.position.z + player.size.z/2}},
                                      wallLimits))
                        {
                            player.colour = RED;
                            resolveCollision(&player, wallLimits);
                        }

                    else player.colour = GREEN;

                    DrawCube((Vector3){foundPoints[i].x, 0.5, foundPoints[i].z}, 1.0f, 1.0f, 1.0f, BLUE);
                    DrawCubeWires((Vector3){foundPoints[i].x, 0.5, foundPoints[i].z}, 1.0f, 1.0f, 1.0f, BLACK);  // DRAW THE RANGE
                }

                DrawCubeWires(quadtreePos, quadtreeSize, 2.0f, quadtreeSize, DARKGRAY);  // DRAW ROOT NODE
                DrawCube(player.position, player.size.x, player.size.y, player.size.z, player.colour);  // DRAW PLAYER
                DrawCubeWires((Vector3){range.x, 1.0f, range.z}, range.halfWidth * 2, 2.0f, range.halfWidth * 2, GREEN);  // DRAW THE RANGE

                DrawGrid(quadtreeSize, TILE_SIZE);

            EndMode3D();

            DrawFPS(10, 10);
            //DrawText(TextFormat("player pos X = %f  \n\nY= %f  \n\nZ = %f", player.position.x, player.position.y, player.position.z), 30, 30, 30, DARKGRAY);
            if (IsKeyDown(KEY_RIGHT)) DrawText("RIGHT", 30, 700, 30, DARKGRAY);
            if (IsKeyDown(KEY_LEFT)) DrawText("LEFT", 30, 750, 30, DARKGRAY);
            if (IsKeyDown(KEY_DOWN)) DrawText("DOWN", 30, 800, 30, DARKGRAY);
            if (IsKeyDown(KEY_UP)) DrawText("UP", 30, 850, 30, DARKGRAY); 

        EndDrawing();

        // Free memory
        freeQuadtree(root);
    }

    free(walls);
    CloseWindow();        // Close window and OpenGL context

    return 0;
}
