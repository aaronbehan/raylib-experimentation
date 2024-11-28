#include "raylib.h"
#include "raymath.h"
#include <stddef.h>  // for NULL definition
#include "timer.h"

int main ()
{
	// set up the window
	InitWindow(1280, 800, "Raylib Timer Example");
	SetTargetFPS(40);

    float duration = 1.0f;  // roughly 1 second 

    // a timer for the circle
    Timer timer = { 0 };

    int ticker = 0;

	// Game loop
	while (!WindowShouldClose())
	{

        UpdateTimer(&timer);  // with every single cycle, timer->Lifetime is deducted by GetFrameTime as long as it is greater than 0

		    // Drawing
		    BeginDrawing();
		    ClearBackground(BLACK);

        // Draw a red circle if the timer is alive
        if (!TimerDone(&timer)) 
        {
            DrawCircleV((Vector2){400, 400}, 50, RED);
        }
        else 
        {
            DrawCircleV((Vector2){400, 400}, 50, BLUE);  
            StartTimer(&timer, duration);
            ticker++;
        }
        
        DrawText(TextFormat("timer.Lifetime = %f", timer.Lifetime), 20, 20, 15, WHITE);
        DrawText(TextFormat("TimerDone() = %d", TimerDone(&timer)), 20, 35, 15, WHITE);
        DrawText(TextFormat("GetFrameTime*() = %f", GetFrameTime()), 20, 65, 15, WHITE);
        DrawText(TextFormat("ticker = %d", ticker), 20, 80, 15, WHITE);

        DrawText(TextFormat("GetTime() = %f", GetTime()), 20, 200, 15, WHITE);  // Does not feature in code

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
