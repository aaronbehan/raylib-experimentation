#include "raylib.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [audio] example - music playing (streaming)");

    InitAudioDevice();                      // Initialize audio device
    SetAudioStreamBufferSizeDefault(4096);  // This fixes some unpleasant crackling/popping 

    Music highHats = LoadMusicStream("resources/High_Hats.wav");
    Music drumMachine = LoadMusicStream("resources/Drum_Machine.wav");

    PlayMusicStream(highHats);
    PlayMusicStream(drumMachine);

    float volumeHighHats = 1.0;  // Max volume

    SetTargetFPS(30);               // Set our game to run at 30 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateMusicStream(highHats);   // Update music buffer with new stream data
        UpdateMusicStream(drumMachine);   // Update music buffer with new stream data

        SetMusicVolume(highHats, volumeHighHats);

        if (IsKeyDown(KEY_DOWN)) volumeHighHats -= 0.01f;
        else if (IsKeyDown(KEY_UP)) volumeHighHats += 0.01f;

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("MUSIC SHOULD BE PLAYING!", 255, 150, 20, LIGHTGRAY);
            DrawText(TextFormat("High hats volume: %f", volumeHighHats), 255, 180, 20, LIGHTGRAY);
            DrawText(TextFormat("Press UP/DOWN for high hats volume control"), 255, 210, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadMusicStream(drumMachine);   // Unload music stream buffers from RAM
    UnloadMusicStream(highHats);   // Unload music stream buffers from RAM

    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
