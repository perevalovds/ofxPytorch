#pragma once

/*
example_rnn_classify - example of creating simple character-level RNN 
which is capable to classify names. This example doesn't uses optimizators,
but rely on PyTorch's native autograd powerfullness.

It's C++ reimplementation of PyTorch tutorial 
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
	int n_categories = 0;	//number of categories	
	vector<string> categories;	//category's name
	vector<vector<string> > category_lines;	//examples of names for category

	//letters
	int n_letters = 0;
	string all_letters;

	//Turn a line into a <line_length x 1 x n_letters>,
	//or an array of one-hot letter vectors
	torch::Tensor lineToTensor(string line);

};