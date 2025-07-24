#include "raylib.h"

#include "C:\raylib\raylib\src\rlgl.h"       // Required to define vertex data (immediate-mode style)

// Custom Functions Declaration
//------------------------------------------------------------------------------------
void DrawCubeSidesTextureRec(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color); // Draw cube with a region of a texture
void DrawCubeTopTextureRec(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color);

// Program main entry point
int main(void)
{

    const int screenWidth = 1400;
    const int screenHeight = 850;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - draw cube texture");

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    // Load texture to be applied to the cubes sides
    Texture2D texture = LoadTexture("resources/cubicmap_atlas.png");

    SetTargetFPS(60); 

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                for (int i = 0; i < 5; i++) 
                {
                    // Draw cube with an applied texture, but only a defined rectangle piece of the texture
                    DrawCubeSidesTextureRec(texture, (Rectangle){ 0.0f, texture.height/2.0f, texture.width/2.0f, texture.height/2.0f }, 
                        (Vector3){ (float)i, 1.0f, 0.5f }, 1.0f, 1.0f, 1.0f, WHITE);
                    
                    DrawCubeTopTextureRec(texture, (Rectangle){texture.width/2.0f, texture.height/2.0f, texture.width/2.0f, texture.height/2.0f},
                        (Vector3){ (float)i, 1.0f, 0.5f }, 1.0f, 1.0f, 1.0f, WHITE);
                }

                DrawGrid(10, 1.0f);  // Draw a grid

            EndMode3D();

            DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    UnloadTexture(texture); // Unload texture
    
    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


// Draw cube with texture piece applied to all faces (Note: commented out top, back and bottom as they are not necessary)
void DrawCubeSidesTextureRec(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;
    float texWidth = (float)texture.width;
    float texHeight = (float)texture.height;

    // Set desired texture to be enabled while drawing following vertex data
    rlSetTexture(texture.id);

    // We calculate the normalized texture coordinates for the desired texture-source-rectangle
    // It means converting from (tex.width, tex.height) coordinates to [0.0f, 1.0f] equivalent 
    rlBegin(RL_QUADS);
        rlColor4ub(color.r, color.g, color.b, color.a);

        // Front face
        rlNormal3f(0.0f, 0.0f, 1.0f);
        rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x - width/2, y - height/2, z + length/2);
        rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x + width/2, y - height/2, z + length/2);
        rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
        rlVertex3f(x + width/2, y + height/2, z + length/2);
        rlTexCoord2f(source.x/texWidth, source.y/texHeight);
        rlVertex3f(x - width/2, y + height/2, z + length/2);

        // // Back face
        // rlNormal3f(0.0f, 0.0f, - 1.0f);
        // rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
        // rlVertex3f(x - width/2, y - height/2, z - length/2);
        // rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
        // rlVertex3f(x - width/2, y + height/2, z - length/2);
        // rlTexCoord2f(source.x/texWidth, source.y/texHeight);
        // rlVertex3f(x + width/2, y + height/2, z - length/2);
        // rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
        // rlVertex3f(x + width/2, y - height/2, z - length/2);

        // // Top face
        // rlNormal3f(0.0f, 1.0f, 0.0f);
        // rlTexCoord2f(source.x/texWidth, source.y/texHeight);
        // rlVertex3f(x - width/2, y + height/2, z - length/2);
        // rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
        // rlVertex3f(x - width/2, y + height/2, z + length/2);
        // rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
        // rlVertex3f(x + width/2, y + height/2, z + length/2);
        // rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
        // rlVertex3f(x + width/2, y + height/2, z - length/2);

        // // Bottom face
        // rlNormal3f(0.0f, - 1.0f, 0.0f);
        // rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
        // rlVertex3f(x - width/2, y - height/2, z - length/2);
        // rlTexCoord2f(source.x/texWidth, source.y/texHeight);
        // rlVertex3f(x + width/2, y - height/2, z - length/2);
        // rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
        // rlVertex3f(x + width/2, y - height/2, z + length/2);
        // rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
        // rlVertex3f(x - width/2, y - height/2, z + length/2);

        // Right face
        rlNormal3f(1.0f, 0.0f, 0.0f);
        rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x + width/2, y - height/2, z - length/2);
        rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
        rlVertex3f(x + width/2, y + height/2, z - length/2);
        rlTexCoord2f(source.x/texWidth, source.y/texHeight);
        rlVertex3f(x + width/2, y + height/2, z + length/2);
        rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x + width/2, y - height/2, z + length/2);

        // Left face
        rlNormal3f( - 1.0f, 0.0f, 0.0f);
        rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x - width/2, y - height/2, z - length/2);
        rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x - width/2, y - height/2, z + length/2);
        rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
        rlVertex3f(x - width/2, y + height/2, z + length/2);
        rlTexCoord2f(source.x/texWidth, source.y/texHeight);
        rlVertex3f(x - width/2, y + height/2, z - length/2);

    rlEnd();

    rlSetTexture(0);
}

// Draw cube with texture piece applied only to top
void DrawCubeTopTextureRec(Texture2D texture, Rectangle source, Vector3 position, float width, float height, float length, Color color)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;
    float texWidth = (float)texture.width;
    float texHeight = (float)texture.height;

    // Set desired texture to be enabled while drawing following vertex data
    rlSetTexture(texture.id);

    // We calculate the normalized texture coordinates for the desired texture-source-rectangle
    // It means converting from (tex.width, tex.height) coordinates to [0.0f, 1.0f] equivalent 
    rlBegin(RL_QUADS);
        rlColor4ub(color.r, color.g, color.b, color.a);

        // Top face
        rlNormal3f(0.0f, 1.0f, 0.0f);
        rlTexCoord2f(source.x/texWidth, source.y/texHeight);
        rlVertex3f(x - width/2, y + height/2, z - length/2);
        rlTexCoord2f(source.x/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x - width/2, y + height/2, z + length/2);
        rlTexCoord2f((source.x + source.width)/texWidth, (source.y + source.height)/texHeight);
        rlVertex3f(x + width/2, y + height/2, z + length/2);
        rlTexCoord2f((source.x + source.width)/texWidth, source.y/texHeight);
        rlVertex3f(x + width/2, y + height/2, z - length/2);

    rlEnd();

    rlSetTexture(0);
}
