#pragma once

#define NN_LAYER_WIDTH 16
#define NN_NUM_CONNECTIONS 4
#define NN_NUM_LAYERS 3
#define NN_TOTAL_SIZE NN_NUM_LAYERS*NN_NUM_CONNECTIONS*NN_LAYER_WIDTH

class NeuralNet
{
public:
	int netLayers[NN_TOTAL_SIZE];
	int workSpace[NN_LAYER_WIDTH * (NN_NUM_LAYERS + 1)];

	void LoadNet(const char* InputString);
	void RunNet();
	void ConvertCharToNNIndices(unsigned char idxCode, int& idx1, int& idx2);
	int GetNNOutputForChar(char idxCode);
	void SetNNInputForChar(char idxCode, int input);
};



