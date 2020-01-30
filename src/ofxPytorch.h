#pragma once

/*
ofxPytorch - addon for openFrameworks, made by Denis Perevalov.

It allows to use PyTorch machine learning capabilities in your(C++) openFrameworks projects.
To implement proper libtorch settings, the libtorch library 
is placed not in addon's regular libs folder, 
but in libs_ folder to prevent Project Generator from automatically including unnecessary libtorch's subfolders 
into Visual Studio project's includes.
*/

#include "ofMain.h"

#include <torch/torch.h>


//Often used functions
struct ofxPytorch {
	// Return uniform random int in 0..n-1
	static int randint(int n);	

	// Return index of maximal value
	static int topk_index(torch::Tensor input);
	
};


// Create one-hot vector from line using alphabet
// The class uses map from chars to int and so works quite fast
struct ofxPyStringOnehotGenerator {
	ofxPyStringOnehotGenerator() {}
	ofxPyStringOnehotGenerator(const string &alphabet);
	void setup(const string &alphabet);

	// If ignore_unknown_symbols==false and is unknown symbol - returns empty tensor
	torch::Tensor string_to_onehot(const string &line, bool ignore_unknown_symbols = true);

	string alphabet() const { return alphabet_; }

protected:
	string alphabet_;	//used alphabet
	int n_ = 0;			//alphabet size
	vector<int> map_;	//map a char to index in alphabet, if no mapping, then -1
};