#include <raylib.h>
#include <iostream>
#include <cmath>

#include "entt/entt.hpp"

struct MyTag {
};

struct Position {
    double x;
    double y;
};

struct Velocity {
    double x;
    double y;
};

int main(int, char* []) 
{
    const int screenWidth = 1000;
    const int screenHeight = 1000;

    entt::registry registry;
    
    // create a few entities
    const int numEntities = 5;
    for (int i = 0; i < numEntities; i++)
    {
        auto e = registry.create();
        auto &pos = registry.emplace<Position>(e, (float)GetRandomValue(0, screenWidth), (float)GetRandomValue(0, screenHeight));
        auto &vel = registry.emplace<Velocity>(e);
        vel.x = (float)GetRandomValue(-5, 5);
        vel.y = (float)GetRandomValue(-5, 5);
    }

    InitWindow(screenWidth, screenHeight, "Flecs - Raylib Example");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // move system
        registry.view<Position, Velocity>().each([](Position &p, Velocity &v)
              {
                  p.x += v.x;
                  p.y += v.y; });

        // edges collisions system
        registry.view<Position, Velocity>().each([](Position &p, Velocity &v)
              {
                if ( p.x < 0 ) v.x = -v.x;            
                if ( p.y < 0 ) v.y = -v.y;            
                if ( p.x > screenWidth ) v.x = -v.x;
                if ( p.y > screenHeight ) v.y = -v.y; });

        BeginDrawing();
        ClearBackground(BLACK);
        // render system
        registry.view<Position>().each([](auto entity, Position &p)
                    { DrawCircle(p.x, p.y, 2, (Color){255, 121, 241, 255}); });
        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
};


   