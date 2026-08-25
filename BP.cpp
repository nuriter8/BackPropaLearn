#include "BP.h"
#include <iostream>
#include <cmath>
#include <random>
using namespace std;

// sudo apt-get install libsfml-dev

// default values
Neuron::Neuron(size_t size_prev_layer)
{
    // basic initialization
    this->bias = this->delta = this->output = this->sum = 0.0;

    this->bias = (((double)rand()) / RAND_MAX) - 0.5;

    this->weights.resize(size_prev_layer);

    for (double &weight : this->weights)
    {
        // XAVIER INITIALIZATION

        // fan_in = size_previous_layer and fan_out = 1 (simplification of xavier init.)
        double limit = sqrt(6.0 / (size_prev_layer + 1));
        weight = ((double)rand() / RAND_MAX) * 2 * weight - limit;

        // cout << "initializing w as " << weight << endl;
    }

    graphic.setRadius(20.0f);
    graphic.setFillColor(sf::Color(100, 100, 100));
    graphic.setOutlineColor(sf::Color::White);
    graphic.setOutlineThickness(2.0f);
    position = sf::Vector2f(0, 0);
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
        cout << "current size of layer " << i << " initialized" << endl;
    }
    cout << endl;
}

void BackPropagation::clear_neurons()
{
    for (auto &layer : layers)
    {
        for (auto &neuron : layer.neurons)
        {
            neuron.output = 0.0;
            neuron.delta = 0.0;
            neuron.sum = 0.0;
        }
    }
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
double BackPropagation::relu_deriv(double x)
{
    return x > 0 ? 1 : 0;
}

// the input is input_layer, hidden layers, output layer
BackPropagation::BackPropagation(const vector<size_t> &architecture)
{
    for (int i = 0; i < architecture.size(); i++)
    {

        cout << endl
             << "CURRENT LAYER NUMBER: " << i << endl;
        Layer layer(architecture[i], architecture[i - 1]);
        this->layers.push_back(layer);
    }

    cout << "everything initialized " << endl;
}

vector<double> BackPropagation::forward(const vector<double> &input, char function)
{

    // cout << "in forward" << endl;

    vector<double> modified = input;

    for (int i = 0; i < layers.size(); i++)
    {

        // cout << endl << "for layer " << i << endl;

        vector<double> modifications;

        for (int j = 0; j < layers[i].neurons.size(); j++)
        {

            // cout << endl << "in neuron " << j << endl;

            double sum = layers[i].neurons[j].bias;

            for (size_t k = 0; k < modifications.size(); k++)
            {
                sum += layers[i].neurons[j].weights[k] * modifications[i];
            }

            bool is_output = (&layers[i] == &layers.back());

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
                layers[i].neurons[j].output = sum;
            }
            else
            {
                layers[i].neurons[j].output = value;
            }

            // cout << "modif : " << layers[i].neurons[j].bias;

            modifications.push_back(layers[i].neurons[j].output);
        }

        modified = modifications;
    }
    return modified;
}
void BackPropagation::back(const vector<double> &input, const vector<double> target, char func)
{
    // cout << endl << "BACKPROPAGATION" << endl;
    Layer &outputLayer = this->layers.back(); // outputLayer.neurons.size() IS THE SIZE OF THE OUTPUT LAYER

    for (size_t i = 0; i < outputLayer.neurons.size(); i++)
    {

        double output_value = outputLayer.neurons[i].output;

        outputLayer.neurons[i].delta = (output_value - target[i]); //* sigmoid_derivative(output_value);
    }

    // GOING BACKWARDS THROUGH THE LAYERS:

    for (int j = this->layers.size() - 2; j >= 0; j--)
    {

        for (size_t i = 0; i < layers[j].neurons.size(); i++)
        {
            // cout << "finding error in NEURON " << i << " of HIDDEN LAYER " << j << endl;

            double sum = 0.0;

            // it's the next layer, the closest to the output
            for (auto &next_neuron : layers[j + 1].neurons)
            {

                sum += next_neuron.weights[i] * next_neuron.delta;
            }

            double hidden_layer_output = layers[j].neurons[i].output;

            double derivative_value = 0.0;
            if (func == 'r')
            {
                derivative_value = relu_deriv(layers[j].neurons[i].output);
            }
            else if (func == 't')
            {
                derivative_value = tanh_deriv(layers[j].neurons[i].output);
            }
            else if (func == 's')
            {
                derivative_value = sigmoid_deriv(layers[j].neurons[i].output);
            }

            layers[j].neurons[i].delta = sum * derivative_value;
        }
    }

    // UPDATE WEIGHTS

    vector<double> previous_activations = input;

    for (auto &layer : layers)
    {

        for (auto &neuron : layer.neurons)
        {

            for (size_t i = 0; i < neuron.weights.size(); i++)
            {

                // GRADIENT DESCENT
                neuron.weights[i] -= neuron.delta * this->learning_rate * previous_activations[i];
            }

            // NOW I'M IN THE NEURON

            neuron.bias -= this->learning_rate * neuron.delta;
        }

        // clear the previous activations vector

        previous_activations.clear();
        for (auto &n : layer.neurons)
        {

            previous_activations.push_back(n.output);
        }
    }
}

// TRAINING IS DONE WITH ALL DATA AND THE LAST LAYER DETERMINES THE OUTPUT, BUT YOU NEED TO PROVIDE ALL DATA TO TRAIN IT (activations)
// THE PREDICTOR WILL DETERMINE BASED ON THE ENEMY'S MOVEMENT, WHAT THE PLAYER SHOULD DO
// the structure will be like this
/*

    {
        [3, 4, 5, ...., 5],
        [3, 4, 5, ...., 5],
        [3, 4, 5, ...., 5]

    }

and we don't know the data we will include yet, so I'll make both vectors dynamic

*/

// THE LEARNING SET HAS INPUT AND OUTPUT: input is the enemy's movement and output is the player's movement BUT BOTH ARE THE 'INPUT' TO TRAIN THE NETWORK
// for the predictor, the idea is that the input is enemy data and the output is character data

// supposedly 1 hidden layer is enough for regression problems
// in the print you can see 0 and 1 (output and hidden layers, the input layer is not explicit)

double BackPropagation::train(const vector<vector<double>> &X, const vector<vector<double>> &Y, int epochs, char activation)
{

    vector<int> indices(X.size());
    for (int i = 0; i < indices.size(); i++)
        indices[i] = i;

    double mse = 0.0;

    for (int e = 0; e < epochs; e++)
    {

        shuffle(indices.begin(), indices.end(), mt19937(random_device{}()));
        mse = 0.0;

        for (int idx : indices)
        {
            auto prediction = forward(X[idx], activation);
            back(X[idx], Y[idx], activation);

            for (size_t j = 0; j < prediction.size(); j++)
                mse += pow(prediction[j] - Y[idx][j], 2);
        }

        mse /= X.size();

        // double rmse = sqrt(mse);
        // double precision = (1 - rmse) * 100;

        cout << "EPOCH " << e + 1 << "/" << epochs << " - MSE: " << mse << endl;
    }
    return mse;
}