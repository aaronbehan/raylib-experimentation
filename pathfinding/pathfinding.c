#include <stdlib.h>
#include <stddef.h>  // for NULL definition
#include <math.h>
#include "raylib.h"
#include "structs.h"
#include "functions.h"

#define GRID_WIDTH 10
#define GRID_HEIGHT 10
#define TILE_SIZE 1.0f

// to do:
// make algorithm independent from built in grid. it should detect walls based on collision or wall data overlap, not whether a node is marked as a wall or not
// 

typedef struct PFNode {
    PlaneVector position;
    float gCost, hCost, fCost; // Costs
    struct PFNode* parent; // Parent node for path reconstruction
    int walkable; // 1 if walkable, 0 if not
} PFNode;

typedef struct PFPath {
    //PlaneVector* path;  //  can reduce it to planeVector later on
    PFNode* node;
    int numberOfTiles;
} PFPath;

// Function to calculate the heuristic (Euclidean distance)
float calculateHeuristic(PFNode* a, PFNode* b) {
    return sqrtf((a->position.x - b->position.x) * (a->position.x - b->position.x) + (a->position.z - b->position.z) * (a->position.z - b->position.z));
}

// Function to find the node with the lowest fCost in the open list
PFNode* findLowestFCostNode(PFNode** openList, int openListSize) // whenever an array is passed as an argument, the array decays to a pointer to its first element. hence the pointer to a pointer PFNode**
{
    PFNode* lowest = openList[0];  // asserts that the lowest F cost is openList[0]
    for (int i = 1; i < openListSize; i++) // then checks that fact against all other F costs in openList[]
    {
        if (openList[i]->fCost < lowest->fCost || 
           (openList[i]->fCost == lowest->fCost && openList[i]->hCost < lowest->hCost)) {  // lower H cost is the deciding factor when considering F costs of equivalent values
            lowest = openList[i];
        }
    }
    return lowest;
}

// Checks if the node is present in the list
int isNodeInList(PFNode** list, int listSize, PFNode* node) 
{
    for (int i = 0; i < listSize; i++) 
    {
        if (list[i]->position.x == node->position.x && list[i]->position.z == node->position.z) {  // this can be simplified through use of vector struct
            return 1;
        }
    }
    return 0;
}

// Get neighbors of a node                              
int getNeighbors(PFNode grid[GRID_WIDTH][GRID_HEIGHT], PFNode* currentNode, PFNode** neighbors) {
    int count = 0;
    for (int dx = -1; dx <= 1; dx++) {  // starts at the west
        for (int dz = -1; dz <= 1; dz++) // south corner (southwest)
        {  
            if (dx == 0 && dz == 0) continue; // Skip the current node

            int nx = currentNode->position.x + dx;  // taking currentNode.x, adding dx (which could be a negative number) to it  and then storing it in nx
            int nz = currentNode->position.z + dz;  // taking currentNode.y, adding dy to it and then storing it in ny

            //    <<<<<ensure it is within the bounds of the map>>>>>>>         ensure not a wall
            if (nx >= 0 && nx < GRID_WIDTH && nz >= 0 && nz < GRID_HEIGHT && grid[nx][nz].walkable) {
                neighbors[count++] = &grid[nx][nz];  // add the node at grid[nx][ny] to neighbors and increment count by 1
            }
        }
    }
    return count;
}

// Reconstruct the path from the end node to the start node
PFPath reconstructPath(PFNode* endNode) 
{
    PFPath path;
    path.numberOfTiles = 0;

    PFNode* current = endNode;
    while (current != NULL) // The loop is purely to count path.numberOfTiles
    {
        current = current->parent; // the parent of the very first node will be null
        path.numberOfTiles++;
    }

    path.node = (PFNode*)malloc(path.numberOfTiles * sizeof(PFNode));
    
    current = endNode;
    for (int i = 0; i < path.numberOfTiles; i++) // Populating array with nodes going from end to start
    {
        path.node[i] = *current;
        current = current->parent; 
    }

    for (int i = 0; i < path.numberOfTiles / 2; i++) // Reversing the variable so that it's easy for a unit to follow
    {
        PFNode temp = path.node[i];
        path.node[i] = path.node[path.numberOfTiles - 1 - i];
        path.node[path.numberOfTiles - 1 - i] = temp;
    }

    return path; 
}

int main(void)
{
    const int screenWidth = 1400;
    const int screenHeight = 850;
    InitWindow(screenWidth, screenHeight, "raylib pf");
    Camera camera = { { 5.0f, 10.0f, 10.0f }, { 5.0f, 0.0f, 5.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    PFNode grid[GRID_WIDTH][GRID_HEIGHT];

    // Initialize the grid. Goes through every node in the grid and assigns it:
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int z = 0; z < GRID_HEIGHT; z++) {
            grid[x][z].position.x = x;  // coordinates
            grid[x][z].position.z = z;
            grid[x][z].gCost = grid[x][z].hCost = grid[x][z].fCost = 0;  // costs
            grid[x][z].parent = NULL;
            grid[x][z].walkable = 1; // All nodes are walkable by default
        }
    }

    // Reassigning some tiles to walls
    grid[4][4].walkable = 0;
    grid[4][5].walkable = 0;
    grid[4][6].walkable = 0;

    PFNode* startNode = &grid[0][0];
    PFNode* endNode = &grid[9][9];
    PFPath path = { 0 };

    //aStar(grid, startNode, endNode);

    PFNode* openList[GRID_WIDTH * GRID_HEIGHT];  // CREATING AN ARRAY WITH THE TOTAL AMOUNT OF TILES AS ITS SIZE
    int openListSize = 0;

    PFNode* closedList[GRID_WIDTH * GRID_HEIGHT];  // CREATING AN ARRAY WITH THE TOTAL AMOUNT OF TILES AS ITS SIZE
    int closedListSize = 0;

    openList[openListSize++] = startNode;  // putting the first node into the start list and then incrementing by 1?

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            PFNode* currentNode = findLowestFCostNode(openList, openListSize);  // assigns current node to node with lowest F cost

            // Remove currentNode from openList
            for (int i = 0; i < openListSize; i++) // iterates through openList
            {
                if (openList[i] == currentNode) // it finds the currentNode inside openList
                {
                    openList[i] = openList[--openListSize];  // it takes the very last element in openList and puts it at the address currentNode was found at. Then decreases size of openList by 1. Not so convinced by this
                    break;
                }
            }

            closedList[closedListSize++] = currentNode;  // adds currentNode to closedList and increases closedListSize by 1

            if (currentNode == endNode) // exit function if currentNode is endNode
            {  
                path = reconstructPath(endNode);
            }

            PFNode* neighbors[8];  // else: create a pointer to an array of 8 neighbours
            int neighborCount = getNeighbors(grid, currentNode, neighbors);  // stores 8 nodes (that neighbour currentNode) in neighbours array and returns number of neighbours created. only neighbours that are out of bounds or inside a wall do not get created

            for (int i = 0; i < neighborCount; i++) // iterates through the newly made neighbours array
            {
                PFNode* neighbor = neighbors[i];  

                if (isNodeInList(closedList, closedListSize, neighbor)) {
                    continue;  // continuing here means skip everything below it and move onto the next iteration
                }

                float tentativeGCost = currentNode->gCost + calculateHeuristic(currentNode, neighbor);  // this does work for getting the g cost. you take the g cost from parent node and add 1 or 1.4. i'm guessing these values are returned from the calculateH func

                if (!isNodeInList(openList, openListSize, neighbor)) // if node is not in the openList: add it to the open list. !!!!!!! significant step!
                {  
                    openList[openListSize++] = neighbor;
                } else if (tentativeGCost >= neighbor->gCost) {  // if node is in the list and the newly calculated gcost is greater than its previously calculated gcost, skip to next iteration
                    continue;
                }

                // initialise its members 
                neighbor->gCost = tentativeGCost;
                neighbor->hCost = calculateHeuristic(neighbor, endNode);
                neighbor->fCost = neighbor->gCost + neighbor->hCost;
                neighbor->parent = currentNode;
            }
        }

        // float axisZ = ((GRID_WIDTH * -1) / 2) + 0.5f;    // to offset the nodes
        // float axisX = ((GRID_HEIGHT * -1) / 2) + 0.5f;

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // walls
                DrawCube((Vector3){4, 0.5, 4}, 0.75f, 1.0f, 0.75f, GRAY);
                DrawCube((Vector3){4, 0.5, 5}, 0.75f, 1.0f, 0.75f, GRAY);
                DrawCube((Vector3){4, 0.5, 6}, 0.75f, 1.0f, 0.75f, GRAY);
                // end node
                DrawCube((Vector3){9, 0.5, 9}, 0.75f, 1.0f, 0.75f, GOLD);

                for (int i = 0; i < openListSize; i++) {
                    DrawCube((Vector3){openList[i]->position.x, 0.05, openList[i]->position.z}, 0.75f, 0.1f, 0.75f, GREEN);
                }                
                
                for (int i = 0; i < closedListSize; i++) {
                    DrawCube((Vector3){closedList[i]->position.x, 0.05, closedList[i]->position.z}, 0.75f, 0.1f, 0.75f, RED);
                }

                for (int i = 0; i < path.numberOfTiles; i++) {
                    DrawCube((Vector3){path.node[i].position.x, 0.05, path.node[i].position.z}, 0.75f, 0.2f, 0.75f, LIGHTGRAY);
                }

                DrawGrid(GRID_HEIGHT * 2, 1.0f);        // Draw a grid

            EndMode3D();

            DrawFPS(10, 10);
            if (path.numberOfTiles != 0) {
                for (int i = 0; i < path.numberOfTiles; i++) 
                {
                    DrawText(TextFormat("path.node[%d] = %.f, %.f", i, path.node[i].position.x, path.node[i].position.z), 30, i * 35, 30, DARKGRAY);
                }
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    free(path.node);
    CloseWindow();        // Close window and OpenGL context

    return 0;
}
