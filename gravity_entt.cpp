#include <raylib.h>
#include <iostream>
#include <cmath>

#include "entt/entt.hpp"

#define NUM_ENTITIES 10
#define GRAVITY 5.0f

// components
struct MyTag {};

struct Position
{
    double x, y;
};

struct Velocity
{
    double x, y;
};

// systems
class MoveSystem
{
public:
    void update(entt::registry &registry) {
        registry.view<Position, Velocity>().each([](Position &p, Velocity &v) {
                  p.x += v.x;
                  p.y += v.y; });
    }
};

class EdgesCollisionsSystem
{
public:
    void update(entt::registry &registry, int screenWidth, int screenHeight) {
        registry.view<Position, Velocity>().each([&](Position &p, Velocity &v) {
                if ( p.x < 0 ) v.x = -v.x;            
                if ( p.y < 0 ) v.y = -v.y;            
                if ( p.x > screenWidth ) v.x = -v.x;
                if ( p.y > screenHeight ) v.y = -v.y; 
            });
    }
};

class GravitySystem 
{
public:
    void update(entt::registry &registry) {
        registry.view<Position, Velocity>().each([&registry](auto e1, Position &p, Velocity &v) { 
                registry.view<Position>().each([&](auto e2, Position &p2) {
                        if ( e1 != e2 ) {
                            float distance = sqrt(( p.x - p2.x ) * ( p.x - p2.x) +
                                            (p.y - p2.y) * (p.y - p2.y));
                            // Apply gravitational force
                            float force = GRAVITY / (distance * distance);
                            v.x += force * (p2.x - p.x) ;
                            v.y += force * (p2.y - p.y) ; 
                        } 
                    });
            });
    }
};

class RenderSystem
{
    public:
    void render(entt::registry &registry) {
        registry.view<Position>().each([](auto entity, Position &p) {
                DrawCircle(p.x, p.y, 2, (Color){255, 121, 241, 255}); 
            });
    }
};

int main(int, char *[])
{
    const int screenWidth = 1000;
    const int screenHeight = 1000;

    MoveSystem moveSystem;
    EdgesCollisionsSystem edgesCollisionsSystem;
    GravitySystem gravitySystem;
    RenderSystem RenderSystem;

    entt::registry registry;

    // create a few entities
    const int numEntities = NUM_ENTITIES;
    for (int i = 0; i < numEntities; i++)
    {
        auto e = registry.create();
        auto &pos = registry.emplace<Position>(e, (float)GetRandomValue(0, screenWidth), (float)GetRandomValue(0, screenHeight));
        auto &vel = registry.emplace<Velocity>(e);
        vel.x = (float)GetRandomValue(-5, 5);
        vel.y = (float)GetRandomValue(-5, 5);
    }

    InitWindow(screenWidth, screenHeight, "EnTT - Raylib Example");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        moveSystem.update(registry);
        edgesCollisionsSystem.update(registry, screenWidth, screenHeight);    
        gravitySystem.update(registry);

        BeginDrawing();
        ClearBackground(BLACK);
        RenderSystem.render(registry);
        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
};
