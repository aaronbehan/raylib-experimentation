#include "stdio.h"
#include "stdlib.h"

#include "raylib.h"
#include "header.h"


int return_six() 
{
    int variable = 6666;

    return variable;
}

void UpdateTitleScreen(void)
{

    if (IsKeyDown(KEY_Q))
    {
        currentScreen = GAMEPLAY;
    }

    BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText(TextFormat("Press Q to change game state to GAMEPLAY ==== %d", 13), 20, 35, 15, BLACK);
        
        DrawFPS(10, 10);

    EndDrawing();
}