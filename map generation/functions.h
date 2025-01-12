#ifndef FUNCTIONS_H
#define FUNCTIONS_H

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
        { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 },  // 5
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
    float axisZ = ((MAXIMUM_ROWS * -1) / 2) + 0.5;  
    float axisX = ((MAXIMUM_COLUMNS * -1) / 2) + 0.5;

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
                walls[index].position.y = 0.25;
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

#endif