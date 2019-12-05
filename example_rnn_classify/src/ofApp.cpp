
#include "ofApp.h"
#include "RNN.h"

//The Python code from this RNN tutorial:
//https://pytorch.org/tutorials/intermediate/char_rnn_classification_tutorial.html
//is translated into C++ code using Pytorch C++ frontend documentation:
//https://pytorch.org/cppdocs/frontend.html


//Declare RNN object 
//RNN is defined in RNN.h
std::shared_ptr<RNN> rnn;


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
	//('rnn' variable is defined above)
	cout << "Creating network..." << endl;
	n_hidden = //128;
		5;	//TEST
	rnn = std::make_shared<RNN>(n_letters, n_hidden, n_categories);

	//Test processing one symbol
	/*cout << "Test processing one symbol" << endl;
	auto input = lineToTensor("Albert");
	auto hidden = rnn->initHidden();	
	auto pair = rnn->forward(input[0], hidden);
	auto splitted = pair.split_with_sizes({ rnn->output_size, rnn->hidden_size }, 1);
	auto output = splitted[0];
	hidden = splitted[1];
	cout << "output " << output << endl;
	cout << "hidden " << hidden << endl;
	cout << "result: " << categoryFromOutput(output) << endl;
	*/

	//Test generating random example
	/*
	cout << "Generating example..." << endl;
	TrainingExample ex = randomTrainingExample();
	cout << "example: " << categories[ex.category] << " " << ex.line << endl;
	//cout << ex.category_tensor << endl;
	//cout << ex.line_tensor << endl;
	*/

	//Test train step
	/*
	cout << "Train step..." << endl;
	train_step();
	*/


	//save the net - probably without 'hidden' value - torch::save(rnn, ofToDataPath("rnn.pt"));
}


//--------------------------------------------------------------
//Turn a line into a <line_length x 1 x n_letters>,
//or an array of one-hot letter vectors
torch::Tensor ofApp::lineToTensor(string line) {
	torch::Tensor tensor = torch::zeros({ int(line.length()), 1, n_letters });
	for (int i = 0; i < line.length(); i++) {
		size_t index = all_letters.find_first_of(line[i]);
		//cout << "index " << index << endl;
		if (index == string::npos) {
			cout << "Error, bad symbol in " << line << endl;
			OF_EXIT_APP(0);
		}
		tensor[i][0][index] = 1;
	}

	return tensor;
}

//--------------------------------------------------------------
//Interpret the output of the network, which we know to be a likelihood of each category
int ofApp::categoryFromOutput(torch::Tensor output) {
	std::tuple<torch::Tensor, torch::Tensor> result = output.topk(1);
	//retuls[0] holds value, result[1] holds index
	//cout << "categoryFromOutput " << endl << std::get<0>(result) << " " << std::get<1>(result) << endl;
	torch::Tensor index = std::get<1>(result);
	return index.item<float>();

}

//--------------------------------------------------------------
//Generate random example
ofApp::TrainingExample ofApp::randomTrainingExample() {
	int cat = torch::randint(n_categories, { 1 }).item<int>();
	auto &lines = category_lines[cat];

	int i = torch::randint(lines.size(), { 1 }).item<int>();

	TrainingExample ex;
	ex.category = cat;
	ex.line = lines[i];
	ex.category_tensor = torch::tensor({ cat }, torch::kLong);
	ex.line_tensor = lineToTensor(ex.line);

	return ex;
}


//--------------------------------------------------------------
/*
Training is implemented directly, without optimizers
"Each loop of training will:
	Create input and target tensors
	Create a zeroed initial hidden state
	Read each letter in and
		Keep hidden state for next letter
	Compare final output to target
	Back-propagate
	Return the output and loss"
*/
void ofApp::train_step() {
	TrainingExample ex = randomTrainingExample();

	float learning_rate = 0.005; //"If you set this too high, it might explode.If too low, it might not learn"

	rnn->zero_grad();
	torch::Tensor output;
	torch::Tensor hidden = rnn->initHidden();

	for (int i = 0; i < ex.line_tensor.size(0); i++) {
		torch::Tensor pair = rnn->forward(ex.line_tensor[i], hidden);
		auto splitted = pair.split_with_sizes({ rnn->output_size, rnn->hidden_size }, 1);
		output = splitted[0];
		hidden = splitted[1];
		//break;//TEST
	}
	//cout << "output " << output << endl;
	//cout << "hidden " << hidden << endl;

	//NLL - negative log-likelihood, https://pytorch.org/docs/stable/nn.html
	torch::Tensor loss = torch::nll_loss(output, ex.category_tensor);

	//compute gradient
	loss.backward();

	//"Add parameters' gradients to their values, multiplied by learning rate"
	for (auto &p : rnn->parameters()) {
		if (!p.grad().defined()) {
			cout << "ERROR in train_step: empty parameter! It looks like something wrong with network structure" << endl;
			OF_EXIT_APP(0);
		}
		if (p.grad().defined()) {
			p.data().add_(p.grad().data(), torch::Scalar(-learning_rate));	//add_ - "_" means that object calling the function will be changed
		}
	}


	//return output, loss.item()
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
