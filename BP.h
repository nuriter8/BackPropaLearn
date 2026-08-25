#ifndef _BP_NN_
#define _BP_NN_
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
using namespace std;

class Neuron
{

public:
    sf::CircleShape graphic;
    sf::Vector2f position;

    vector<double> weights;
    // graphic weights: each one is connected to each neuron from the previous layer
    vector<sf::VertexArray> graphic_weights;

    double sum;
    double bias;
    double output;
    double delta; // from back propagation

    // its attributes will be init. with default values
    Neuron(size_t size_prev_layer);
};

class Layer
{
public:
    vector<Neuron> neurons;

    Layer(size_t size_actual_layer, size_t size_prev_laver);
};

class BackPropagation
{
public:
    vector<Layer> layers;
    double learning_rate;

    double sigmoid(double n);
    double sigmoid_deriv(double n);
    double my_tanh(double x);
    double tanh_deriv(double y);
    double relu(double x);
    double relu_deriv(double x);

    BackPropagation(const vector<size_t> &architecture);
    vector<double> forward(const vector<double> &input, char function);
    void back(const vector<double> &input, const vector<double> output, char function);
    void clear_neurons();
    double train(const vector<vector<double>> &X, const vector<vector<double>> &Y, int epocas, char activ);
};

#endif