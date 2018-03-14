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
	Layer(); ~Layer();

	enum LType
	{
		Input,
		Hidden,
		Output
	};   LType LayerType;

	int Number_of_Neurons;
	std::vector<Neuron> Neurons;

};


class Net
{
public:
	Net(); ~Net();
	Net(int inputs, int hidden, int outputs);

	int Number_of_Layers;
	std::vector<Layer> Layers;

	void Think();
	void Draw();
};

extern inline float Sigmoid(float x);
extern inline float Activation(float x);
