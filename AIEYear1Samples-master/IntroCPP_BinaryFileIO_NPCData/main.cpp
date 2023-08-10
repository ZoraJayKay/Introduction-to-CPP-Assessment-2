/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "DataFile.h"
#include <iostream>

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    DataFile data;
    int currentRecordIdx = 0;
    const char* fileName = "npc_data.dat";

    // ZORA: This is the name of the binary data file holding NPC info
    data.Load(fileName);
    // ZORA: The Record to be retrieved as a result of left/right arrow keystrokes (starts on 1st NPC in the file)
    DataFile::Record* currentRecord = data.GetRecord(fileName, currentRecordIdx);
    // ZORA: The texture of the Record to be retrieved
    Texture2D recordTexture = LoadTextureFromImage(currentRecord->image);

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        if (IsKeyPressed(KEY_LEFT))
        {
            if (currentRecordIdx > 0)
            {
                currentRecordIdx--;
                UnloadTexture(recordTexture);
                currentRecord = data.GetRecord(fileName, currentRecordIdx);
                recordTexture = LoadTextureFromImage(currentRecord->image);
                
            }
            
            else {
                currentRecordIdx = 0;
            }
        }

        if (IsKeyPressed(KEY_RIGHT))
        {
            currentRecordIdx++;

            if (currentRecordIdx < data.GetRecordCount())
            {                
                UnloadTexture(recordTexture);
                currentRecord = data.GetRecord(fileName, currentRecordIdx);
                recordTexture = LoadTextureFromImage(currentRecord->image);
            }

            else {
                currentRecordIdx = data.GetRecordCount() - 1;
                currentRecord = data.GetRecord(fileName, currentRecordIdx);
            }
        }


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexture(recordTexture, 300, 50, WHITE);

        DrawText("NAME", 10, 50, 20, LIGHTGRAY);
        DrawText(currentRecord->name.c_str(), 10, 80, 20, LIGHTGRAY);

        DrawText("AGE", 10, 120, 20, LIGHTGRAY);
        DrawText(to_string(currentRecord->age).c_str(), 10, 150, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}