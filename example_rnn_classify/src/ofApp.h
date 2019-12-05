#pragma once

/*
example_rnn_classify - example of creating simple character-level RNN 
which is capable to classify names. This example doesn't uses optimizators,
but rely on PyTorch's native autograd powerfullness.

It's C++-ported and modified version of RNN's PyTorch (Python) tutorial
"NLP From Scratch: Classifying Names with a Character-Level RNN" by Sean Robertson
https://pytorch.org/tutorials/intermediate/char_rnn_classification_tutorial.html
https://github.com/spro/practical-pytorch/blob/master/char-rnn-classification
*/

#include "ofMain.h"
#include "ofxPytorch.h"




class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);


	//train data
	int n_categories;	//number of categories	
	vector<string> categories;	//category's name
	vector<vector<string> > category_lines;	//examples of names for category

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
