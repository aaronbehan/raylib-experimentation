// note to self: this was an attempt to use game states in raylib

#include "stdlib.h"
#include "include/raylib.h"
#include "header.h"


// Global variables
GameScreen currentScreen = TITLE;


// Forward declarations
static void UpdateDrawFrame(void);


int main(void)
{
    const int screenWidth = 1400;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "Game state experimentation");

    SetTargetFPS(60);

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}


static void UpdateDrawFrame(void)
{
    switch(currentScreen)
    {
        case GAMEPLAY:
        {
            UpdateGameplayScreen();
        } break;
        case TITLE:
        {
            UpdateTitleScreen();
        } break;
    }
}
