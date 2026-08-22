#ifndef _BP_NN_
#define _BP_NN_
#include <iostream>
#include <vector>
using namespace std;

class Neuron
{

public:
    vector<double> weights;
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
    double relu_derivada(double x);

    BackPropagation(const vector<size_t> &architecture);
    vector<double> forward(const vector<double> &input, char function);
    void back(const vector<double> &input, const vector<double> output, char function);
    
};

#endif