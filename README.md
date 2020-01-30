# ofxPytorch

**ofxPytorch** is an addon for openFrameworks, made by Denis Perevalov.
It allows to use PyTorch machine learning capabilities in your (C++) openFrameworks projects. 

PyTorch is a very powerful and popular ML library. Its C++ frontend named LibTorch allows 
to use PyTorch in realtime C++-based applications. Here we adopted it for using in openFrameworks.

## Addon's Features

* Addon is added to your oF project by a regular way using Project Generator.

* Addon works with CPU only (not using CUDA).

* Addon must be compiled in **64 bit, Release mode** only (no Debug mode currently allowed)
The reason: "on Windows, debug and release builds are not ABI-compatible" - https://pytorch.org/tutorials/advanced/cpp_frontend.htmlfor,
so for Debug mode you need to use debug version of LibTorch.
See 'Notes on debugging in Release mode' below.

* Addon uses LibTorch 1.3.1 (downloaded from https://pytorch.org/get-started/locally/).


## Requirements

* Addon works in Windows, Visual Studio 2017 C++, openFrameworks 0.10.1.

* Please compile projects with the addon in 64 bit mode.

## Installing

1. Please install addon to oF's **addons** folder.

2. Download LibTorch's .H, .LIB and .DLL files as a ZIP archive named **ofxPytorch_libs_win_64_CPU.zip**
from **releases** of this repository: https://github.com/perevalovds/ofxPytorch/releases

3. Unpack this ZIP into the addon's folder (we can't add it to the github directly because of limit on the file size).
The ZIP has the following structure:
* **libs_/libtorch** - .H and .LIB files from LibTorch 1.3.1 (https://pytorch.org/get-started/locally/) excluding DLLs.
* **libs/dlls** - DLLs from LibTorch 1.3.1

(See Appendix below for explanation).


## Examples

### example_test
It's a minimalistic example to check PyTorch is linked properly. 
It's based on https://pytorch.org/cppdocs/installing.html#minimal-example
The example creates a random tensor, prints it in Console and saves to **data** folder as **saved.tensor** file.
This example demonstrates the way for using files made in oF project in your Python's Pytorch code, 
and back, use your Python's Pytorch data in the oF project!
(Note, actually **saved.tensor** is ZIP archive, which you can explore).

### example_rnn_classify
This is a matured example of creating simple character-level RNN 
which is capable to classify names by countries. This example doesn't uses optimizators,
but rely on PyTorch's native autograd powerfullness.

It's C++-ported and modified version of RNN's PyTorch (Python) tutorial  
"NLP From Scratch: Classifying Names with a Character-Level RNN" by Sean Robertson
https://pytorch.org/tutorials/intermediate/char_rnn_classification_tutorial.html
https://github.com/spro/practical-pytorch/blob/master/char-rnn-classification

The example includes interactive menu, 
allowing you to train, save, load and test network.

For example, 'Perevalov' is not in training database, but is correctly classified as 'Russian':

```
Type name (for example: Smith, Kabakov, Mendoza, see more in 'names_ansi' folder):
        (Network is loaded)
>>> Perevalov
Predict: 'Perevalov'
     -0.312548  Russian
     -2.70598  Greek
     -2.94786  Czech
```

The technology of the example can be used for various text classification tasks,
such as defining the language of the text and detecting blogger by article's title
(see more info on this at original tutorial https://pytorch.org/tutorials/intermediate/char_rnn_classification_tutorial.html)


### example_rnn_generation
This is a matured example of using RNN for 
generating names conditioned by countries. It's pure deterministic generation,
which uses first character and country to generate the rest symbols of name.
This example doesn't uses optimizators, but rely on PyTorch's native autograd power.

The example includes interactive menu, 
allowing you to train, save, load and test network.

## PyTorch C++ documentation 

Using PyTorch C++ is explained here: 
* "Using the PyTorch C++ Frontend" tutorial: https://pytorch.org/tutorials/advanced/cpp_frontend.html
* All documentation: https://pytorch.org/cppdocs/index.html
* High-level frontend documentation: https://pytorch.org/cppdocs/frontend.html
* Working with tensors: https://pytorch.org/cppdocs/notes/tensor_basics.html
https://pytorch.org/cppdocs/notes/tensor_creation.html
* Tests - serves as examples: https://github.com/pytorch/pytorch/tree/master/test/cpp/api

Other:
* Library API: https://pytorch.org/cppdocs/api/library_root.html
* Library API/torch::nn: https://pytorch.org/cppdocs/api/namespace_torch__nn.html
* (Python) torch module documentation (explanation of 'cat', 'stack' and other functions) https://pytorch.org/docs/stable/torch.html

* (Python) nn module documentation (NLLLoss and others) https://pytorch.org/docs/stable/nn.html

* (Python) Pytorch ML tutorials: https://pytorch.org/tutorials/index.html

## Notes on debugging in Release mode


Because we are running the project with PyTorch in Release mode, 
it can be hard to find the error, when program halts.
To simplify this, you can use catching exceptions:

~~~~
try { 
	//PyTorch code here
} 

catch (const std::exception& exc) { 
	cout << "Exception in ...(you description of the code): " << exc.what() << endl; 
} 
~~~~

## Appendix: Visual Studio project setup for using LibTorch without oF

All addon's setup should be made automatically by Project Generator.

But, sometimes it's required to build LibTorch without oF and addon, 
so this notes may be useful (also, you can see them implemented in addon_config.mk file)

1. Add to includes ONLY this directories: 
   .... libtorch\include
   .... libtorch\include\torch\csrc\api\include
2. Add to linker input:
     c10.lib;torch.lib    
3. Set paths to libs in linker's General - Additional Library Directories
   .... libtorch\lib
4. Copy to project's bin the following DLLs: libiomp5md.dll, libiompstubs5md.dll, torch.dll
(and others if required).

To implement this in addon's settings, the **libtorch** library .H and .LIB files
are placed not in addon's regular **libs** folder, 
but in **libs_** folder to prevent Project Generator from automatically including 
unnecessary libtorch's subfolders into Visual Studio project's includes.

However, libtorch's DLLS are placed in libs folder to be copied into your project's **bin** automatically.
