#include "BP.h"
#include <iostream>
#include <cmath>
using namespace std;


// sudo apt-get install libsfml-dev

// default values
Neuron::Neuron(size_t size_prev_layer)
{
    // basic initialization
    this->bias = this->delta = this->output = this->sum = 0.0;

    this->bias = (((double)rand()) / RAND_MAX) - 0.5;

    for (double &weight : this->weights)
    {
        // XAVIER INITIALIZATION

        // fan_in = size_previous_layer and fan_out = 1 (simplification of xavier init.)
        double limit = sqrt(6.0 / (size_prev_layer + 1));
        weight = ((double)rand() / RAND_MAX) * 2 * weight - limit;

        cout << "initializing weigth as " << weight << endl;
    }
}

Layer::Layer(size_t size_actual_layer, size_t size_prev_laver)
{

    // this is a basic initialization, the vector(neurons) of the layer will have as much weights as the size of the previous layer
    for (int i = 0; i < size_actual_layer; i++)
    {
        // for each neuron of this layer, theres a vector of weights that has the size of the previous layer
        // each neuron from the previous layer connects with this neuron.
        // this->neurons.emplace_back(size_prev_laver);
        Neuron new_neuron(size_prev_laver);
        this->neurons.push_back(new_neuron);
        cout << "layer " << i << " initialized" << endl;
    }
    cout << endl;
}

double BackPropagation::sigmoid(double n)
{
    return (1.0 / (1.0 + (exp(n * -1))));
}
double BackPropagation::sigmoid_deriv(double n)
{
    return (n * (1.0 - n));
}
double BackPropagation::my_tanh(double x)
{
    return std::tanh(x);
}
double BackPropagation::tanh_deriv(double y)
{
    return 1.0 - y * y;
}
double BackPropagation::relu(double x)
{
    return x > 0 ? x : 0;
}
double BackPropagation::relu_derivada(double x)
{
    return x > 0 ? 1 : 0;
}

// the input is input_layer, hidden layers, output layer
BackPropagation::BackPropagation(const vector<size_t> &architecture)
{
    for (int i = 0; i < architecture.size(); i++)
    {
        Layer layer(architecture[i], architecture[i - 1]);
        this->layers.push_back(layer);
    }
}

vector<double> BackPropagation::forward(const vector<double> &input, char function)
{

    vector<double> modified = input;

    for (auto &layer : layers)
    {

        vector<double> modifications;

        for (auto &neuron : layer.neurons)
        {
            double sum = neuron.bias;

            for (size_t i = 0; i < modifications.size(); i++)
            {
                sum += neuron.weights[i] * modifications[i];
            }

            bool is_output = (&layer == &layers.back());

            double value = 0.0;
            if (function == 'r')
            {
                value = relu(sum);
            }
            else if (function == 't')
            {
                value = my_tanh(sum);
            }
            else if (function == 's')
            {
                value = sigmoid(sum);
            }
            else
            {
                value = sum;
            }

            if (is_output)
            {
                neuron.output = sum;
            }
            else
            {
                neuron.output = value;
            }

            modifications.push_back(neuron.output);
        }

        modified = modifications;
    }
    return modified;
}
void BackPropagation::back(const vector<double> &input, const vector<double> target, char function)
{
    Layer &output_layer = layers.back();
    // output is last layer of all the layers we've got

    for (size_t i = 0; i < output_layer.neurons.size(); i++){

        double current_neuron_output = output_layer.neurons[i].output;
        output_layer.neurons[i].delta = current_neuron_output - target[i];
    }

    // go backwards

    for (size_t i = layers.size() - 2; i >= 0; i --){
        // each layer


        for(size_t j = 0; j < layers[i].neurons.size(); i++){
            // all the neurons from each layer

            double sum = 0.0;

            for(auto &nextlayer_neuron : layers[i+1].neurons){

                sum += nextlayer_neuron.weights[i] * nextlayer_neuron.delta;
                // our current neuron set is connected to the next set of neurons
            }
        }
    }


}
