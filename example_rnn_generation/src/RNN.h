#pragma once

#include "ofMain.h"
#include "ofxPytorch.h"

//Network class from this RNN tutorial:
//https://pytorch.org/tutorials/intermediate/char_rnn_generation_tutorial.html
//is translated into C++ code using Pytorch C++ frontend documentation:
//https://pytorch.org/cppdocs/frontend.html


struct RNN : torch::nn::Module {
	//We need to declare modules in this way and create in constructor for reference-type using
	//torch::nn::Linear i2o{ nullptr }, i2h{ nullptr };
	torch::nn::Linear i2o{ nullptr };
	torch::nn::Linear i2h{ nullptr };
	torch::nn::Linear o2o{ nullptr };
	//torch::nn::Dropout dropout{ nullptr };

	int input_size, hidden_size, output_size;
	float dropout_p = 0.1;

	RNN(int input_size, int hidden_size, int output_size) {

		this->input_size = input_size;
		this->hidden_size = hidden_size;
		this->output_size = output_size;

		i2h = register_module("i2h", torch::nn::Linear(input_size + hidden_size, hidden_size));
		i2o = register_module("i2o", torch::nn::Linear(input_size + hidden_size, output_size));
		o2o = register_module("o2o", torch::nn::Linear(hidden_size + output_size, output_size));
		//	self.dropout = nn.Dropout(0.1)
		//	self.softmax = nn.LogSoftmax(dim = 1)
	}

	// Implement the RNN algorithm.
	torch::Tensor forward(torch::Tensor category, torch::Tensor input, torch::Tensor hidden) {
		torch::Tensor input_combined = torch::cat({ category, input, hidden }, 1);
		hidden = i2h->forward(input_combined);
		torch::Tensor output = i2o->forward(input_combined);
		torch::Tensor output_combined = torch::cat({ hidden, output }, 1);

		output = i2o->forward(output_combined);
		output = torch::dropout(output, dropout_p, is_training());
		output = torch::log_softmax(output, /*dim=*/1);
		
		//We combine output and hidden in one tensor - 
		//because using other thing, such as std::tuple<torch::Tensor, torch::Tensor>(output, hidden)
		//does not allow to make "grad" computation

		torch::Tensor combined = torch::cat({ output, hidden }, 1);
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
