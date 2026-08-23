#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "BP.h"

using namespace std;

class NetworkVisualizer
{
private:
    sf::RenderWindow window;
    //std::vector<sf::CircleShape> neurons;
    //std::vector<sf::Vector2f> positions;

    const float NODE_RADIUS = 20.0f;
    const float SPACING_X = 250.0f;
    const float SPACING_Y = 120.0f;

    
    const float WINDOW_WIDTH = 1600.0f;
    const float WINDOW_HEIGHT = 800.0f;


    BackPropagation *architecture;


    void setup_network();
    void draw_nodes();
    void draw_connections();

public:
    NetworkVisualizer(BackPropagation &arch);
    void run();
    ~NetworkVisualizer();
};