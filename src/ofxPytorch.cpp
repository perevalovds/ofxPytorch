
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
// Create one-hot vector from line using alphabet
ofxPyStringOnehotGenerator::ofxPyStringOnehotGenerator(const string &alphabet) {
	setup(alphabet);
}

void ofxPyStringOnehotGenerator::setup(const string &alphabet) {
	alphabet_ = alphabet;
	n_ = alphabet_.size();
	map_.resize(255);
	fill(map_.begin(), map_.end(), -1);
	for (int i = 0; i < n_; i++) {
		map_[alphabet[i]] = i;
	}
}


torch::Tensor ofxPyStringOnehotGenerator::string_to_onehot(const string &line, bool ignore_unknown_symbols) {
	int n = line.length();
	torch::Tensor tensor = torch::zeros({ n, 1, n_ });
	for (int i = 0; i < n; i++) {
		int index = map_[line[i]];
		if (index == -1) {
			return torch::Tensor();
		}
		tensor[i][0][index] = 1;
	}
	return tensor;

}

//Also, for more optimal operations look at unsqueeze and scatter_ functions
//https://discuss.pytorch.org/t/what-kind-of-loss-is-better-to-use-in-multilabel-classification/32203/3
//(Python)
//labels = torch.tensor([1, 4, 1, 0, 5, 2])
//labels = labels.unsqueeze(0)
//target = torch.zeros(labels.size(0), 15).scatter_(1, labels, 1.)

//--------------------------------------------------------------
