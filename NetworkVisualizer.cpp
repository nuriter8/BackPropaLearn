#include "NetworkVisualizer.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <iomanip>
using namespace std;

NetworkVisualizer::NetworkVisualizer(BackPropagation &arch)
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

    architecture = &arch;

    setup_network();
    setup_panel();
}

void NetworkVisualizer::setup_panel()
{
    float panel_width = WINDOW_WIDTH * PANEL_WIDTH_RATIO;

    infoPanel.setSize(sf::Vector2f(panel_width, WINDOW_HEIGHT));
    infoPanel.setPosition(0.0f, 0.0f);
    infoPanel.setFillColor(sf::Color(171, 213, 255));


    /*
    
    sf::Font font;

    if (!font.loadFromFile("ARIALLGT.TTF"))
    {
        cout << "can't load font" << endl;
        return;
    }

    sf::Text weightText;
    weightText.setFont(font);
    weightText.setCharacterSize(8);
    weightText.setFillColor(sf::Color::Black);
    weightText.setStyle(sf::Text::Bold);



    */

    // sf::Font titleFont;
    titleFont.loadFromFile("ARIALLGT.TTF");

    // sf::Text title;
    title.setFont(titleFont);
    title.setCharacterSize(25);
    title.setFillColor(sf::Color::Blue);
    title.setStyle(sf::Text::Bold);
    title.setString("BackPropaLearn");
    title.setPosition(90.0f, 40.0f);


}
void NetworkVisualizer::draw_panel()
{
    window.draw(infoPanel);
    window.draw(title);
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

        window.clear(sf::Color::White);

        draw_connections();
        draw_nodes();

        draw_panel();

        window.display();

        sf::sleep(sf::milliseconds(50));
    }
}

NetworkVisualizer::~NetworkVisualizer()
{
}

void NetworkVisualizer::setup_network()
{
    cout << "setting up";

    architecture->clear_neurons();

    if (architecture->layers.empty())
    {
        return;
    }

    float panel_width = WINDOW_WIDTH * PANEL_WIDTH_RATIO;
    float diagram_x_start = panel_width;
    float diagram_width = WINDOW_WIDTH - panel_width;
    float heigh = WINDOW_HEIGHT;

    float total_width = (architecture->layers.size() - 1) * SPACING_X;
    float start_x_offset = diagram_x_start + (diagram_width - total_width) / 2.0f;

    for (int i = 0; i < architecture->layers.size(); i++)
    {
        vector<Neuron> &neurons_in_layer = architecture->layers[i].neurons;

        float center_y = heigh / 2.0f;
        float y_offset = (neurons_in_layer.size() - 1) * SPACING_Y / 2.0f;

        sf::Color layer_color;

        if (i == 0)
        {
            layer_color = sf::Color::Red;
        }
        else if (i == architecture->layers.size() - 1)
        {
            layer_color = sf::Color::Blue;
        }
        else
        {
            layer_color = sf::Color::Green;
        }

        for (size_t j = 0; j < neurons_in_layer.size(); j++)
        {
            float x = start_x_offset + i * SPACING_X;
            float y = center_y - y_offset + j * SPACING_Y;

            architecture->layers[i].neurons[j].graphic.setRadius(NODE_RADIUS);

            neurons_in_layer[j].graphic.setPosition(x - NODE_RADIUS, y - NODE_RADIUS);
            neurons_in_layer[j].graphic.setFillColor(layer_color);
            neurons_in_layer[j].graphic.setOutlineColor(sf::Color::Black);
            neurons_in_layer[j].graphic.setOutlineThickness(1.0f);

            neurons_in_layer[j].position = sf::Vector2f(x, y);
        }
    }
}

void NetworkVisualizer::draw_nodes()
{

    for (int i = 0; i < this->architecture->layers.size(); i++)
    {
        for (int j = 0; j < this->architecture->layers[i].neurons.size(); j++)
        {
            // cout << "drawing node " << j << " from layer " << i << endl;

            window.draw(this->architecture->layers[i].neurons[j].graphic);
        }
    }
}

void NetworkVisualizer::draw_connections()
{
    if (!architecture)
    {
        cout << "no architecture" << endl;
        return;
    }

    if (architecture->layers.size() < 2)
    {
        cout << "not enough layers" << endl;
        return;
    }

    cout << "drawing connections (not alg)" << endl;

    sf::Font font;

    if (!font.loadFromFile("arial.ttf"))
    {
        cout << "can't load font" << endl;
        return;
    }

    sf::Text weightText;
    weightText.setFont(font);
    weightText.setCharacterSize(8);
    weightText.setFillColor(sf::Color::Black);
    //weightText.setStyle(sf::Text::Bold);

    for (size_t i = 1; i < architecture->layers.size(); i++)
    {

        vector<Neuron> &current_layer = architecture->layers[i].neurons;
        vector<Neuron> &previous_layer = architecture->layers[i - 1].neurons;

        for (size_t j = 0; j < current_layer.size(); j++)
        {

            for (size_t k = 0; k < previous_layer.size(); k++)
            {
                // cout << "j " << j << " " << current_layer[j].weights.size() << " and k " << previous_layer.size() << endl;

                if (k < current_layer[j].weights.size())
                {
                    double weight = current_layer[j].weights[k];

                    // cout << "drawing cons from layer " << i << " node " << j << " w : " << weight << endl;

                    sf::Color color;

                    color = sf::Color::Black;

                    sf::Vertex line[] = {
                        sf::Vertex(previous_layer[k].position, color),
                        sf::Vertex(current_layer[j].position, color)};

                    window.draw(line, 2, sf::Lines);

                    sf::Vector2f posA = previous_layer[k].position;
                    sf::Vector2f posB = current_layer[j].position;

                    float t = 0.2f;

                    sf::Vector2f textPos;
                    textPos.x = posA.x + (posB.x - posA.x) * t;
                    textPos.y = posA.y + (posB.y - posA.y) * t;

                    // sf::Vector2f midpoint = (posA + posB) / 2.0f;

                    sf::Vector2f dir = posB - posA;

                    float angle = atan2(dir.y, dir.x);

                    float length = sqrt(dir.x * dir.x + dir.y * dir.y);

                    if (length > 0)
                    {
                        sf::Vector2f perp(-dir.y / length, dir.x / length);

                        float offset = 10.0f;
                        textPos += perp * offset;
                    }

                    std::ostringstream ss;
                    ss << fixed << setprecision(2) << weight;
                    weightText.setString(ss.str());

                    // weightText.setString(std::to_string(weight));

                    sf::FloatRect textBounds = weightText.getLocalBounds();
                    weightText.setOrigin(textBounds.left + textBounds.width / 2.0f,
                                         textBounds.top + textBounds.height / 2.0f);

                    weightText.setPosition(textPos);
                    weightText.setRotation(angle * 180.0f / 3.14159265f);

                    window.draw(weightText);
                }
            }
        }
    }
}