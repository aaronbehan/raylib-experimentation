#include "raylib.h"
#include "structs.h"
#include <stdlib.h>

Unit CreateEnemyUnit(void) 
{
    Unit enemy = { 0 };

    enemy.unitPos = (Vector3){GetRandomValue(-5, 5), 0.5, GetRandomValue(-5, 5)};  // Unit overlap currently possible
    enemy.unitSize = (Vector3){1.0f, 1.0f, 1.0f};
    enemy.maxHealth = 50;
    enemy.currentHealth = enemy.maxHealth;
    enemy.colour = ORANGE;

    // other variables are currently irrelevant or correctly set to 0

    return enemy;
}

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "raylib example - core world screen");

    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    Unit playerUnit = { { 0.0f, 1.0f, 0.0f }, { 0 }, { 1.0f, 2.0f, 1.0f }, { 0 }, 0.05f, 40, 40, 3, 1, false, false, false, RED};
    Vector3 attackBox = { 0 };
    Vector3 attackBoxSize = { 0.5f, 0.5f, 0.5f };

    // This will be highly variable, hence the need for the heap instead of fixed arrays. 
    int numberOfEnemies = 4;  // KEEP THIS UPDATED

    Unit *enemies = (Unit*)malloc(numberOfEnemies * sizeof(Unit));  // could it be friendlier to lower RAM machines to use pointers to Units here instead of actual Units here? update: reddit consensus is that it makes no difference

    for (int i = 0; i < numberOfEnemies; i++) 
    {
        enemies[i] = CreateEnemyUnit();
    }

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        if (IsKeyDown(KEY_RIGHT)) playerUnit.unitPos.x += 0.1;
        if (IsKeyDown(KEY_LEFT)) playerUnit.unitPos.x -= 0.1;
        if (IsKeyDown(KEY_DOWN)) playerUnit.unitPos.z += 0.1;
        if (IsKeyDown(KEY_UP)) playerUnit.unitPos.z -= 0.1;

        if (IsKeyDown(KEY_SPACE)) // attacking prototype 
        {
            attackBox.x = playerUnit.unitPos.x;
            attackBox.y = playerUnit.unitPos.y;
            attackBox.z = playerUnit.unitPos.z + 1;  // only one direction attack so far
        }
        if (IsKeyReleased(KEY_SPACE)) 
        {
            attackBox.x = 0.0f;
            attackBox.y = 10.0f;
            attackBox.z = 0.0f;
        }

        // check collisions for each enemy against attack box
        for (int i = 0; i < numberOfEnemies; i++) 
        {
            if (!enemies[i].dead) 
            {
                if (CheckCollisionBoxes(
                (BoundingBox){(Vector3){ attackBox.x - attackBoxSize.x/2,
                                        attackBox.y - attackBoxSize.y/2,
                                        attackBox.z - attackBoxSize.z/2 },
                            (Vector3){ attackBox.x + attackBoxSize.x/2,
                                        attackBox.y + attackBoxSize.y/2,
                                        attackBox.z + attackBoxSize.z/2 }},
                (BoundingBox){(Vector3){enemies[i].unitPos.x - enemies[i].unitSize.x/2,
                                        enemies[i].unitPos.y - enemies[i].unitSize.y/2,
                                        enemies[i].unitPos.z - enemies[i].unitSize.z/2 },
                            (Vector3){ enemies[i].unitPos.x + enemies[i].unitSize.x/2,
                                        enemies[i].unitPos.y + enemies[i].unitSize.y/2,
                                        enemies[i].unitPos.z + enemies[i].unitSize.z/2 }})) 
                    {
                        enemies[i].currentHealth -= 1;
                        if (enemies[i].currentHealth <= 0) enemies[i].dead = true;
                    }
            }
        }

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                // Draw player
                DrawCube(playerUnit.unitPos, playerUnit.unitSize.x, playerUnit.unitSize.y, playerUnit.unitSize.z, playerUnit.colour);
                DrawCubeWires(playerUnit.unitPos, playerUnit.unitSize.x, playerUnit.unitSize.y, playerUnit.unitSize.z, BLACK);
                // Draw attack box
                DrawCube(attackBox, 0.5f, 0.5f, 0.5f, BLUE);
                DrawCubeWires(attackBox, 0.5f, 0.5f, 0.5f, BLACK);
                
                for (int i = 0; i < numberOfEnemies; i++) 
                {
                    if (!enemies[i].dead) 
                    {
                        // Calculate the 2D screen coordinates of the 3D object for accurate text drawing
                        enemies[i].screenPos = GetWorldToScreen((Vector3){enemies[i].unitPos.x, enemies[i].unitPos.y + 1.0f, enemies[i].unitPos.z}, camera);
                        
                        DrawCube(enemies[i].unitPos, enemies[i].unitSize.x, enemies[i].unitSize.y, enemies[i].unitSize.z, enemies[i].colour);
                        DrawCubeWires(enemies[i].unitPos, enemies[i].unitSize.x, enemies[i].unitSize.y, enemies[i].unitSize.z, BLACK);
                    }
                    else  // only draw cube wires if unit is dead
                    {
                        DrawCubeWires(enemies[i].unitPos, enemies[i].unitSize.x, enemies[i].unitSize.y, enemies[i].unitSize.z, BLACK);
                    }
                }

                DrawGrid(10, 1.0f);

            EndMode3D();

            for (int i = 0; i < numberOfEnemies; i++) 
            {
                if (!enemies[i].dead) 
                {
                    // Draw text above non-dead enemies
                    DrawText(TextFormat("Health: %d/%d", enemies[i].currentHealth, enemies[i].maxHealth), (int)enemies[i].screenPos.x - MeasureText("Health: -----", 20)/2, (int)enemies[i].screenPos.y, 20, BLACK);
                }
                else 
                {
                    DrawText(TextFormat("Dead", enemies[i].currentHealth, enemies[i].maxHealth), (int)enemies[i].screenPos.x - MeasureText("Dead", 20)/2, (int)enemies[i].screenPos.y, 20, BLACK);
                }
            }

            DrawFPS(10, 10);

        EndDrawing();
    }

    free(enemies);
    CloseWindow();  // Close window and OpenGL context

    return 0;
}
