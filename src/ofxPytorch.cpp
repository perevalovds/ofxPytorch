
#include "ofxPytorch.h"


//--------------------------------------------------------------
int ofxPytorch::randint(int n) {	//0..n-1
	return torch::randint(n, { 1 }).item<int>();

}

//--------------------------------------------------------------
