#include <math.h>
#include "raylib.h"

typedef struct Timer {
    float Lifetime;
}Timer;

// start or restart a timer with a specific lifetime
void StartTimer(Timer* timer, float duration)
{
    timer->Lifetime = duration;  
}

// update a timer with the current frame time
void UpdateTimer(Timer* timer)
{
    timer->Lifetime += GetFrameTime();
}


int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    Vector2 startPoint = { 50, 180 };
    Vector2 endPoint = { 180, 180 };

    // controls the speed of the animations
    int speed = 2;

    // Timer
    Timer universalTimer = { 0 };
    StartTimer(&universalTimer, 0);

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateTimer(&universalTimer);

        BeginDrawing();

            ClearBackground(RAYWHITE);


            // Pulsating circle
            int size = 60; // to give the pulsating circle a base size to sort of aim for
            DrawCircle(300, 150, (size + cos(universalTimer.Lifetime * speed) * 30), RED);


            // Hovering circle 
            float approximateYCoord = 100; // this is to make positioning the circle easier, otherwise it will just hover around the top of the screen
                                           // the cos function is just there to make it fluctuate 
            DrawCircle(150, (approximateYCoord + (10 * cos(universalTimer.Lifetime * speed) * 3)), 30, PURPLE);


            // Orbiting circle
            // the below values: 450 and 350 are also used to position the circle, with the cos and sin modifiers being there to slightly alter that main variable
            int orbitDistance = 30;
            DrawCircle((450 + (orbitDistance * cos(universalTimer.Lifetime * speed))), 
                       (350 + (orbitDistance * sin(universalTimer.Lifetime * speed))), 
                        30, ORANGE);


            DrawText(TextFormat("universalTimer = %f", universalTimer.Lifetime), 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
