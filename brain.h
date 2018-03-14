#pragma once

#include<iostream>
#include<vector>

using namespace std;

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

	vector<Synapse> Synapses;
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
	vector<Neuron> Neurons;

};


class Net
{
public:
	Net(); ~Net();
	Net(int inputs, int hidden, int outputs);

	int Number_of_Layers;
	vector<Layer> Layers;

	void Think();
	void Draw();
};

extern inline float Sigmoid(float x);
extern inline float Activation(float x);
