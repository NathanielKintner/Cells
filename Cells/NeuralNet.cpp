#include "NeuralNet.h"



void NeuralNet::LoadNet(const char * InputString)
{
	for (int i = 0; i < NN_TOTAL_SIZE; i++)
	{
		netLayers[i] = InputString[i];
	}
}

void NeuralNet::RunNet()
{
	//zero out all but the first layer (input layer)
	for (int i = NN_LAYER_WIDTH; i < NN_LAYER_WIDTH * NN_NUM_LAYERS; i++)
	{
		workSpace[i] = 0;
	}
	//do net math across all layers, using relu
	for (int CurrentLayer = 0; CurrentLayer < NN_NUM_LAYERS; CurrentLayer++)
	{
		for (int i = NN_LAYER_WIDTH * CurrentLayer; i < NN_LAYER_WIDTH * (CurrentLayer+1); i++)
		{
			//if this node's value is greater than 0
			if (workSpace[i] > 0)
			{
				int nodeConnectionsIdx = i * NN_NUM_CONNECTIONS;
				//look at the net layers to see what connections this node has, and for each connection
				for (int j = 0; j < NN_NUM_CONNECTIONS; j++)
				{
					unsigned char connectionValue = netLayers[nodeConnectionsIdx + j];
					unsigned char connectionDestination = ((CurrentLayer + 1) * NN_LAYER_WIDTH) + (connectionValue / 16);
					char connectionMultiplier = ((char)(connectionValue % 16)) - 7;
					//the value of the node being connected to will be modified based on the value and connection multiplier
					workSpace[connectionDestination] = workSpace[i] * connectionMultiplier;
				}
			}
		}
	}
	//clear input layer for fresh round of inputs
	for (int i = 0; i < NN_LAYER_WIDTH; i++)
	{
		workSpace[i] = 0;
	}
}

void NeuralNet::ConvertCharToNNIndices(unsigned char idxCode, int& idx1, int& idx2)
{
	//fb nathaniel here
	//this appears to be 'splitting' the input into two parts, such that any character that is input here
	//will actually translate to TWO locations on the input and output layers, allowing char-sized numbers
	//to freely index different locations in a less-than-char-sized input layer
	idx1 = idxCode % NN_LAYER_WIDTH;
	idx2 = (idxCode / NN_LAYER_WIDTH) % NN_LAYER_WIDTH;
}


int NeuralNet::GetNNOutputForChar(char idxCode)
{
	int idx1, idx2;
	ConvertCharToNNIndices(idxCode, idx1, idx2);
	return workSpace[idx1 + (NN_LAYER_WIDTH * NN_NUM_LAYERS)] + workSpace[idx2 + (NN_LAYER_WIDTH * NN_NUM_LAYERS)];
}


void NeuralNet::SetNNInputForChar(char idxCode, int input)
{
	int idx1, idx2;
	ConvertCharToNNIndices(idxCode, idx1, idx2);
	workSpace[idx1] += input;
	workSpace[idx2] += input;
}