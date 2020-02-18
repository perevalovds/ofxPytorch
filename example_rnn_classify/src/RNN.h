#pragma once

#include "ofMain.h"
#include "ofxPytorch.h"

//Network class from this RNN tutorial:
//https://pytorch.org/tutorials/intermediate/char_rnn_classification_tutorial.html
//is translated into C++ code using Pytorch C++ frontend documentation:
//https://pytorch.org/cppdocs/frontend.html



// Define a new Module.
//"RNN module(mostly copied from the PyTorch for Torch users tutorial) 
//is just 2 linear layers which operate on an input and hidden state, 
//with a LogSoftmax layer after the output."
// input    hidden
//    combined
//  i2o       i2h
// softmax
// output   hidden


struct RNN : torch::nn::Module {
	//We need to declare modules in this way and create in constructor for reference-type using
	//torch::nn::Linear i2o{ nullptr }, i2h{ nullptr };
	torch::nn::Linear i2o{ nullptr };
	torch::nn::Linear i2h{ nullptr };

	int input_size, hidden_size, output_size;

	RNN(int input_size, int hidden_size, int output_size) {

		this->input_size = input_size;
		this->hidden_size = hidden_size;
		this->output_size = output_size;

		i2h = register_module("i2h", torch::nn::Linear(input_size + hidden_size, hidden_size));
		i2o = register_module("i2o", torch::nn::Linear(input_size + hidden_size, output_size));

	}

	// Implement the RNN algorithm.
	torch::Tensor forward(torch::Tensor input, torch::Tensor hidden) {
		torch::Tensor combined = torch::cat({ input, hidden }, 1);
		torch::Tensor output = i2o->forward(combined);
		hidden = i2h->forward(combined);
		output = torch::log_softmax(output, /*dim=*/1);

		//We combine output and hidden in one tensor - 
		//because using other thing, such as std::tuple<torch::Tensor, torch::Tensor>(output, hidden)
		//does not allow to make "grad" computation

		combined = torch::cat({ output, hidden }, 1);
		return combined;
	}

	//Initialize hidden state
	torch::Tensor initHidden() {
		return torch::zeros({ 1, hidden_size });
	}

	//x = torch::relu(fc1->forward(x.reshape({ x.size(0), 784 })));
	//x = torch::dropout(x, /*p=*/0.5, /*train=*/is_training());
	//x = torch::relu(fc2->forward(x));
	//x = torch::log_softmax(fc3->forward(x), /*dim=*/1);


};
