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
//                             NET CONSTRUCTOR AND METHODS                                           
//___________________________________________________________________________________________________
Net::Net(std::size_t inputs, std::size_t hidden, std::size_t outputs)
{
    //_____________________________Input_________________________________________________________________

    Layer *Input = new Layer();
    Input->LayerType = Layer::Input;

    FOR_LOOP(N, inputs)
    {
        Input->Neurons.emplace_back();
    }

    Layers.push_back(*Input);

    //___________________________Hidden__________________________________________________________________

    Layer *Hidden = new Layer();
    Hidden->LayerType = Layer::Hidden;

    FOR_LOOP(H, hidden)
    {
        Hidden->Neurons.emplace_back();
    }
    Layers.push_back(*Hidden);

    //___________________________Output__________________________________________________________________

    Layer *Output = new Layer();
    Output->LayerType = Layer::Output;

    FOR_LOOP(O, outputs)
    {
        Output->Neurons.emplace_back();
    }

    Layers.push_back(*Output);


    FOR_LOOP(Hcount, Layers[1].Neurons.size())
    {
        FOR_LOOP(Icount, Layers[0].Neurons.size())
        {
            Layers[1].Neurons[Hcount].Synapses.push_back(
                MakeSynapse(RANDOM(1), &Layers[1].Neurons[Hcount],
                    &Layers[0].Neurons[Icount]));
        }
    }

    FOR_LOOP(Ocount, Layers[2].Neurons.size())
    {
        FOR_LOOP(Hcount, Layers[1].Neurons.size())
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

    FOR_LOOP(HiddenCount, Layers[1].Neurons.size())
    {
        FOR_LOOP(InputCount, Layers[0].Neurons.size())
        {
            Sum += (Layers[0].Neurons[InputCount].Value *
                Layers[1].Neurons[HiddenCount].Synapses[InputCount].Weight);
        }
        Layers[1].Neurons[HiddenCount].Value = Activation(Sum);
    }

    Sum = 0.0;

    FOR_LOOP(OutputCount, Layers[2].Neurons.size())
    {
        FOR_LOOP(HiddenCount, Layers[1].Neurons.size())
        {
            Sum += (Layers[1].Neurons[HiddenCount].Value *
                Layers[2].Neurons[OutputCount].Synapses[HiddenCount].Weight);
        }
        Layers[2].Neurons[OutputCount].Value = Activation(Sum);
    }
}
void Net::Draw()
{ }


static inline float Sigmoid(float x)
{
    return 1.0 / (1.0 + exp(-x));
}

static inline float Activation(float x)
{
    return (Sigmoid(x) * 2) - 1;
}
