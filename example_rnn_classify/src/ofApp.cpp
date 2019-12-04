
#include "ofApp.h"

// Define a new Module.
// input    hidden
//    combined
//  i2o       i2h
// softmax
// output   hidden

//https://pytorch.org/cppdocs/frontend.html
struct Net : torch::nn::Module {
	//We need to declare modules in this way and create in constructor for reference-type using
	torch::nn::Linear i2o{ nullptr }, i2h{ nullptr };

	int input_size, hidden_size, output_size;

	Net(int input_size, int hidden_size, int output_size) {
		this->input_size = input_size;
		this->hidden_size = hidden_size;
		this->output_size = output_size;

		i2o = register_module("i2o", torch::nn::Linear(input_size + hidden_size, output_size));
		i2h = register_module("i2h", torch::nn::Linear(input_size + hidden_size, hidden_size));

	}

	// Implement the Net's algorithm.
	torch::Tensor forward(torch::Tensor input, torch::Tensor hidden) {
		torch::Tensor combined = torch::cat((input, hidden), 1);
		hidden = i2h->forward(combined);
		torch::Tensor output = i2o->forward(combined);
		output = torch::log_softmax(output, /*dim=*/1);
		return (output, hidden);

		/*	def initHidden(self) :
			return torch.zeros(1, self.hidden_size)
			*/
		// Use one of many tensor manipulation functions.
		//x = torch::relu(fc1->forward(x.reshape({ x.size(0), 784 })));
		//x = torch::dropout(x, /*p=*/0.5, /*train=*/is_training());
		//x = torch::relu(fc2->forward(x));
		//x = torch::log_softmax(fc3->forward(x), /*dim=*/1);
		//return x;
	}

};

//--------------------------------------------------------------
void ofApp::setup(){
	cout << "Example of creating simple character-level RNN which is capable to classify names" << endl;

	//Reading train data - examples of names for different countries
	ofDirectory dir;
	n_categories = dir.listDir("names_ansi");
	cout << "Categories: " << n_categories << endl;

	categories.resize(n_categories);
	category_lines.resize(n_categories);

	for (int i = 0; i < n_categories; i++) {
		categories[i] = ofFilePath::removeExt(dir.getName(i));

		//Parse the file
		ofBuffer buffer = ofBufferFromFile(dir.getPath(i));
		for (auto it : buffer.getLines()) {
			category_lines[i].push_back(it);
		}
		cout << "  " << i + 1 << ": " << categories[i] << " " << category_lines[i].size() << endl;
	}

	//Construct list of all letters
	//https://github.com/spro/practical-pytorch/blob/master/char-rnn-classification/data.py
	//https://kite.com/python/examples/288/string-get-all-ascii-letters
	all_letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ .,;'-";
	n_letters = all_letters.length();

	//Test lineToTensor
	//auto test = lineToTensor("abc");
	//cout << "lineToTensor test " << endl << test << endl;

	//Create RNN
	//"RNN module(mostly copied from the PyTorch for Torch users tutorial) 
	//is just 2 linear layers which operate on an input and hidden state, 
	//with a LogSoftmax layer after the output."




	//at::Tensor a = at::ones({ 2, 2 }, at::kInt);
	//at::Tensor b = at::randn({ 2, 2 });
	//auto c = a + b.to(at::kInt);
	//std::cout << c << std::endl;

	//torch::Tensor tensor = torch::rand({ 2, 3 });
	//cout << "Tensor:" << endl;
	//cout << tensor << endl;

	//string file_name = "saved.tensor";
	//torch::save(tensor, ofToDataPath(file_name));

	//cout << endl;
	//cout << "PyTorch works well, the created tensor was saved to '" << file_name << "'" << endl;

}


//--------------------------------------------------------------
//Turn a line into a <line_length x 1 x n_letters>,
//or an array of one-hot letter vectors
torch::Tensor ofApp::lineToTensor(string line) {
	torch::Tensor tensor = torch::zeros({ int(line.length()), 1, n_letters });
	for (int i = 0; i < line.length(); i++) {
		size_t index = all_letters.find_first_of(line[i]);
		cout << "index " << index << endl;
		if (index == string::npos) {
			cout << "Error, bad symbol in " << line << endl;
			OF_EXIT_APP(0);
		}
		tensor[i][0][index] = 1;
	}

	return tensor;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
