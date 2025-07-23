#include <raylib.h>

#define RAYGUI_IMPLEMENTATION
#include <C:\raylib\raylib\src\raygui.h>

#define BTN_NUM_FRAMES 3

int main()
{
    int screenWidth = 1400;
    int screenHeight = 850;
    InitWindow(screenWidth, screenHeight, "raygui - controls test suite");

    Camera camera = { { 0.0f, 10.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 45.0f, 0 };

    // Button 
    Texture2D button1 = LoadTexture("resources/button.png");
    // Define frame rectangle for drawing
    float frameHeight = (float)button1.height / BTN_NUM_FRAMES;
    Rectangle sourceRec = { 0, 0, (float)button1.width, frameHeight };
    // Define button bounds on screen
    Rectangle btnBounds = { screenWidth/2.0f - button1.width/2.0f, screenHeight/2.0f - button1.height/BTN_NUM_FRAMES/2.0f, (float)button1.width, frameHeight };

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
            btnState = 2;
        }
        else btnState = 1;

        sourceRec.y = btnState * frameHeight;

        // Draw
        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            BeginMode3D(camera);  // 3D elements must be rendered before the GUI elements, otherwise 3D elements will obscure the overlay

            DrawGrid(10, 1.0f);

            EndMode3D();

            DrawRectangle(0, screenHeight - panelHeight, screenWidth, panelHeight, BLUE);

            if (GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Show Message")) showMessageBox = true;

            if (showMessageBox)
            {
                int result = GuiMessageBox((Rectangle){ 85, 70, 250, 100 },
                    "#191#Message Box", "Hi! This is a message!", "Nice;Cool");

                if (result >= 0) showMessageBox = false;
            }

            DrawTextureRec(button1, sourceRec, (Vector2){ btnBounds.x, btnBounds.y }, WHITE); // Draw button frame

        EndDrawing();
    }

    UnloadTexture(button1);

    CloseWindow();
    return 0;
}
