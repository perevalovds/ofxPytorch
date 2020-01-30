#pragma once

/*
example_rnn_generation is a matured example of using RNN for 
generating names conditioned by countries. It's pure deterministic generation,
which uses first character and country to generate the rest symbols of name.
This example doesn't uses optimizators, but rely on PyTorch's native autograd power.

It's C++-ported and modified version of RNN's PyTorch (Python) tutorial  
"NLP From Scratch: Generating Names with a Character-Level RNN" by Sean Robertson
https://pytorch.org/tutorials/intermediate/char_rnn_generation_tutorial.html

The example includes interactive menu, 
allowing you to train, save, load and test network.

*/

#include "ofMain.h"
#include "ofxPytorch.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();


	//train data
	int n_countries;	//number of categories	
	vector<string> contries;	//category's name
	vector<vector<string> > country_names;	//examples of names for category

	//letters
	int n_letters;
	string all_letters;

	//number of hidden states
	int n_hidden;


	//Check if word is correct (contains only registered symbols)
	bool checkWord(string line);

	//Turn a line into a <line_length x 1 x n_letters>,
	//or an array of one-hot letter vectors
	torch::Tensor lineToTensor(string line);

	//Interpret the output of the network, which we know to be a likelihood of each category
	int categoryFromOutput(torch::Tensor output); 
	
	//Generate random example
	struct TrainingExample {
		int category = 0;
		string line;
		torch::Tensor category_tensor;
		torch::Tensor line_tensor;
	};
	TrainingExample randomTrainingExample();	

	//Train step
	//Training is implemented directly, without optimizers
	//on one sample (no mini-batches used).
	//Function returns loss.
	float train_step(TrainingExample &ex);

	//Training
	void train();
	
	//Prediction 
	struct PredictResult {
		string category;
		float value = 0;
	};

	vector<PredictResult> predict(string line, int n_predictions = 3);

	//User interaction via console
	void menu();

};
