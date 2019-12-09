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

//#define _CRT_SECURE_NO_WARNINGS
#include <torch/torch.h>



struct ofxPytorch {
	//returns uniform random int in 0..n-1
	static int randint(int n);	

	//returns index of maximal value
	static int topk_index(torch::Tensor input);
	
};