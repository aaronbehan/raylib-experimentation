#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Map generation ----------- 

Wall InitialiseWall(int ID) 
{
    Wall wall = { 0 };

    if (ID == 1) 
    {
        wall.health = 20;
        wall.colour = GRAY;
    }
    else if (ID == 2)
    {
        wall.health = 25;
        wall.colour = PINK;
    }

    return wall;
}

Wall *CompileMapData(int *numberOfWalls) // eventually perhaps this can derive array data from an external source
{
    #define MAXIMUM_ROWS 18  // IMPORTANT TO UPDATE THESE IF THE MAP DIMENSION LIMITS INCREASE 
    #define MAXIMUM_COLUMNS 24

    // WALLS = 1, EMPTY SPACE = 0
    int layout[MAXIMUM_ROWS][MAXIMUM_COLUMNS] = {
        //0, 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23
        { 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1 },  // 0
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },  // 1
        { 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },  // 2
        { 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 3
        { 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2 },  // 4
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 5
        { 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2 },  // 6
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 7
        { 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2 },  // 8
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 9
        { 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2 },  // 10
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 11
        { 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2 },  // 12
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 13
        { 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 2 },  // 14
        { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 16
        { 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2 }   // 17
    };

    int sumOfWalls = 0;

    // Evaluating characteristics of the array so that we can inform malloc
    for (int row = 0; row < MAXIMUM_ROWS; row++) {
        for (int column = 0; column < MAXIMUM_COLUMNS; column++) {
            if (layout[row][column] > 0) sumOfWalls ++;
        }
    }

    Wall *walls = (Wall*)malloc(sumOfWalls * sizeof(Wall));  // Creating the dynamic array for all walls on the map
    *numberOfWalls = sumOfWalls;

    // Below equation translates the coordinate into 3D space based on array size. Starts at coordinate like {-12, -9}
    float axisZ = ((MAXIMUM_ROWS * -1) / 2) + 0.5f;  
    float axisX = ((MAXIMUM_COLUMNS * -1) / 2) + 0.5f;

    int index = 0;  // Index to help us insert each Wall inside *walls

    // Iterate once more through layout[], this time cutting out the 0s and populating the dynamic array
    for (int i = 0; i < MAXIMUM_ROWS; i++) {
        for (int j = 0; j < MAXIMUM_COLUMNS; j++) 
        {
            if (layout[i][j] > 0)
            {
                int ID = layout[i][j];
                walls[index] = InitialiseWall(ID);
                walls[index].position.x = axisX;
                walls[index].position.y = 0.5;
                walls[index].position.z = axisZ;

                index++;  // moving onto the next piece of wall
            }
            axisX++;
        }
        axisX = ((MAXIMUM_COLUMNS * -1) / 2) + 0.5;
        axisZ++;
    }
    return walls;
}

// Beginnings of a random map generating algorithm based off of Random Walk 

typedef struct Vector2Int 
{
    int x;
    int y;
} Vector2Int;

void randomDirection(Vector2Int* position)
{
    int randomNumber = GetRandomValue(0, 3);

    if (randomNumber == 0) position->x ++; // right
    else if (randomNumber == 1) position->x --;  // left
    else if (randomNumber == 2) position->y ++; // down
    else if (randomNumber == 3) position->y --;  // up

    // i do not account for the vector attempting to go out of bounds. will crash program
}

Wall* initialiseMapData(int walkLength, int* numberOfWalls)
{
    // maybe start pos should always be the same at 0, 0. in order to use the function recursively, i should have start pos as arg

    // Create a grid of size NxM
    // Choose a random starting position in the grid
    // Set the position as visited
    // Choose a new random position by moving a single cell from the current position (go left / up / right / down)
    // If the position is valid (the position is not out of bonds for the grid), set this new position as the current position
    // Go back to 4 and iterate until the completion condition is fulfilled (for example number of iterations)


    #define N 30
    #define M 30

    Vector2Int startPos = {(N/2), (M/2)};  // starting in the middle of the map right? 

    // Initialise map
    int map[N][M] = { 0 };

    // Generate walls 
    for (int i = 0; i < walkLength; i++) 
    {
        map[startPos.y][startPos.x] = 1;
        randomDirection(&startPos);
    }

    int sumOfWalls = 0;  // probably not necessary to have both sumOfWalls and numberOfWalls

    // Evaluating characteristics of the array so that we can inform malloc
    for (int row = 0; row < N; row++) {
        for (int column = 0; column < M; column++) {
            if (map[row][column] > 0) sumOfWalls ++;
        }
    }

    Wall *walls = (Wall*)malloc(sumOfWalls * sizeof(Wall));  // Creating the dynamic array for all walls on the map
    *numberOfWalls = sumOfWalls;

    // Below equation translates the coordinate into 3D space based on array size. Starts at coordinate like {-12, -9}
    float axisZ = ((N * -1) / 2);
    float axisX = ((M * -1) / 2);

    int index = 0;  // Index to help us insert each Wall inside *walls

    // Iterate once more through map, this time cutting out the 0s and populating the dynamic array
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) 
        {
            if (map[i][j] > 0)
            {
                int ID = map[i][j];
                walls[index] = InitialiseWall(ID);
                walls[index].position.x = axisX;
                walls[index].position.y = 0.25;
                walls[index].position.z = axisZ;

                index++;  // moving onto the next piece of wall
            }
            axisX++;
        }
        axisX = ((M * -1) / 2) + 0.5;
        axisZ++;
    }

    return walls;
}

// Quadtree related ----------------------------

// Create a new boundary
Boundary createBoundary(float x, float z, float halfWidth) {
    Boundary b;
    b.x = x;
    b.z = z;
    b.halfWidth = halfWidth;
    return b;
}

// CREATE A NEW QUADTREE node
QuadtreeNode* createQuadtreeNode(Boundary boundary, int capacity) {
    QuadtreeNode* node = (QuadtreeNode*)malloc(sizeof(QuadtreeNode));  // creating a pointer to a QuadtreeNode, allocating space for 1 QuadtreeNode
    node->boundary = boundary;
    node->points = (Vector3*)malloc(capacity * sizeof(Vector3));  // allocating (capacity * Point) space in memory
    node->pointCount = 0;
    node->capacity = capacity;  // capacity normally set to 4
    node->isDivided = 0;  // false by default

    for (int i = 0; i < 4; i++) {
        node->children[i] = NULL;
    }
    return node;
}


// Check if a point IS INSIDE a boundary
int containsPoint(Boundary boundary, Vector3 p) {
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
    float z = node->boundary.z;
    float hw = node->boundary.halfWidth / 2;

    node->children[0] = createQuadtreeNode(createBoundary(x - hw, z + hw, hw), node->capacity); // NW
    node->children[1] = createQuadtreeNode(createBoundary(x + hw, z + hw, hw), node->capacity); // NE
    node->children[2] = createQuadtreeNode(createBoundary(x - hw, z - hw, hw), node->capacity); // SW
    node->children[3] = createQuadtreeNode(createBoundary(x + hw, z - hw, hw), node->capacity); // SE
    node->isDivided = 1;  // Setting boolean value to true to prevent this parent from being divided again
}


// INSERT a point into the quadtree. This function will continue recursively until it has stored the point. awesome. 
int insert(QuadtreeNode* node, Vector3 p) {  // p is just a 2D vector
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
void queryRange(QuadtreeNode* node, Boundary range, Vector3* foundPoints, int* foundCount) {
    
    // Check if the query range does not intersect this node's boundary
    if (!intersectsBoundary(node->boundary, range)) {  // disqualify the node immediately if it does not intersect with our range of interest
        return;
    }

    // Check points in this node
    for (int i = 0; i < node->pointCount; i++) {
        if (containsPoint(range, node->points[i])) {  // if the point is contained in the range,
            foundPoints[(*foundCount)++] = node->points[i];  // store it in foundPoints array at the index dictated by foundCount integer   // SEGMENTATION ERROR
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

// ------------------------------------

#endif
