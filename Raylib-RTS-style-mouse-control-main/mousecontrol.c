#include "raylib.h"
#include "structs.h"
#include "functions.h"
#include "timer.h"
#include <stddef.h>  // FOR NULL DEFINITION

#define FLT_MAX     340282346638528859811704183484516925440.0f  // Maximum value of a float, from bit pattern 01111111011111111111111111111111. Necessary for ground detection, but cannot discern why yet
#define NUMBER_OF_PLAYER_UNITS 2  // VERY IMPORTANT TO KEEP THIS UPDATED. i suppose if you want to do something like take control of enemies then this approach should be revised
#define NUMBER_OF_ENEMY_UNITS 2

// upon right click w enemy and eventual collission w enemy, enemy.targeted.true unless right clicked away

int main(void)
{
    // Screen
    const int screenWidth = 1400;
    const int screenHeight = 850;
    InitWindow(screenWidth, screenHeight, "window title");

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

    // Player controlled units
    Unit playerUnit1 = { { 0.0f, 1.0f, -2.0f }, { 0 }, { 1.0f, 2.0f, 1.0f }, { 0 }, 0.05f, 0.5f, 40, 40, 6, false, 0, false, 0, RED};
    Unit playerUnit2 = { { 5.0f, 1.0f, 0.0f }, { 5.0f, 1.0f, 0.0f }, { 1.0f, 2.0f, 1.0f }, { 0 }, 0.05f, 0.5f, 40, 40, 6, false, 0, false, 0, ORANGE};
    // Unit playerUnit2 = { { 4.0f, 1.0f, 3.0f }, { 4.0f, 1.0f, 3.0f }, { 1.0f, 2.0f, 1.0f }, 0.01f, false, false, ORANGE}; 

    Unit playerUnits[NUMBER_OF_PLAYER_UNITS] = {playerUnit1, playerUnit2};

    // Enemies
    Unit enemyUnit1 =  { { -3.0f, 1.0f, 0.0f }, { 0 }, { 1.0f, 2.0f, 1.0f }, { 0 }, 0.05f, 0.5f, 40, 40, 1, false, 0, false, 0, PURPLE};
    Unit enemyUnit2 =  { { 3.0f, 1.0f, -4.0f }, { 0 }, { 1.0f, 2.0f, 1.0f }, { 0 }, 0.05f, 0.5f, 40, 40, 1, false, 0, false, 0, BLUE};

    Unit enemyUnits[NUMBER_OF_ENEMY_UNITS] = {enemyUnit1, enemyUnit2};

    // For drawing a visual aid rectangle with the mouse when selecting screen elements
    Vector2 saveMousePos2D = { 0 };
    // For drawing a bounding box in 3D space behind the above rectangle
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

        if (IsKeyDown(KEY_RIGHT)) enemyUnits[0].unitPos.x += 0.1f;
        if (IsKeyDown(KEY_LEFT)) enemyUnits[0].unitPos.x -= 0.1f;
        if (IsKeyDown(KEY_DOWN)) enemyUnits[0].unitPos.z += 0.1f;
        if (IsKeyDown(KEY_UP)) enemyUnits[0].unitPos.z -= 0.1f;

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

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            DrawCircle(GetMousePosition().x, GetMousePosition().y, 20.0, ORANGE);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))  // Move selected units with RMB. This is also how we will select targets for my units
        {
            for (int i = 0; i < NUMBER_OF_PLAYER_UNITS; i++)  // ITERATE THROUGH ALL PLAYER CONTROLLED UNITS IN ORDER TO UPDATE THEIR WAYPOINT
            {
                if (playerUnits[i].selected)  // ONLY WANT TO ALLOW SELECTED UNITS NEW WAYPOINTS
                {
                        playerUnits[i].waypoint.x = groundCollision.point.x;  // Setting a static coordinate for units to follow
                        playerUnits[i].waypoint.z = groundCollision.point.z;
                        // when deciding on animations, you could make a small bounding box around each waypoint and tie idling/attacking to that
                        // perhaps just rounding the huge floats is a better idea so that equillibrium can be attained. if waypoint == unitpos, play animation x
                }
            }

            objectCollision.hit = false;

            // Check collision between ray and every enemy unit (except the ground). This can be refined using a quadtree!
            for (int i = 0; i < NUMBER_OF_ENEMY_UNITS; i++) 
            {
                objectCollision = GetRayCollisionBox(ray,
                                        (BoundingBox){(Vector3){ enemyUnits[i].unitPos.x - enemyUnits[i].unitSize.x/2, enemyUnits[i].unitPos.y - enemyUnits[i].unitSize.y/2, enemyUnits[i].unitPos.z - enemyUnits[i].unitSize.z/2 },
                                        (Vector3){ enemyUnits[i].unitPos.x + enemyUnits[i].unitSize.x/2, enemyUnits[i].unitPos.y + enemyUnits[i].unitSize.y/2, enemyUnits[i].unitPos.z + enemyUnits[i].unitSize.z/2 }});

                if (objectCollision.hit) // Upon finding a collision between the mouse and an enemy
                {
                    for (int j = 0; j < NUMBER_OF_PLAYER_UNITS; j++)  // Iterate through all player controlled units
                    {
                        if (playerUnits[j].selected) // Identifying which of them are selected, so that they begin targeting the right clicked enemy
                        {
                            playerUnits[j].targeting = &enemyUnits[i].unitPos;  // this should allow the targeting pointer to update as the enemy position changes
                        }
                    }
                    break;  // Break out of parent for loop so that the targeting members are not overwritten
                }
                else // if no enemy collision was found,
                {
                    for (int j = 0; j < NUMBER_OF_PLAYER_UNITS; j++)  // All currently selected units should lose their target
                    {
                        if (playerUnits[j].selected) 
                        {
                            playerUnits[j].targeting = NULL;
                        }
                    }
                }
            }
        }
        // END OF MOUSE CONTROLS ----------------------------------------------------------------


        for (int i = 0; i < NUMBER_OF_PLAYER_UNITS; i++) // FOR MOVING SELECTED UNITS. Link this to the timer later on
        {
            // Following an enemy target
            // ... 
            
            // Following the waypoint
            if (playerUnits[i].unitPos.x < playerUnits[i].waypoint.x) playerUnits[i].unitPos.x += playerUnits[i].unitSpeed;  // these increments are too large to achieve equillibrium between waypoint and unit coordinates
            if (playerUnits[i].unitPos.x > playerUnits[i].waypoint.x) playerUnits[i].unitPos.x -= playerUnits[i].unitSpeed;
            if (playerUnits[i].unitPos.z < playerUnits[i].waypoint.z) playerUnits[i].unitPos.z += playerUnits[i].unitSpeed;
            if (playerUnits[i].unitPos.z > playerUnits[i].waypoint.z) playerUnits[i].unitPos.z -= playerUnits[i].unitSpeed;

        }

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // Draw player characters
                for (int i = 0; i < NUMBER_OF_PLAYER_UNITS; i++) {  // Iterate through the player units

                    DrawCube(playerUnits[i].unitPos, playerUnits[i].unitSize.x, playerUnits[i].unitSize.y, playerUnits[i].unitSize.z, playerUnits[i].colour); 
                    DrawCubeWires(playerUnits[i].unitPos, playerUnits[i].unitSize.x, playerUnits[i].unitSize.y, playerUnits[i].unitSize.z, DARKGRAY);
                
                    if (playerUnits[i].selected) // Draw a wireframe around those that are selected
                    {
                        DrawCubeWires(playerUnits[i].unitPos, playerUnits[i].unitSize.x, playerUnits[i].unitSize.y, playerUnits[i].unitSize.z, MAROON);
                        DrawCubeWires(playerUnits[i].unitPos, playerUnits[i].unitSize.x + 0.2f, playerUnits[i].unitSize.y + 0.2f, playerUnits[i].unitSize.z + 0.2f, GREEN);
                        // Wireframe around the enemy the selected unit is currently targeting
                        if (playerUnits[i].targeting != NULL) // Avoids a crash if the selected unit is not currently targeting anything
                        {
                            DrawCubeWires((Vector3){playerUnits[i].targeting->x, 1.0f, playerUnits[i].targeting->z}, 1.2f, 2.2f, 1.2f, YELLOW);
                        }
                    }
                }
                
                // Draw enemies
                for (int i = 0; i < NUMBER_OF_ENEMY_UNITS; i++) 
                {
                    DrawCube(enemyUnits[i].unitPos, enemyUnits[i].unitSize.x, enemyUnits[i].unitSize.y, enemyUnits[i].unitSize.z, enemyUnits[i].colour); 
                    DrawCubeWires(enemyUnits[i].unitPos, enemyUnits[i].unitSize.x, enemyUnits[i].unitSize.y, enemyUnits[i].unitSize.z, DARKGRAY);
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
                
                DrawGrid(10, 1.0f);  // Draw a grid (slices, spacing)

            EndMode3D();

            DrawText("Select cuboid with left click and control it with right click.", 400, 100, 20, GRAY);

            DrawFPS(10, 10);

            if (showDebug) 
            {
                for (int i = 0; i < NUMBER_OF_PLAYER_UNITS; i++) 
                {
                    if (playerUnits[i].targeting != NULL) DrawText(TextFormat("playerUnits[%d].targeting x = %f  \n\n  y= %f  \n\n  z = %f", i, playerUnits[i].targeting->x, playerUnits[i].targeting->y, playerUnits[i].targeting->z), 30, i * 100, 30, DARKGRAY);
                }
                // DrawText(TextFormat("ticker= %d", ticker), 30, 140, 30, LIGHTGRAY);
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context

    return 0;
}