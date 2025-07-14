#include <stdlib.h>
#include <stddef.h>  // for NULL definition
#include <math.h>
#include <string.h>  // for memset
#include "raylib.h"
#include "structs.h"
#include "functions.h"

// THINGS TO DOUBLE CHECK:
// Double freeing pointers (if a pointer could live in both openList and closedList). Make sure that all pointers that are moved from openList to closedList instead of duplicated (eg. remove it from the array without freeing it)
// Dangling pointers (freeing while still referenced). Don't free(neighbor) if you stored that pointer in openList (or later, closedList). 
// Path reconstruction. reconstructPath makes a DEEP COPY of each node (path.node[i] = *current;). Therefore freeing the original nodes later is safe as the path keeps its own copies.
// Functionality reuse. If you restart the astar functionality in-game, run the same clean up loop for the lists before reallocating/repopulating them 

#define TILE_SIZE 1.0f

// Function to find the node with the lowest fCost in the open list. A star prioritises node with lowest fCost. Compatible.
PFNode* findLowestFCostNode(PFNode** openList, int openListSize) // whenever an array is passed as an argument, the array decays to a pointer to its first element. hence the pointer to a pointer PFNode**
{
    PFNode* lowest = openList[0];  // asserts that the lowest F cost is openList[0]
    for (int i = 1; i < openListSize; i++) // then checks that assertion against all other F costs in openList, starting at index 1
    {
        if (openList[i]->fCost < lowest->fCost || (openList[i]->fCost == lowest->fCost && openList[i]->hCost < lowest->hCost)) {  // lower H cost is the deciding factor when considering F costs of equivalent values
            lowest = openList[i];  // reassigns lowest to the newly discovered lower F cost
        }
    }
    return lowest;
}

// Reconstruct the path from the end node to the start node. Compatible. 
PFPath reconstructPath(PFNode* endNode) 
{
    PFPath path;
    path.numberOfTiles = 0;

    PFNode* current = endNode;
    while (current != NULL) // The loop is purely to count path.numberOfTiles variable
    {
        current = current->parent; // the parent of the very first node will be null
        path.numberOfTiles++;
    }

    path.node = (PFNode*)malloc(path.numberOfTiles * sizeof(PFNode));  // path.node is a pointer struct member and we must declare how many nodes go into it
    
    current = endNode;
    for (int i = 0; i < path.numberOfTiles; i++) // Populating array with nodes going from end to start
    {
        path.node[i] = *current;  // using pointers to each successive node from end to start
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

// Get neighbors of a node                              
int getNeighbors(Wall* walls, int numberOfWalls, PFNode* currentNode, PFNode** neighbors) {

    int count = 0;
    
    for (int dx = -1; dx <= 1; dx++) {  // starts at the west corner
        for (int dz = -1; dz <= 1; dz++) {  
            if (dx == 0 && dz == 0) continue; // Skip currentNode

            int nx = currentNode->position.x + dx;  // taking currentNode.x, adding dx (which could be a negative number) to it   and then storing it in nx
            int nz = currentNode->position.z + dz;  // taking currentNode.z, adding dz to it   and then storing it in nz

            for (int i = 0; i < numberOfWalls; i++) // iterate through every wall (we can use a quadtree later)
            {
                // the logic should be: if it iterates through all the walls and it has not found a wall that overlaps, create a node and store it in neighbors, but if it does find a wall, break before
                // it can make a node/iterate through the rest.

                // Wall overlap found
                if ((walls[i].position.x == nx) && (walls[i].position.z == nz)) {
                    break;  // Abort. Do not allow creation of node. 
                }
                
                if (i == (numberOfWalls - 1)) // on the very last iteration of the for loop
                {
                    PFNode* neighborNode = (PFNode*)calloc(1, sizeof(PFNode));  // calloc each time a neighbour is initialised? is this a good idea? 
                    neighborNode->position.x = nx;
                    neighborNode->position.z = nz;

                    neighbors[count++] = neighborNode;  // and add it to neighbours array while incrementing count by 1
                }
            }
        }
    }

    return count;

    // maybe some issue with nx, nz being ints and not floats. 
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

// Function to calculate the heuristic (Euclidean distance)
float calculateHeuristic(PFNode* a, PFNode* b) {
    return sqrtf((a->position.x - b->position.x) * (a->position.x - b->position.x) + (a->position.z - b->position.z) * (a->position.z - b->position.z));
}


int main(void)
{
    const int screenWidth = 1400;
    const int screenHeight = 850;
    InitWindow(screenWidth, screenHeight, "raylib pf");
    Camera camera = { { 5.0f, 10.0f, 10.0f }, { 5.0f, 0.0f, 5.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second

    // Initialising start and end nodes
    PFNode* startNode = (PFNode*)malloc(sizeof(PFNode));  // don't forget to free this and the endNode!
    startNode->position.x = 0;
    startNode->position.z = 0;
    startNode->gCost = 0.0f;
    startNode->hCost = 0.0f;
    startNode->fCost = 0.0f;
    startNode->parent = NULL;
    
    PFNode* endNode = (PFNode*)malloc(sizeof(PFNode));
    endNode->position.x = 9;
    endNode->position.z = 9;
    endNode->gCost = 0.0f;
    endNode->hCost = 0.0f;
    endNode->fCost = 0.0f;
    endNode->parent = NULL;

    PFPath path = { 0 };  // the path i can expect to return upon finding endNode

    int openListAllowance = 10;
    int closedListAllowance = 15;

    PFNode** openList = (PFNode**)malloc(sizeof(PFNode*) * openListAllowance);
    int openListSize = 0;

    PFNode** closedList = (PFNode**)malloc(sizeof(PFNode*) * closedListAllowance);  
    int closedListSize = 0;

    openList[openListSize++] = startNode;  // putting the first node into the start list and then incrementing by 1

    int numberOfWalls = 3;
    Wall* walls = (Wall*)calloc(numberOfWalls, sizeof(Wall));  // calloc is like malloc but initalises all bytes to 0. later we can link this pointer to actual map generation
    for (int i = 0; i < numberOfWalls; i++)
    {
        walls[i].position.x = 4;
        walls[i].position.z = 4 + i;
    }

    // Main game loop  -------------------------------------------------------------------------------------------------------
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // moved this here so that i can draw it to the screen for debugging 
        PFNode* currentNode = findLowestFCostNode(openList, openListSize);  // assigns node with lowest F cost to current node

        if (IsKeyPressed(KEY_SPACE))
        {
            // PFNode* currentNode = findLowestFCostNode(openList, openListSize);  // assigns node with lowest F cost to current node

            // Remove currentNode from openList
            for (int i = 0; i < openListSize; i++) // iterates through openList
            {
                if (openList[i] == currentNode) // it finds the currentNode inside openList
                {
                    openList[i] = openList[--openListSize];  // it takes the very last element in openList and puts it at the address currentNode was found at
                    break;
                } // i get that you're concerned about the ACTUAL CONTENTS of the list, but, because openlistsize is decremented to 0, it is basically the same as being empty because the next value to be added to the list will be at index 0 because it has been reduced to this value on the first iteration of the root for loop
            }
            
            if ((closedListSize + 1) >= closedListAllowance)  // Before adding current node to closedList, check if memory has been exceeded and add more if necessary
            {
                closedListAllowance = 2 * closedListSize;
                PFNode** temp = realloc(closedList, sizeof(PFNode) * (closedListAllowance));  // not sure whether PFNode needs to be a pointer or a pointer to a pointer
                if (temp) closedList = temp;
            }
            closedList[closedListSize++] = currentNode;  // adds currentNode to closedList and increases closedListSize by 1
            

            if ((currentNode->position.x == endNode->position.x) && (currentNode->position.z == endNode->position.z))  // exit function if currentNode has reached endNode coordinates
            {
                path = reconstructPath(currentNode);
            }

            PFNode* neighbors[8];  // creates a pointer to an array of 8 neighbours
            int neighborCount = getNeighbors(walls, numberOfWalls, currentNode, neighbors);  // stores 8 nodes (that neighbour currentNode) in neighbours array and returns number of neighbours created. only neighbours that are out of bounds or inside a wall do not get created

            for (int i = 0; i < neighborCount; i++) // iterates through the newly made neighbours array
            {
                PFNode* neighbor = neighbors[i];

                if (isNodeInList(closedList, closedListSize, neighbor)) {
                    free(neighbor);
                    continue;  // skip everything below it and move onto the next iteration
                }

                float tentativeGCost = currentNode->gCost + calculateHeuristic(currentNode, neighbor);  // this does work for getting the g cost. you take the g cost from parent node and add 1 or 1.4. i'm guessing these values are returned from the calculateH func

                if (!isNodeInList(openList, openListSize, neighbor)) // if node is not in the openList: add it to the open list. !!!!!!! significant step!
                {  
                    if ((openListSize + 1) >= openListAllowance) { // checking if more memory is needed
                        openListAllowance = 2 * openListSize;
                        PFNode** temp = realloc(openList, sizeof(PFNode) * (openListAllowance));  // is this honestly right??????????????????????
                        if (temp) openList = temp;
                    }
                    openList[openListSize++] = neighbor;
                } else if (tentativeGCost >= neighbor->gCost) {  // if node is in the list and the newly calculated gcost is greater than its previously calculated gcost, skip to next iteration
                    free(neighbor);
                    continue;
                }

                // initialise its members 
                neighbor->gCost = tentativeGCost;
                neighbor->hCost = calculateHeuristic(neighbor, endNode);
                neighbor->fCost = neighbor->gCost + neighbor->hCost;
                neighbor->parent = currentNode;
            }
        }

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // Draw actual start node
                DrawCube((Vector3){startNode->position.x, 0.05, startNode->position.z}, 0.75f, 0.1f, 0.75f, YELLOW);
                // Draw actual end node
                DrawCube((Vector3){endNode->position.x, 0.05, endNode->position.z}, 0.75f, 0.1f, 0.75f, BROWN);

                // Draw walls
                for (int i = 0; i < numberOfWalls; i++) {
                    DrawCube((Vector3){walls[i].position.x, 0.5, walls[i].position.z}, 0.75f, 1.0f, 0.75f, GRAY);
                }

                // Draw open nodes
                for (int i = 0; i < openListSize; i++) {
                    DrawCube((Vector3){openList[i]->position.x, 0.05, openList[i]->position.z}, 0.75f, 0.1f, 0.75f, GREEN);
                }
                
                // Draw closed nodes
                for (int i = 0; i < closedListSize; i++) {
                    DrawCube((Vector3){closedList[i]->position.x, 0.05, closedList[i]->position.z}, 0.75f, 0.1f, 0.75f, RED);
                }

                // Draw completed path
                for (int i = 0; i < path.numberOfTiles; i++) {
                    DrawCube((Vector3){path.node[i].position.x, 0.05, path.node[i].position.z}, 0.75f, 0.2f, 0.75f, LIGHTGRAY);
                }

                DrawGrid(50, TILE_SIZE);  // Draw a grid

            EndMode3D();

            // Debugging info --------------------------
            DrawText(TextFormat("Current node position = x%f, z%f", currentNode->position.x, endNode->position.z), 800, 40, 15, BLACK);
            DrawText(TextFormat("End node position = x%f, z%f", endNode->position.x, endNode->position.z), 800, 55, 15, BLACK);

            if ((currentNode->position.x == endNode->position.x) && (currentNode->position.z == endNode->position.z)) { DrawText(TextFormat("CurrentNode coords == EndNode coords", currentNode->position.x, endNode->position.z), 10, 40, 15, BLACK); }
            else { DrawText(TextFormat("CurrentNode != EndNode", currentNode->position.x, endNode->position.z), 10, 40, 15, BLACK); }

            if (openList[0] != NULL) 
            {
                DrawText(TextFormat("openListSize = %d", openListSize), 800, 10, 15, DARKGRAY);
                DrawText(TextFormat("closedListSize = %d", closedListSize), 800, 25, 15, DARKGRAY);
            }

            if (path.numberOfTiles != 0)
            {
                for (int i = 0; i < path.numberOfTiles; i++) 
                {
                    DrawText(TextFormat("path.node[%d] = %.f, %.f", i, path.node[i].position.x, path.node[i].position.z), 30, i * 35, 30, DARKGRAY);
                }
            }
            // -------------------------------------------

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    free(startNode);
    free(endNode);
    for (int i = 0; i < openListSize; i++) free(openList[i]);
    free(openList);
    for (int i = 0; i < closedListSize; i++) free(closedList[i]);
    free(closedList);
    free(walls);
    free(path.node);  // i think this should only be freed once the character has reached their destination. 
    CloseWindow();        // Close window and OpenGL context

    return 0;
}
