#include <stdlib.h>
#include <stddef.h>  // for NULL definition
#include <math.h>
#include "raylib.h"
#include "structs.h"
#include "functions.h"

#define TILE_SIZE 1.0f
#define ORTHOGONAL_COST 1.0f
#define DIAGONAL_COST 1.4f

// Function to calculate Diagonal Distance heuristic (best for my program as it uses graphs)
int calculateHCost(PlaneVector start, PlaneVector goal) 
{
    int dx = abs(goal.x - start.x);  // returns the difference and stores it in dx
    int dy = abs(goal.z - start.z);
    float HCostFloat = (ORTHOGONAL_COST * (dx > dy ? dx : dy)) + (DIAGONAL_COST - ORTHOGONAL_COST) * (dx < dy ? dx : dy);
    return (int){HCostFloat * 10};  // making the variable simpler to work with
}

// could not think of a better way to do this
void initPFNodeNeighbours(PlaneVector parentPosition, PlaneVector targetPosition, int parentGCost, PFNodeList open) 
{
    PathFindingNode north;
    north.position.x = parentPosition.x;
    north.position.z = parentPosition.z - TILE_SIZE;

    // check open, closed and walls

    north.parentPosition = parentPosition;
    north.GCost = parentGCost + 10;
    north.HCost = calculateHCost((PlaneVector){north.position.x, north.position.z}, targetPosition);
    north.FCost = north.GCost + north.HCost;
    open.pointer[0] = north;

    PathFindingNode northEast;
    northEast.position.x = parentPosition.x + TILE_SIZE;
    northEast.position.z = parentPosition.z - TILE_SIZE;
    northEast.parentPosition = parentPosition;
    northEast.GCost = parentGCost + 14;
    northEast.HCost = calculateHCost((PlaneVector){northEast.position.x, northEast.position.z}, targetPosition);
    northEast.FCost = northEast.GCost + northEast.HCost;
    open.pointer[1] = northEast;

    PathFindingNode east;
    east.position.x = parentPosition.x + TILE_SIZE;
    east.position.z = parentPosition.z;
    east.parentPosition = parentPosition;
    east.GCost = parentGCost + 10;
    east.HCost = calculateHCost((PlaneVector){east.position.x, east.position.z}, targetPosition);
    east.FCost = east.GCost + east.HCost;
    open.pointer[2] = east;

    PathFindingNode southEast;
    southEast.position.x = parentPosition.x + TILE_SIZE;
    southEast.position.z = parentPosition.z + TILE_SIZE;
    southEast.parentPosition = parentPosition;
    southEast.GCost = parentGCost + 14;
    southEast.HCost = calculateHCost((PlaneVector){southEast.position.x, southEast.position.z}, targetPosition);
    southEast.FCost = southEast.GCost + southEast.HCost;
    open.pointer[3] = southEast;

    PathFindingNode south;
    south.position.x = parentPosition.x;
    south.position.z = parentPosition.z + TILE_SIZE;
    south.parentPosition = parentPosition;
    south.GCost = parentGCost + 10;
    south.HCost = calculateHCost((PlaneVector){south.position.x, south.position.z}, targetPosition);
    south.FCost = south.GCost + south.HCost;
    open.pointer[4] = south;

    PathFindingNode southWest;
    southWest.position.x = parentPosition.x - TILE_SIZE;
    southWest.position.z = parentPosition.z + TILE_SIZE;
    southWest.parentPosition = parentPosition;
    southWest.GCost = parentGCost + 14;
    southWest.HCost = calculateHCost((PlaneVector){southWest.position.x, southWest.position.z}, targetPosition);
    southWest.FCost = southWest.GCost + southWest.HCost;
    open.pointer[5] = southWest;

    PathFindingNode west;
    west.position.x = parentPosition.x - TILE_SIZE;
    west.position.z = parentPosition.z;
    west.parentPosition = parentPosition;
    west.GCost = parentGCost + 10;
    west.HCost = calculateHCost((PlaneVector){west.position.x, west.position.z}, targetPosition);
    west.FCost = west.GCost + west.HCost;
    open.pointer[6] = west;

    PathFindingNode northWest;
    northWest.position.x = parentPosition.x - TILE_SIZE;
    northWest.position.z = parentPosition.z - TILE_SIZE;
    northWest.parentPosition = parentPosition;
    northWest.GCost = parentGCost + 14;
    northWest.HCost = calculateHCost((PlaneVector){northWest.position.x, northWest.position.z}, targetPosition);
    northWest.FCost = northWest.GCost + northWest.HCost;
    open.pointer[7] = northWest;

    //check the intended position against closed and walls (you must indeed check open because you must overwrite if it yields a lower F cost) 

    // for i < 8:
        // iterate through the positions of open, checking for duplicates.
        // if duplicate found: check its F cost and replace it with the new one if it is lower
        // check the closed array for duplicates

    // realloc with ~8 extra spaces for open?

    // put current node into closed
}

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

PlaneVector* FindPath(PlaneVector currentPosition, PlaneVector targetPosition) // must be a pointer because we don't know how many parent nodes it will have taken in order to relay the path
{
    PlaneVector* path = { 0 };

    return path;
}

int main(void) 
{
    const int screenWidth = 1400;
    const int screenHeight = 850;
    InitWindow(screenWidth, screenHeight, "raylib path finding");
    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    // Start and end nodes
    PathFindingNode startNode = {{ -2.5, -2.5 }, { 0 }, 0, 0, 0 }; 
    PathFindingNode endNode = {{ 1.5, 0.5 }, { 0 }, 0, 0, 0 };

    PFNodeList openNodes;
    openNodes.counter = 8;
    openNodes.pointer = (PathFindingNode*)malloc(openNodes.counter * sizeof(PathFindingNode));

    initPFNodeNeighbours(startNode.position, endNode.position, startNode.GCost, openNodes);

    // PFNodeList closedNodes = {NULL, 0};

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second

    int debugging_int = 0;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Calculate heuristics
        int hCost = calculateHCost(startNode.position, endNode.position);

        //start node controls
        if (IsKeyPressed(KEY_RIGHT)) startNode.position.x += TILE_SIZE;
        if (IsKeyPressed(KEY_LEFT)) startNode.position.x -= TILE_SIZE;
        if (IsKeyPressed(KEY_DOWN)) startNode.position.z += TILE_SIZE;
        if (IsKeyPressed(KEY_UP)) startNode.position.z -= TILE_SIZE;        
        //end node controls
        if (IsKeyPressed(KEY_D)) endNode.position.x += TILE_SIZE;
        if (IsKeyPressed(KEY_A)) endNode.position.x -= TILE_SIZE;
        if (IsKeyPressed(KEY_S)) endNode.position.z += TILE_SIZE;
        if (IsKeyPressed(KEY_W)) endNode.position.z -= TILE_SIZE;

        if (IsKeyPressed(KEY_Q)) debugging_int -= 1;
        if (IsKeyPressed(KEY_E)) debugging_int += 1;

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawCube((Vector3){startNode.position.x, 0, startNode.position.z}, TILE_SIZE, 0.1, TILE_SIZE, BLUE);
                DrawCube((Vector3){endNode.position.x, 0, endNode.position.z}, TILE_SIZE, 0.1, TILE_SIZE, LIGHTGRAY);

                for (int i = 0; i < 8; i++) // drawing child nodes
                {
                    DrawCube((Vector3){openNodes.pointer[i].position.x, 0, openNodes.pointer[i].position.z}, TILE_SIZE, 0.1, TILE_SIZE, LIGHTGRAY);
                }
                // DrawCube((Vector3){openNodes.pointer[0].position.x, 0, openNodes.pointer[0].position.z}, TILE_SIZE, 0.1, TILE_SIZE, LIGHTGRAY);
                // DrawCube((Vector3){openNodes.pointer[1].position.x, 0, openNodes.pointer[1].position.z}, TILE_SIZE, 0.1, TILE_SIZE, LIGHTGRAY);

                DrawCubeWires((Vector3){openNodes.pointer[debugging_int].position.x, 0, openNodes.pointer[debugging_int].position.z}, TILE_SIZE + 0.1, 0.15, TILE_SIZE + 0.1, GREEN);


                DrawGrid(20, 1.0f);  // Draw a grid

            EndMode3D();

            // debug
            DrawText(TextFormat("Start node X = %f, Z = %f", startNode.position.x, startNode.position.z), 20, 40, 30, GRAY);
            DrawText(TextFormat("End node X = %f, Z = %f", endNode.position.x, endNode.position.z), 20, 75, 30, GRAY);
            DrawText(TextFormat("dx = %d, dy = %d", (abs(endNode.position.x - startNode.position.x)), abs(endNode.position.z - startNode.position.z)), 20, 110, 30, GRAY);

            DrawText(TextFormat("first half of h cost = %f", (1.0f * ((abs(endNode.position.x - startNode.position.x)) > abs(endNode.position.z - startNode.position.z) ? (abs(endNode.position.x - startNode.position.x)) : abs(endNode.position.z - startNode.position.z)))), 20, 450, 30, GRAY);
            DrawText(TextFormat("second half of h cost = %f", (0.4f * ((abs(endNode.position.x - startNode.position.x)) < abs(endNode.position.z - startNode.position.z) ? (abs(endNode.position.x - startNode.position.x)) : abs(endNode.position.z - startNode.position.z)))), 20, 485, 30, GRAY);
            
            // DrawText(TextFormat("H cost = %d", hCost), 20, 715, 30, GRAY);
            DrawText(TextFormat("H cost = %d", hCost), 20, 750, 30, GRAY);


            DrawText(TextFormat("Selected node's costs:"), 800, 5, 30, GRAY);

            DrawText(TextFormat("G cost = %d", openNodes.pointer[debugging_int].GCost), 800, 35, 30, GRAY);
            DrawText(TextFormat("H cost = %d", openNodes.pointer[debugging_int].HCost), 800, 70, 30, GRAY);
            DrawText(TextFormat("F cost = %d", openNodes.pointer[debugging_int].FCost), 800, 115, 30, GRAY);
            
            DrawFPS(10, 5);

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
