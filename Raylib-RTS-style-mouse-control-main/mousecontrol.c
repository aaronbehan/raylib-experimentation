#include "raylib.h"
#include "raymath.h"

#define FLT_MAX     340282346638528859811704183484516925440.0f     // Maximum value of a float, from bit pattern 01111111011111111111111111111111. I think Ray used this in the event of a rare edge case

//  debugging time. units only have to be selected once for their moveunit status to be set permantly to true

// Player units which can be controlled
typedef struct Unit {
    Vector3 unitPos;  // X = East and West. Y = Up and Down. Z = North and South.
    Vector3 waypoint; 
    Vector3 unitSize;
    float unitSpeed;
    bool selected;
    bool moveUnit;
    Color color;
} Unit;

BoundingBox createBoundingBox(Vector3 startPoint, Vector3 endPoint) // Creates a bounding box using its two furthest corners
{
    BoundingBox boundingBox = { 0 };

    if (startPoint.x <= endPoint.x) boundingBox.min.x = startPoint.x, boundingBox.max.x = endPoint.x;  // Expected mouse drag direction
    else if (startPoint.x > endPoint.x) boundingBox.min.x = endPoint.x, boundingBox.max.x = startPoint.x;  // Unexpected 
    
    if (startPoint.z <= endPoint.z) boundingBox.min.z = startPoint.z, boundingBox.max.z = endPoint.z;  // Expected mouse drag direction
    else if (startPoint.z > endPoint.z) boundingBox.min.z = endPoint.z, boundingBox.max.z = startPoint.z;  // Unexpected

    boundingBox.min.y = 0.0f;
    boundingBox.max.y = 0.5f; // Arbitrary height value 
    
    return boundingBox;
}

int main(void)
{
    // Screen
    const int screenWidth = 1400;
    const int screenHeight = 850;
    InitWindow(screenWidth, screenHeight, "raylib concepts and experimentation");

    // Camera
    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };
    float cameraSpeed = 0.5;
    Ray ray = { 0 };

    // Ground quad. Allows mouse rays to collide with the ground and return a 3D coordinate
    Vector3 g0 = (Vector3){ -50.0f, 0.0f, -50.0f };
    Vector3 g1 = (Vector3){ -50.0f, 0.0f,  50.0f };
    Vector3 g2 = (Vector3){  50.0f, 0.0f,  50.0f };
    Vector3 g3 = (Vector3){  50.0f, 0.0f, -50.0f };

    RayCollision objectCollision = { 0 };  // For casting a ray at an OBJECT. Stores collision hit info
    RayCollision groundCollision = { 0 };  // For casting a ray at the GROUND in order to return 3D coordinates 
    Vector3 wayPoint = { 0 };  // Saves a 3D coordinate. Used for unit control 

    #define NUMBER_OF_PLAYER_UNITS 2

    // Player controlled units
    Unit playerUnit1 = { { 0.0f, 1.0f, -4.0f }, { 0.0f, 1.0f, -4.0f }, { 1.0f, 2.0f, 1.0f }, 0.01f, false, false, GRAY}; 
    Unit playerUnit2 = { { 4.0f, 1.0f, 3.0f }, { 4.0f, 1.0f, 3.0f }, { 1.0f, 2.0f, 1.0f }, 0.01f, false, false, ORANGE}; 

    Unit playerUnits[NUMBER_OF_PLAYER_UNITS] = {playerUnit1, playerUnit2};

    // For drawing a visual aid rectangle with the mouse
    Vector2 saveMousePos2D = { 0 };
    // For drawing a bounding box in 3D space behind the above rectangle. Cannot draw the bounding box until mouse is released because user may drag upwards or downwards, left or right, and we cannot know where to put the min and the max until we have defined the intended collision space
    Vector3 save3DMouseStartPos = { 0 };
    Vector3 save3DMouseEndPos = { 0 };

    bool showDebug = false;  // option to show debug stats

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second

    // Main game loop -------------------------------------------------------------------------------
    while (!WindowShouldClose())
    {
        // Camera controls
        if (IsKeyDown(KEY_W)) camera.position.z -= cameraSpeed, camera.target.z -= cameraSpeed;
        if (IsKeyDown(KEY_A)) camera.position.x -= cameraSpeed, camera.target.x -= cameraSpeed;
        if (IsKeyDown(KEY_S)) camera.position.z += cameraSpeed, camera.target.z += cameraSpeed;
        if (IsKeyDown(KEY_D)) camera.position.x += cameraSpeed, camera.target.x += cameraSpeed;
        camera.position.y -= (GetMouseWheelMove()*cameraSpeed);  // CAMERA UP
        camera.target.y -= (GetMouseWheelMove()*cameraSpeed);  
        camera.position.z -= (GetMouseWheelMove()*0.6);

        if ((IsKeyPressed(KEY_SPACE)) && (!showDebug)) showDebug = true;
        if ((IsKeyPressed(KEY_P)) && (showDebug)) showDebug = false;
        
        // re-implement the mechanic which returned the name of the object being pointed at (eg, tower, triangle, ground, etc.)
        groundCollision.distance = FLT_MAX;
        groundCollision.hit = false;

        // Get ray and test against objects
        ray = GetMouseRay(GetMousePosition(), camera);  // CAST THE RAY
        RayCollision groundHitInfo = GetRayCollisionQuad(ray, g0, g1, g2, g3);  // I THINK WE RETURN WHERE THE RAY IS TOUCHING THE GROUND QUAD 
        if ((groundHitInfo.hit) && (groundHitInfo.distance < groundCollision.distance)) groundCollision = groundHitInfo;  // just confirming that the cursor is making contact with the ground i think


        // MOUSE CONTROLS ----------------------------------------------------------------------
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            // At the start of each click, we assume that no object collision has occurred until we evaluate with below functions
            objectCollision.hit = false;

            // Saving coordinate to start as the top left corner of a visual aid rectangle
            saveMousePos2D.x = GetMousePosition().x;
            saveMousePos2D.y = GetMousePosition().y;  
            
            // Deciding where to put ONE CORNER of the cuboid which will we will use to define bounding box. Y coordinate is static
            save3DMouseStartPos.x = groundCollision.point.x;
            save3DMouseStartPos.z = groundCollision.point.z;
            
            // Check collision between ray and each player unit (except the ground)
            for (int i = 0; i < NUMBER_OF_PLAYER_UNITS; i++) {
                objectCollision = GetRayCollisionBox(ray,
                                        (BoundingBox){(Vector3){ playerUnits[i].unitPos.x - playerUnits[i].unitSize.x/2, playerUnits[i].unitPos.y - playerUnits[i].unitSize.y/2, playerUnits[i].unitPos.z - playerUnits[i].unitSize.z/2 },
                                        (Vector3){ playerUnits[i].unitPos.x + playerUnits[i].unitSize.x/2, playerUnits[i].unitPos.y + playerUnits[i].unitSize.y/2, playerUnits[i].unitPos.z + playerUnits[i].unitSize.z/2 }});

                playerUnits[i].selected = objectCollision.hit;
            }
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) 
        {
            // Defining where the other corner of our bounding box will be
            save3DMouseEndPos.x = groundCollision.point.x;
            save3DMouseEndPos.z = groundCollision.point.z; 

            BoundingBox boundingBox = createBoundingBox(save3DMouseStartPos, save3DMouseEndPos);
            
            for (int i = 0; i < NUMBER_OF_PLAYER_UNITS; i++) {
                if (CheckCollisionBoxes(boundingBox, (BoundingBox){(Vector3){ playerUnits[i].unitPos.x - playerUnits[i].unitSize.x/2, playerUnits[i].unitPos.y - playerUnits[i].unitSize.y/2, playerUnits[i].unitPos.z - playerUnits[i].unitSize.z/2 },
                                (Vector3){ playerUnits[i].unitPos.x + playerUnits[i].unitSize.x/2, playerUnits[i].unitPos.y + playerUnits[i].unitSize.y/2, playerUnits[i].unitPos.z + playerUnits[i].unitSize.z/2 }})) playerUnits[i].selected = true;
            }                               
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            // Draws a rectangle as a visual aid to the player
            DrawRectangleLines(saveMousePos2D.x, saveMousePos2D.y, GetMousePosition().x - saveMousePos2D.x, GetMousePosition().y - saveMousePos2D.y, BLUE);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))  // Move selected units with RMB. This is also how we will select targets for my units
        {
            for (int i = 0; i < NUMBER_OF_PLAYER_UNITS; i++)  // ITERATE THROUGH ALL UNITS
            {
                if (playerUnits[i].selected)  // ONLY WANT TO ALLOW SELECTED UNITS NEW WAYPOINTS
                {
                        playerUnits[i].waypoint.x = groundCollision.point.x;  // Setting a static coordinate for units to follow
                        playerUnits[i].waypoint.z = groundCollision.point.z;
                }
            }
        }
        // END OF MOUSE CONTROLS ----------------------------------------------------------------

        for (int i = 0; i < NUMBER_OF_PLAYER_UNITS; i++) // FOR MOVING SELECTED UNITS
        {
            if (playerUnits[i].unitPos.x < playerUnits[i].waypoint.x) playerUnits[i].unitPos.x += playerUnits[i].unitSpeed;  // these increments are large to achieve equillibrium between waypoint and unit coordinates
            if (playerUnits[i].unitPos.x > playerUnits[i].waypoint.x) playerUnits[i].unitPos.x -= playerUnits[i].unitSpeed;
            if (playerUnits[i].unitPos.z < playerUnits[i].waypoint.z) playerUnits[i].unitPos.z += playerUnits[i].unitSpeed;
            if (playerUnits[i].unitPos.z > playerUnits[i].waypoint.z) playerUnits[i].unitPos.z -= playerUnits[i].unitSpeed;
        }

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // Highlight them if they are selected/hit
                for (int i = 0; i < NUMBER_OF_PLAYER_UNITS; i++) {  // iterate through the units

                    DrawCube(playerUnits[i].unitPos, playerUnits[i].unitSize.x, playerUnits[i].unitSize.y, playerUnits[i].unitSize.z, playerUnits[i].color);  // unit 1
                    DrawCubeWires(playerUnits[i].unitPos, playerUnits[i].unitSize.x, playerUnits[i].unitSize.y, playerUnits[i].unitSize.z, DARKGRAY);
                
                    if (playerUnits[i].selected) // to check which of them are selected. then draw a wireframe around those that are
                    {
                        DrawCubeWires(playerUnits[i].unitPos, playerUnits[i].unitSize.x, playerUnits[i].unitSize.y, playerUnits[i].unitSize.z, MAROON);
                        DrawCubeWires(playerUnits[i].unitPos, playerUnits[i].unitSize.x + 0.2f, playerUnits[i].unitSize.y + 0.2f, playerUnits[i].unitSize.z + 0.2f, GREEN);
                    }
                }

                // This lets me see where the invisible bounding box will be created for debugging purposes
                DrawCube((Vector3){save3DMouseStartPos.x, 0.0f, save3DMouseStartPos.z}, 0.1f, 0.1f, 0.1f, GRAY);
                DrawCube((Vector3){save3DMouseEndPos.x, 0.0f, save3DMouseEndPos.z}, 0.1f, 0.1f, 0.1f, GRAY);

                Vector3 normalEnd;
                normalEnd.x = groundCollision.point.x + groundCollision.normal.x;
                normalEnd.y = groundCollision.point.y + groundCollision.normal.y;
                normalEnd.z = groundCollision.point.z + groundCollision.normal.z;

                // CREATING A LITTLE CUBE THAT STICKS TO THE GROUND AND FOLLOWS THE MOUSE
                DrawCube(groundCollision.point, 0.1f, 0.1f, 0.1f, GRAY);
                DrawLine3D(groundCollision.point, normalEnd, RED);
                
                DrawGrid(10, 1.0f);        // Draw a grid (slices, spacing)

            EndMode3D();

            DrawText("Select cuboid with left click and control it with right click.", 400, 100, 20, GRAY);

            if (showDebug) 
            {
                DrawFPS(10, 10);
                DrawText(TextFormat("playerUnits[0].moveUnit = %d,\n\nplayerUnits[0].selected= %d", playerUnits[0].moveUnit, playerUnits[0].selected), 30, 20, 30, LIGHTGRAY);
                DrawText(TextFormat("playerUnits[0].unitPos.x = %f,\n\nplayerUnits[0].unitPos.z= %f", playerUnits[0].unitPos.x, playerUnits[0].unitPos.z), 30, 220, 30, LIGHTGRAY);
                DrawText(TextFormat("wayPointX = %f,\n\nZ= %f", wayPoint.x, wayPoint.z), 30, 140, 30, LIGHTGRAY);
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context

    return 0;
}
