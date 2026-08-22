#include "NetworkVisualizer.h"
#include <iostream>
#include <cstdlib>
using namespace std;

NetworkVisualizer::NetworkVisualizer(const std::vector<size_t> &architecture)
{
    sf::ContextSettings settings;

    settings.antialiasingLevel = 0;
    settings.majorVersion = 1;
    settings.minorVersion = 1;

    window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                  "Neural Network learner",
                  sf::Style::Default,
                  settings);

    if (!window.isOpen())
    {
        cerr << "couldn't open window" << endl;
        return;
    }
}

void NetworkVisualizer::run()
{
    cout << "running";

    while (window.isOpen())
    {

        sf::Event event;

        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
        }

        window.clear(sf::Color(30, 30, 30));

        window.display();

        sf::sleep(sf::milliseconds(16));
    }
}

NetworkVisualizer::~NetworkVisualizer()
{
}

void NetworkVisualizer::setup_network(const std::vector<size_t> &architecture)
{
    cout << "setting up";
}

void NetworkVisualizer::draw_nodes()
{
    cout << "drawing nodes";
}