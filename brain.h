#pragma once

#include<iostream>
#include<vector>

class Neuron;

class Synapse
{
public:
    Synapse(); ~Synapse();

    int CREATOR;
    float     Weight;
    Neuron   *Other;

};
extern  Synapse   MakeSynapse(float value, Neuron *parent, Neuron *other);


class Neuron
{
public:
    Neuron(); ~Neuron();

    float Value;

    std::vector<Synapse> Synapses;
};

class Layer
{
public:
    Layer() = default;
    ~Layer() = default;

    enum LType
    {
        Input,
        Hidden,
        Output
    };   LType LayerType;

    std::vector<Neuron> Neurons;

};


class Net
{
public:
    Net() = default;
    ~Net() = default;
    Net(std::size_t inputs, std::size_t hidden, std::size_t outputs);

    std::vector<Layer> Layers;

    void Think();
    void Draw();
};

extern inline float Sigmoid(float x);
extern inline float Activation(float x);
