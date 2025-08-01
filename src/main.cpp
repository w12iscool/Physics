#include "raylib.h"
#include "GameEngine.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

GameEngine game;

int main()
{
    InitWindow(800, 600, "Physics");
    SetTargetFPS(60);


    game.startUp();
    while (!WindowShouldClose())
    {
        game.update();
        BeginDrawing();
        game.render();
        EndDrawing();
    }
    game.shutDown();
    CloseWindow();
}
