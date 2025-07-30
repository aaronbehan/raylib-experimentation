#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <C:\raylib\raylib\src\raygui.h>

#define Q_NUM_FRAMES 2

int main()
{
    int screenWidth = 1400;
    int screenHeight = 850;
    InitWindow(screenWidth, screenHeight, "raygui - controls test suite");
    SetExitKey(-1);  // Disabling Raylib's default Esc key to close.

    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    // Button 
    Texture2D buttonQ = LoadTexture("resources/simplekeys/Q.png");
    float frameWidth = (float)buttonQ.width / Q_NUM_FRAMES;
    Rectangle sourceRec = { 0, 0, frameWidth, (float)buttonQ.height}; // defining how much of the sprite sheet we want to look at at one time. 
    Rectangle btnBounds = { screenWidth/2.0f - buttonQ.width/2.0f, screenHeight/2.0f - buttonQ.height/Q_NUM_FRAMES/2.0f, frameWidth, (float)buttonQ.height};


    Vector2 mousePos = {0.0f, 0.0f};
    int btnState = 0;

    int panelHeight = 100;

    SetTargetFPS(60);

    bool showMessageBox = false;

    while (!WindowShouldClose())
    {
        mousePos = GetMousePosition();

        if (IsKeyDown(KEY_Q) || (CheckCollisionPointRec(mousePos, btnBounds) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)))
        {
            btnState = 1;
        }
        else btnState = 0;

        sourceRec.x = btnState * frameWidth;

        // Draw
        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            BeginMode3D(camera);  // 3D elements must be rendered before the GUI elements, otherwise 3D elements will obscure the overlay

            DrawGrid(10, 1.0f);

            EndMode3D();

            DrawRectangle(0, screenHeight - panelHeight, screenWidth, panelHeight, BLUE);

            if ((IsKeyPressed(KEY_ESCAPE)) || (GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Show Message"))) showMessageBox = true;

            if (showMessageBox)
            {
                int result = GuiMessageBox((Rectangle){ 85, 70, 250, 100 },
                    "#191#Message Box", "Hi! This is a message!", "Nice;Cool");

                if (result >= 0) 
                {
                    CloseWindow();
                    return 0;  // necessary to avoid seg fault on below lines 
                }
            }

            DrawTextureRec(buttonQ, sourceRec, (Vector2){ btnBounds.x, btnBounds.y }, WHITE); // Draw button frame

        EndDrawing();
    }

    UnloadTexture(buttonQ);

    CloseWindow();
    return 0;
}
