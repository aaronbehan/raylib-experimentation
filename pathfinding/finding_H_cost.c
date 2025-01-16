#include <math.h>
#include "raylib.h"

// Core principles

// G cost = distance from starting node 
// H cost (heuristic) = distance from end node (goal)
// F cost = G cost + H cost. 
//
// Assert that the distance between tiles is 1. The distance between a tile and its closest diagonal neighbour is √2 which is 1.4 (Pythagoras)
// Multiply these numbers by 10 on the basis that 10 and 14 are easier to work with.

// Idea
// 1. Algorithm begins at starting node. It calculates the G, H and F costs for its 8 neighbouring tiles. 
// 2. Algorithm looks at all nodes and chooses the one with the lowest F cost. If it is tied, prioritise lowest H cost.
// 3 It marks that node as CLOSED and calculates the G, H, F values for its 8 neighbours
// 3. 

// have a very tiny bounding box which returns whether a space is occupied or not
// it starts at node A, a coordinate on the map 
// it moves according to the grid, 1.0f tile at a time 
//

// Pseudo code
// OPEN // array of nodes to be evaluated
// CLOSED // array of nodes already evaluated
// add the start node to OPEN
 
// loop
// current = node in OPEN with the lowest f_cost
// remove current from OPEN
// add current to CLOSED

// if current is the target node //path has been found
//     return

// for each neighbour of the current node
//     if neighbour is not traversable or neighbour is in CLOSED
//         skip to the next neighbour

//     if new path to neighbour is shorter OR neighbour is not in OPEN
//         set f_cost of neighbour
//         set parent of neighbour to current
//         if neighbour is not in OPEN
//             add neighbour to OPEN

#define TILE_SIZE 1.0f

// Function to calculate Diagonal Distance heuristic (best for my program as it uses graphs)
double diagonal_distance(Vector3 start, Vector3 goal, double orthogonal_cost, double diagonal_cost) 
{
    // abs function returns an absolute value (eg: absolute of -10 = 10. absolute of 12 = 12)
    int dx = abs(goal.x - start.x);  // returns the difference and stores it in dx
    int dy = abs(goal.z - start.z);
    // orthogonal_cost * dx if dx is greater than dy. otherwise, multiply it by dy instead
    return (orthogonal_cost * (dx > dy ? dx : dy)) + (diagonal_cost - orthogonal_cost) * (dx < dy ? dx : dy);
}

int main(void) 
{
    const int screenWidth = 1400;
    const int screenHeight = 850;
    InitWindow(screenWidth, screenHeight, "raylib path finding");

    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    // Start and end nodes
    Vector3 startNode = { -2.5, 0.05, -2.5 };
    Vector3 endNode = { 1.5, 0.05, 0.5 };

    float orthogonal_cost = 1.0;   // Cost for orthogonal movement
    float diagonal_cost = 1.4;  // Cost for diagonal movement

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        // Calculate h cost
        double hCost = diagonal_distance(startNode, endNode, orthogonal_cost, diagonal_cost);

        //start node controls
        if (IsKeyPressed(KEY_RIGHT)) startNode.x += TILE_SIZE;
        if (IsKeyPressed(KEY_LEFT)) startNode.x -= TILE_SIZE;
        if (IsKeyPressed(KEY_DOWN)) startNode.z += TILE_SIZE;
        if (IsKeyPressed(KEY_UP)) startNode.z -= TILE_SIZE;        
        //end node controls
        if (IsKeyPressed(KEY_D)) endNode.x += TILE_SIZE;
        if (IsKeyPressed(KEY_A)) endNode.x -= TILE_SIZE;
        if (IsKeyPressed(KEY_S)) endNode.z += TILE_SIZE;
        if (IsKeyPressed(KEY_W)) endNode.z -= TILE_SIZE;

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawCube(startNode, TILE_SIZE, 0.1, TILE_SIZE, BLUE);
                DrawCube(endNode, TILE_SIZE, 0.1, TILE_SIZE, LIGHTGRAY);

                DrawGrid(20, 1.0f);        // Draw a grid

            EndMode3D();

            DrawText(TextFormat("Start node X = %f, Z = %f", startNode.x, startNode.z), 20, 40, 30, GRAY);
            DrawText(TextFormat("End node X = %f, Z = %f", endNode.x, endNode.z), 20, 75, 30, GRAY);
            DrawText(TextFormat("dx = %d, dy = %d", (abs(endNode.x - startNode.x)), abs(endNode.z - startNode.z)), 20, 110, 30, GRAY);

            DrawText(TextFormat("first half of h cost = %f", (1.0f * ((abs(endNode.x - startNode.x)) > abs(endNode.z - startNode.z) ? (abs(endNode.x - startNode.x)) : abs(endNode.z - startNode.z)))), 20, 450, 30, GRAY);
            DrawText(TextFormat("second half of h cost = %f", (0.4f * ((abs(endNode.x - startNode.x)) < abs(endNode.z - startNode.z) ? (abs(endNode.x - startNode.x)) : abs(endNode.z - startNode.z)))), 20, 485, 30, GRAY);
            
            DrawText(TextFormat("H cost = %f", hCost), 20, 750, 30, GRAY);
            
            DrawFPS(10, 5);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
