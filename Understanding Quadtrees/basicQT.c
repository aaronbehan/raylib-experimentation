#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

typedef struct Boundary {
    float x;
    float y;
    float halfWidth;
} Boundary;

typedef struct Point {
    float x, z;
} Point;

typedef struct QuadtreeNode {
    Boundary boundary;               // Region boundary
    Point* points;                   // Points in this node
    int pointCount;                  // Number of points stored
    int capacity;                    // Maximum points this node can hold before splitting
    struct QuadtreeNode* children[4]; // Child nodes (NW, NE, SW, SE)
    int isDivided;                   // Indicates whether the node has been divided
} QuadtreeNode;

// ^-------------------------------- STRUCTS ----------------------------------^


// Create a new boundary
Boundary createBoundary(float x, float y, float halfWidth) {
    Boundary b;
    b.x = x;
    b.y = y;
    b.halfWidth = halfWidth;
    return b;  // returns a Boundary with x, y and halfwidth variables
}


// CREATE A NEW QUADTREE node
QuadtreeNode* createQuadtreeNode(Boundary boundary, int capacity) {
    QuadtreeNode* node = (QuadtreeNode*)malloc(sizeof(QuadtreeNode));  // creating a pointer to a QuadtreeNode, allocating space for 1 QuadtreeNode
    node->boundary = boundary;
    node->points = (Point*)malloc(capacity * sizeof(Point));  // allocating (capacity * Point) space in memory
    node->pointCount = 0;
    node->capacity = capacity;  // capacity normally set to 4
    node->isDivided = 0;  // false by default

    for (int i = 0; i < 4; i++) {
        node->children[i] = NULL;
    }
    return node;
}


// Check if a point is within a boundary
int containsPoint(Boundary boundary, Point p) {
    return (p.x >= boundary.x - boundary.halfWidth &&  // checks that p.x is greater than boundary.min.x
            p.x <= boundary.x + boundary.halfWidth &&  // checks that p.x is smaller than boundary.max.x
            p.z >= boundary.y - boundary.halfWidth &&  // checks that p.z is greater than boundary.min.y
            p.z <= boundary.y + boundary.halfWidth);   // checks that p.z is smaller than boundary.max.y
}  // if every single condition here is met then that means that p is INSIDE boundary


// SUBDIVIDE the quadtree node into four quadrants
void subdivide(QuadtreeNode* node) {
    float x = node->boundary.x;
    float y = node->boundary.y;
    float hw = node->boundary.halfWidth / 2;

    node->children[0] = createQuadtreeNode(createBoundary(x - hw, y + hw, hw), node->capacity); // NW
    node->children[1] = createQuadtreeNode(createBoundary(x + hw, y + hw, hw), node->capacity); // NE
    node->children[2] = createQuadtreeNode(createBoundary(x - hw, y - hw, hw), node->capacity); // SW
    node->children[3] = createQuadtreeNode(createBoundary(x + hw, y - hw, hw), node->capacity); // SE
    node->isDivided = 1;  // Setting boolean value to true to prevent this parent from being divided again
}


// INSERT a point into the quadtree. This function will continue recursively until it has stored the point. awesome. 
int insert(QuadtreeNode* node, Point p) {  // p is just a 2D vector
    // CANCELS function immediately if point is OUTSIDE THE NODE (boundary is just x, y, halfwidth)
    if (!containsPoint(node->boundary, p)) {
        return 0;
    }

    // If the node has space, add the point. If the node is full, continue to next if statement. 
    if (node->pointCount < node->capacity) 
    {
        node->points[node->pointCount++] = p;  // adding p to the space in points[] equal to the pointCount
        return 1;
    }

    // The node is already full, so divide it into 4 nodes (and remember that each of them has defined boundaries, meaning it will know if to ignore the Point using the first If condition of the Insert function and let its brother handle it)
    if (!node->isDivided) 
    {
        subdivide(node);
    }

    // Then, iterate through them, trying each time to insert the point into each child node until it finds a suitable space
    for (int i = 0; i < 4; i++) 
    {
        if (insert(node->children[i], p)) {
            return 1;
        }
    }

    return 0; // Program shouldn't reach here
}


// FREE the quadtree. This should be done after each frame of animation 
void freeQuadtree(QuadtreeNode* node) {
    if (node->isDivided) {
        for (int i = 0; i < 4; i++) {
            freeQuadtree(node->children[i]);
        }
    }
    free(node->points);
    free(node);
}

// ^------------------------------- FUNCTIONS --------------------------------------------------^



int main() 
{
    // Initialization
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "raylib quadtree");

    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
    float cameraSpeed = 0.5;


    Vector3 quadtreePos = { 0.0f, 1.0f, 0.0f };
    Vector3 quadtreeSize = { 20.0f, 2.0f, 20.0f };
    Vector3 pointSize = {0.05f, 1.0f, 0.05f};

    Point points[] = {{-5, -5}, {5, 5}, {3, 3}, {-2, -2}, {7, 7}, {8, 8}};

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

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                //                                 x    y    halfwdth
                Boundary boundary = createBoundary(0.0, 0.0, 10.0); // Create a boundary of 20x20
                QuadtreeNode* root = createQuadtreeNode(boundary, 4); // Root node with capacity of 4 Points

                // Insert points. Each of these points are mutable, either through player commands or AI. Each iteration through the game loop, we insert every one of them back into the tree, rebuilding it again and again and again to account for the changes made by the player and by the AI
                for (int i = 0; i < 6; i++) {        
                    if (insert(root, points[i])) // we put in a pointer to a QuadtreeNode and a Point(on the graph)
                    {
                        DrawCube((Vector3){points[i].x, 0.5, points[i].z}, pointSize.x, pointSize.y, pointSize.z, RED);
                    }
                    else // unless the coordinates fall outside of largest parent boundary, this should never trigger
                    {
                    
                    }
                }

                DrawCubeWires(quadtreePos, quadtreeSize.x, quadtreeSize.y, quadtreeSize.z, DARKGRAY);

                DrawGrid(20, 1.0f);   // Draw a grid

            EndMode3D();

            DrawFPS(10, 10);

        EndDrawing();

        // Free memory
        freeQuadtree(root);
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
