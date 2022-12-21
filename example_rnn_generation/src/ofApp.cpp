﻿#include "ofApp.h"
#include "RNN.h"

//Declare RNN object 
//RNN is defined in RNN.h
std::shared_ptr<RNN> rnn;

//holder for network state text
string state_message = "Network is randomly inited. You can load trained network from a file";


//--------------------------------------------------------------
void ofApp::setup(){
	cout << "Example of character-level RNN which generates names conditioned by country" << endl;

	all_letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ .,;'-";
	n_letters = all_letters.length();
	alphabet.setup(all_letters);
		
	//--------------------------------------------------------------
	//Reading train data - examples of names for different countries
	//--------------------------------------------------------------
	ofDirectory dir;
	n_categories = dir.listDir("names_ansi");
	cout << "Categories: " << n_categories << endl;

	categories.resize(n_categories);
	category_lines.resize(n_categories);

	bool error = false;
	for (int i = 0; i < n_categories; i++) {
		categories[i] = ofFilePath::removeExt(dir.getName(i));

		//Parse the file
		ofBuffer buffer = ofBufferFromFile(dir.getPath(i));
		int k = 0;
		for (auto line : buffer.getLines()) {
			k++;
			if (!checkWord(line)) {
				cout << "Error in " << dir.getName(i) << ", bad word " << k << " '" << line << "'" << endl;
				error = true;
			}
			category_lines[i].push_back(line);
		}
		cout << "    " << i + 1 << ": " << categories[i] << " " << category_lines[i].size() << endl;
	}

	if (error) {
		cout << "Exiting due error in input data..." << endl;
		OF_EXIT_APP(0);
	}

	//--------------------------------------------------------------
	//Create RNN
	//('rnn' variable is defined above)
	//--------------------------------------------------------------
	cout << "Creating network..." << endl;
	n_hidden = 128;
		
	rnn = std::make_shared<RNN>(n_letters, n_hidden, n_categories);

	//--------------------------------------------------------------
	//Run interaction with user.
	//--------------------------------------------------------------
	//This is infinite loop
	menu();

	//-----------------------------------------------------------
	//Here are various tests - uncomment to test what you want, and comment "menu();" above
	//-----------------------------------------------------------
	//Test lineToTensor
	//auto test = alphabet.string_to_onehot("abc");
	//cout << "string_to_onehot test " << endl << test << endl;
}


//--------------------------------------------------------------
//User interaction via console, infinite loop
void ofApp::menu() {
	while (true) {
		cout << endl;
		cout << "    Select action: " << endl;
		cout << "        1 - train network (you can repeat it to train incrementally)" << endl;
		cout << "        2 - save network" << endl;
		cout << "        3 - load network" << endl;
		cout << "        4 - predict (enter a country and a first symbol generate a name)" << endl;
		cout << "        5 - exit" << endl;
		cout << "        (" << state_message << ")" << endl;
		cout << ">>> ";

		string key;
		cin >> key;
		cout << endl;
		if (key == "1") {
			cout << "Train..." << endl;
			train();
			state_message = "Network is trained, but not saved to file";
		}
		string file_name = "rnn.pt";
		if (key == "2") {
			cout << "Saving network to " << file_name << " ..." << endl;
			torch::save(rnn, ofToDataPath(file_name));
			state_message = "Network is saved";
		}
		if (key == "3") {
			cout << "Loading network from " << file_name << " ..." << endl;
			torch::load(rnn, ofToDataPath(file_name));
			state_message = "Network is loaded";
		}
		if (key == "4") {
			cout << "Type country and first letter separated by space (for example: 'Russian R', 'Spanish P', see more in 'names_ansi' folder):" << endl;
			cout << "        (" << state_message << ")" << endl;
			cout << ">>> ";
			string line;
			cin >> line;
			int n_predictions = 3;
			predict(line, n_predictions);
			//This function returns list of results, but we ignoring it here,
			//but can be useful in other situations
		}
		if (key == "5") {
			cout << "Exiting..." << endl;
			OF_EXIT_APP(0);
		}
	}
}


//--------------------------------------------------------------
//Check if word is correct (contains only registered symbols)
bool ofApp::checkWord(string line) {
	if (line.empty()) {		//we doesn't allow empty words
		return false;
	}
	for (int i = 0; i < line.length(); i++) {
		size_t index = all_letters.find_first_of(line[i]);
		//cout << "index " << index << endl;
		if (index == string::npos) {
			return false;
		}
	}
	return true;
}

//--------------------------------------------------------------
//Generate random example
ofApp::TrainingExample ofApp::randomTrainingExample() {
	int cat = ofxPytorch::randint(n_categories);
	auto &lines = category_lines[cat];

	int i = ofxPytorch::randint(lines.size());

	TrainingExample ex;
	ex.category = cat;
	ex.line = lines[i];
	ex.category_tensor = torch::tensor({ cat }, torch::kLong);
	ex.line_tensor = alphabet.string_to_onehot(ex.line);

	return ex;
}

//--------------------------------------------------------------

//Training is implemented directly, without optimizers
//Function returns loss.

float ofApp::train_step(TrainingExample &ex) {
	//rnn->train();  //Enable training mode (must be used for dropout and others)


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

	return loss.item<float>();
}


//--------------------------------------------------------------
vector<ofApp::PredictResult> ofApp::predict(string line, int n_predictions) {
	//rnn->eval();  //Enable working mode (must be used for dropout and others)

	
	cout << "Predict: '" << line << "'" << endl;
	if (!checkWord(line)) {
		cout << "Bad word, please use Latin symbols only" << endl;
		return vector<ofApp::PredictResult>();
	}

	auto line_tensor = alphabet.string_to_onehot(line);

	rnn->zero_grad();
	torch::Tensor output;
	torch::Tensor hidden = rnn->initHidden();

	for (int i = 0; i < line_tensor.size(0); i++) {
		torch::Tensor pair = rnn->forward(line_tensor[i], hidden);
		auto splitted = pair.split_with_sizes({ rnn->output_size, rnn->hidden_size }, 1);
		output = splitted[0];
		hidden = splitted[1];
	}

	std::tuple<torch::Tensor, torch::Tensor> result = output.topk(n_predictions);
	//result[0] holds values, result[1] holds indices

	//cout << "prediction values:" << endl
	//	<< std::get<0>(result) << endl
	//	<< " categories:" << endl
	//	<< std::get<1>(result) << endl;
		
	vector<ofApp::PredictResult> items(n_predictions);
	for (int i = 0; i < n_predictions; i++) {
		auto &item = items[i];
		item.value = std::get<0>(result)[0][i].item<float>();
		int cat = std::get<1>(result)[0][i].item<int>();
		item.category = categories[cat];

		cout << "     " << item.value << "  " << item.category << endl;
	}

	return items;
}




//--------------------------------------------------------------
void ofApp::train() {
	int n_iters = //100000;
		10000;				
	int print_every = 1000;
	float time0 = ofGetElapsedTimef();
	float current_loss = 0;
	for (int i = 0; i < n_iters; i++) {
		TrainingExample ex = randomTrainingExample();
		float loss = train_step(ex);
		current_loss += loss;
		if ((i + 1) % print_every == 0) {
			float time = ofGetElapsedTimef();
			int elapsed = int(time - time0);
			cout << "    " << i + 1 << "/" << n_iters 
				<< "\t" << (i+1)*100/n_iters << "%" 
				<< "\t" << elapsed << " sec"
				<< "\tmean loss " << current_loss / print_every << endl;
			current_loss = 0;
		}
	}
}

//--------------------------------------------------------------
