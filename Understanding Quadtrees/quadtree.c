#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

typedef struct Boundary {
    float x;
    float z;
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
Boundary createBoundary(float x, float z, float halfWidth) {
    Boundary b;
    b.x = x;
    b.z = z;
    b.halfWidth = halfWidth;
    return b;  // returns a Boundary with x, z and halfwidth variables
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


// Check if a point IS INSIDE a boundary
int containsPoint(Boundary boundary, Point p) {
    return (p.x >= boundary.x - boundary.halfWidth &&  // checks that p.x is greater than boundary.min.x
            p.x <= boundary.x + boundary.halfWidth &&  // checks that p.x is smaller than boundary.max.x
            p.z >= boundary.z - boundary.halfWidth &&  // checks that p.z is greater than boundary.min.z
            p.z <= boundary.z + boundary.halfWidth);   // checks that p.z is smaller than boundary.max.z
}  // if every single condition here is met then that means that p is INSIDE boundary

// Check if TWO BOUNDARIES INTERSECT
int intersectsBoundary(Boundary a, Boundary b) {
    return !(a.x - a.halfWidth > b.x + b.halfWidth || // checks that a.min.x is greater than b.max.x 
             a.x + a.halfWidth < b.x - b.halfWidth || // checks that a.max.x is less than b.min.x
             a.z - a.halfWidth > b.z + b.halfWidth || // checks that a.min.y is greater than b.max.y
             a.z + a.halfWidth < b.z - b.halfWidth);  // checks that a.max.y is less than b.min.y
}  // if any of these four conditions are found to be true, then it is impossible for the two boundaries to be overlapping. 


// SUBDIVIDE the quadtree node into four quadrants
void subdivide(QuadtreeNode* node) {
    float x = node->boundary.x;
    float y = node->boundary.z;
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

    // The node is already full. Check if it is divided. If not, divide it into 4 nodes (and remember that each of them has defined boundaries, meaning it will know if to ignore the Point using the first If condition of the Insert function and let its brother handle it)
    if (!node->isDivided) 
    {
        subdivide(node);
    }

    // Then, iterate through the 4 child nodes, trying each time to insert the point into each child node until it finds a suitable space
    for (int i = 0; i < 4; i++) 
    {
        if (insert(node->children[i], p)) {
            return 1;
        }
    }

    return 0; // Program shouldn't reach here
}

// QUERY points within a region  -------------------------------------
void queryRange(QuadtreeNode* node, Boundary range, Point* foundPoints, int* foundCount) {
    // Check if the query range does not intersect this node's boundary
    if (!intersectsBoundary(node->boundary, range)) {  // disqualify the node immediately if it does not intersect with our range of interest
        return;
    }

    // Check points in this node
    for (int i = 0; i < node->pointCount; i++) {
        if (containsPoint(range, node->points[i])) {  // if the point is contained in the range,
            foundPoints[(*foundCount)++] = node->points[i];  // store it in foundPoints array at the index dictated by foundCount integer
        }
    }

    // Then we check if the node is subdivided
    if (node->isDivided) {  // if yes, rinse and repeat on the child nodes
        for (int i = 0; i < 4; i++) {
            queryRange(node->children[i], range, foundPoints, foundCount);
        }
    }
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

// ^------------------------------- FUNCTIONS ------------------------------------------------------------------------------


int main() 
{
    // Initialization
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "raylib quadtree");

    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
    float cameraSpeed = 0.5;

    Vector3 playerPos = { 0.0f, 1.0f, 0.0f }; 
    Vector3 playerSize = { 1.0f, 2.0f, 1.0f };
    float playerSpeed = 0.1f;

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
        // Player controls
        if (IsKeyDown(KEY_RIGHT)) playerPos.x += playerSpeed;
        if (IsKeyDown(KEY_LEFT)) playerPos.x -= playerSpeed;
        if (IsKeyDown(KEY_DOWN)) playerPos.z += playerSpeed;
        if (IsKeyDown(KEY_UP)) playerPos.z -= playerSpeed;

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
                }

                // QUERY points within a region ----------------------------------------------------------------------------------------------------
                Boundary range = createBoundary(playerPos.x, playerPos.z, 5.0); // Query range (centered at 0,0 with halfWidth 5)
                Point foundPoints[10];  // array of points 
                int foundCount = 0;

                // feed in Quadtree root the largest parent node and a Boundary range. then feed empty array of points. then pointer to an int. 
                queryRange(root, range, foundPoints, &foundCount);

                for (int i = 0; i < foundCount; i++) {
                    DrawCube((Vector3){foundPoints[i].x, 0.5, foundPoints[i].z}, pointSize.x, pointSize.y, pointSize.z, BLUE);
                }

                DrawCubeWires(quadtreePos, quadtreeSize.x, quadtreeSize.y, quadtreeSize.z, DARKGRAY);  // DRAW ROOT NODE
                DrawCube(playerPos, playerSize.x, playerSize.y, playerSize.z, GRAY);  // DRAW PLAYER
                DrawCubeWires((Vector3){range.x, 1.0f, range.z}, 10.0f, 2.0f, 10.0f, GREEN);  // DRAW THE RANGE

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
