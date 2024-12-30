#include "raylib.h"
#include "raymath.h"
#include <stddef.h>  // for NULL definition
#include "timer.h"

int main ()
{
	
	// set up the window
	InitWindow(1280, 800, "Raylib Timer Example");
	SetTargetFPS(40);
	
    InitAudioDevice();
    Sound sound = LoadSound("stringpluck.wav");

    float second = 1.0f;  // very close to 1 second 

    // Formula     x = 60 / BPM
    // 60 BPM = 1.0
    // 30 BPM = 2.0
    // 120 BPM = 0.5
    
    float beatPerMin = 100;  // some variables such as 88 are slightly off but i think rounder floats tend to behave as expected
	
    float beatPerSec = 60 / beatPerMin;

    // a timer for the circle
    Timer timer = { 0 };

    int ticker = 0;
    bool pause = true;

	// Game loop
	while (!WindowShouldClose())
	{
        if (IsKeyPressed(KEY_SPACE)) 
        {
            StartTimer(&timer, beatPerSec);
            pause = false;
            PlaySound(sound);
        }

        UpdateTimer(&timer);  // with every single cycle, timer->Lifetime is deducted by GetFrameTime as long as it is greater than 0

		// Drawing
		BeginDrawing();
		ClearBackground(BLACK);

        // Draw a red circle if the timer lifetime is greater than 0
        if (!TimerDone(&timer) || pause) 
        {
            DrawCircleV((Vector2){400, 400}, 50, RED);
        }
        else // draw a blue circle if timer lifetime is less than 0
        {
            DrawCircleV((Vector2){400, 400}, 50, BLUE);  
            StartTimer(&timer, beatPerSec);
            ticker++;
            PlaySound(sound);
        }
        
        // debugging stats
        DrawText(TextFormat("timer.Lifetime = %f", timer.Lifetime), 20, 20, 15, WHITE);
        DrawText(TextFormat("TimerDone() = %d", TimerDone(&timer)), 20, 35, 15, WHITE);
        DrawText(TextFormat("GetFrameTime*() = %f", GetFrameTime()), 20, 65, 15, WHITE);
        DrawText(TextFormat("ticker = %d", ticker), 20, 80, 15, WHITE);

        DrawText(TextFormat("GetTime() = %f\n(unused variable)", GetTime()), 20, 200, 15, WHITE);
        DrawText(TextFormat("BeatPerMin = %f,\n BeatPerSec = %f", beatPerMin, beatPerSec), 20, 240, 15, WHITE);

		EndDrawing();
	}

	CloseWindow();
    CloseAudioDevice();
	return 0;
}
