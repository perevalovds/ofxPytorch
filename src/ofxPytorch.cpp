
#include "ofxPytorch.h"


//--------------------------------------------------------------
int ofxPytorch::randint(int n) {	//0..n-1
	return torch::randint(n, { 1 }).item<int>();

}


//--------------------------------------------------------------
int ofxPytorch::topk_index(torch::Tensor input) {	//returns index of maximal value
	return std::get<1>(input.topk(1)).item<float>();
}

//--------------------------------------------------------------
