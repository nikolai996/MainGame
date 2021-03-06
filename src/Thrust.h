#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

struct Thrust {
public:
    Clock clock;
    float frameCounter;
    Texture texture1;
    Sprite thrust;
    Thrust();
    void Draw(RenderWindow & window, Vector2f & position, float & rotation);
};