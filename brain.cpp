#include<math.h>
#include"brain.h"
#include"window.h"


Synapse::Synapse()
{
	CREATOR = 1;
}
Synapse::~Synapse()
{

}
Synapse MakeSynapse(float value, Neuron *parent, Neuron *other)
{
	Synapse ret;
	ret.Other = other;
	ret.Weight = value;
	ret.CREATOR = 3;
	return ret;
}



//---------------------------------------------------------------------------------------------------
//                             LAYER CONSTRUCTOR AND METHODS                                         
//___________________________________________________________________________________________________
Layer::Layer()
{ }
Layer::~Layer()
{ }



//---------------------------------------------------------------------------------------------------
//                             NEURON CONSTRUCTOR AND METHODS                                        
//___________________________________________________________________________________________________
Neuron::~Neuron()
{ }
Neuron::Neuron()
	: Value(0)
{ }



//---------------------------------------------------------------------------------------------------
//                             NET CONSTRUCTOR AND METHODS                                           
//___________________________________________________________________________________________________
Net::Net()
{ }
Net::~Net()
{ }
Net::Net(int inputs, int hidden, int outputs)
{

	//_____________________________Input_________________________________________________________________

	Layer *Input = new Layer();
	Input->LayerType = Layer::Input;
	Input->Number_of_Neurons = inputs;

	FOR_LOOP(N, inputs)
	{
		Input->Neurons.emplace_back();
	}

	Layers.push_back(*Input);

	//___________________________Hidden__________________________________________________________________

	Layer *Hidden = new Layer();
	Hidden->LayerType = Layer::Hidden;
	Hidden->Number_of_Neurons = hidden;

	FOR_LOOP(H, hidden)
	{
		Hidden->Neurons.emplace_back();
	}
	Layers.push_back(*Hidden);

	//___________________________Output__________________________________________________________________

	Layer *Output = new Layer();
	Output->LayerType = Layer::Output;
	Output->Number_of_Neurons = outputs;

	FOR_LOOP(O, outputs)
	{
		Output->Neurons.emplace_back();
	}

	Layers.push_back(*Output);
	//___________________________________________________________________________________________________
	Number_of_Layers = 3;



	FOR_LOOP(Hcount, Layers[1].Number_of_Neurons)
	{
		FOR_LOOP(Icount, Layers[0].Number_of_Neurons)
		{
			Layers[1].Neurons[Hcount].Synapses.push_back(
				MakeSynapse(RANDOM(1), &Layers[1].Neurons[Hcount],
					&Layers[0].Neurons[Icount]));
		}
	}

	FOR_LOOP(Ocount, Layers[2].Number_of_Neurons)
	{
		FOR_LOOP(Hcount, Layers[1].Number_of_Neurons)
		{
			Layers[2].Neurons[Ocount].Synapses.push_back(
				MakeSynapse(RANDOM(1), &Layers[2].Neurons[Ocount],
					&Layers[1].Neurons[Hcount]));
		}
	}
}

void Net::Think()
{
	float Sum = 0.0;

	FOR_LOOP(HiddenCount, Layers[1].Number_of_Neurons)
	{
		FOR_LOOP(InputCount, Layers[0].Number_of_Neurons)
		{
			Sum += (Layers[0].Neurons[InputCount].Value *
				Layers[1].Neurons[HiddenCount].Synapses[InputCount].Weight);
		}
		Layers[1].Neurons[HiddenCount].Value = Activation(Sum);
	}

	Sum = 0.0;

	FOR_LOOP(OutputCount, Layers[2].Number_of_Neurons)
	{
		FOR_LOOP(HiddenCount, Layers[1].Number_of_Neurons)
		{
			Sum += (Layers[1].Neurons[HiddenCount].Value *
				Layers[2].Neurons[OutputCount].Synapses[HiddenCount].Weight);
		}
		Layers[2].Neurons[OutputCount].Value = Activation(Sum);
	}
}
void Net::Draw() { }


static inline float Sigmoid(float x)
{
	return 1.0 / (1.0 + exp(-x));
}

static inline float Activation(float x)
{
	return (Sigmoid(x) * 2) - 1;
}
