#include "raylib.h"
#include "raymath.h"

// https://bedroomcoders.co.uk/posts/198  potentially useful read regarding billboards

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "raylib [models] example - drawing billboards");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 5.0f, 4.0f, 5.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Texture2D appleAtlas = LoadTexture("resources/apple_atlas.png");    // Our billboard texture
    Vector3 billPositionStatic = { 0.0f, 2.0f, 0.0f };          // Position of the static billboard
    Vector3 billPositionRotating = { 1.0f, 2.0f, 1.0f };        // Position of the revolving billboard

    Rectangle source = (Rectangle){0, 0, 60, 60};  // taking just a tiny segment from the apple_atlas png

    // NOTE: Billboard locked on axis-Y
    Vector3 billUp = { 0.0f, 1.0f, 0.0f };

    // Rotate around origin
    // Here we choose to rotate around the image center
    // NOTE: (-1, 1) is the range where origin.x, origin.y is inside the texture
    Vector2 rotateOrigin = { 0.0f };

    // Distance is needed for the correct billboard draw order
    // Larger distance (further away from the camera) should be drawn prior to smaller distance.
    float distanceStatic;
    float distanceRotating;
    float rotation = 0.0f;

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second

    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        rotation += 0.4f;
        distanceStatic = Vector3Distance(camera.position, billPositionStatic);
        distanceRotating = Vector3Distance(camera.position, billPositionRotating);

        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawGrid(10, 1.0f);        // Draw a grid

                // Draw order matters!
                if (distanceStatic > distanceRotating) 
                {
                    //                       texture     source  position            up      size                    origin        rotation  color
                    DrawBillboardPro(camera, appleAtlas, source, billPositionStatic, billUp, (Vector2) {1.0f, 1.0f}, rotateOrigin, 0.0f, WHITE);  // STATIC SECOND
                    DrawBillboardPro(camera, appleAtlas, source, billPositionRotating, billUp, (Vector2) {1.0f, 1.0f}, rotateOrigin, rotation, WHITE);  // MOVING SECOND
                } 
                else
                {
                    DrawBillboardPro(camera, appleAtlas, source, billPositionRotating, billUp, (Vector2) {1.0f, 1.0f}, rotateOrigin, rotation, WHITE);  // MOVING FIRST
                    DrawBillboardPro(camera, appleAtlas, source, billPositionStatic, billUp, (Vector2) {1.0f, 1.0f}, rotateOrigin, 0.0f, WHITE);  // STATIC SECOND
                }
                
            EndMode3D();

            DrawFPS(10, 10);

        EndDrawing();
    }

    // De-Initialization
    UnloadTexture(appleAtlas);        // Unload texture

    CloseWindow();              // Close window and OpenGL context

    return 0;
}
