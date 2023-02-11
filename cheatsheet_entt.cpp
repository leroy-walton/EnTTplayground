#include <iostream>
#include "entt/entt.hpp"

struct MyTag{};

struct Position
{
    double x, y;
};

struct Velocity
{
    double x, y;
};

int main() {

    entt::registry registry;

    auto entity = registry.create();    
    registry.emplace<Position>(entity, 0., 0.);
    
    auto &velo = registry.emplace<Velocity>(entity);
    velo.x = 3.14159265358;
    velo.y = 4.2;
    
    auto &position = registry.get<Position>(entity);
    position.x = 10.5;
    position.y = 20.5;

    const auto [pos, vel] = registry.get<Position, Velocity>(entity);
    std::cout << "Position :" << pos.x << " " << pos.y << "\n";
    std::cout << "Velocity : " << vel.x << " " << vel.y << "\n";

    registry.emplace<MyTag>(entity);
    // Erases all instances of the given components from the entities that own them:
    registry.clear<MyTag>();

    auto view = registry.view<Position>();
    // iltering entities by components is also supported:
    auto view2 = registry.view<Position, Velocity>(entt::exclude<MyTag>);

    for(auto entity: view) {
    // a component at a time ...
        auto &position = view.get<Position>(entity);
        auto &velocity = view.get<Velocity>(entity);

    // ... multiple components ...
        auto [pos, vel] = view.get<Position, Velocity>(entity);
    }

    // through a callback
    registry.view<Position, Velocity>().each([](auto entity, auto &pos, auto &vel) {
        // ...
    });

    // using an input iterator
    for(auto &&[entity, pos, vel]: registry.view<Position, Velocity>().each()) {
        // ...
    }

    registry.destroy(entity);
    // Or destroy all entities in a registry at once:
    registry.clear(); 
}