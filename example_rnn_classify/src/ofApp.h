#pragma once

/*
example_rnn_classify is a matured example of creating simple character-level RNN 
which is capable to classify names by countries. This example doesn't uses optimizators,
but rely on PyTorch's native autograd powerfullness.

It's C++-ported and modified version of RNN's PyTorch (Python) tutorial  
"NLP From Scratch: Classifying Names with a Character-Level RNN" by Sean Robertson
https://pytorch.org/tutorials/intermediate/char_rnn_classification_tutorial.html
https://github.com/spro/practical-pytorch/blob/master/char-rnn-classification
(Note: comparing the original tutorial, in the current implementation 
"end of line" symbol is added after each word)


The example includes interactive menu, 
allowing you to train, save, load and test network.

For example, 'Perevalov' is not in training database, but is correctly classified as 'Russian':

```
Type name (for example: Smith, Kabakov, Mendoza, see more in 'names_ansi' folder):
        (Network is loaded)
>>>> Perevalov
Predict: 'Perevalov'
     -0.312548  Russian
     -2.70598  Greek
     -2.94786  Czech
```

The technology of the example can be used for various text classification tasks,
such as defining the language of the text and detecting blogger by article's title
(see more info on this at original tutorial https://pytorch.org/tutorials/intermediate/char_rnn_classification_tutorial.html)

*/

#include "ofMain.h"
#include "ofxPytorch.h"


class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	//train data
	int n_categories;	//number of categories	
	vector<string> categories;	//category's name
	vector<vector<string> > category_lines;	//examples of names for category

	//letters
	int n_letters;
	string all_letters;
	string end_of_line;			//end of line symbol - added in ofxPytorch port, and was not used in the original tutorial
	ofxPyStringOnehotGenerator alphabet;	//creating one-hot vectors

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
