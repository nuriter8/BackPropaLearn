#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

using namespace std;

class NetworkVisualizer
{
private:
    sf::RenderWindow window;
    std::vector<sf::CircleShape> neurons;
    std::vector<sf::Vector2f> positions;

    const float NODE_RADIUS = 20.0f;
    const float SPACING_X = 120.0f;
    const float SPACING_Y = 70.0f;

    
    const float WINDOW_WIDTH = 1000.0f;
    const float WINDOW_HEIGHT = 600.0f;

    void setup_network(const std::vector<size_t> &architecture);
    void draw_nodes();

public:
    NetworkVisualizer(const std::vector<size_t> &architecture);
    void run();
    ~NetworkVisualizer();
};