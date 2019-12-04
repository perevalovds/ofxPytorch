# ofxPytorch

**ofxPytorch** is an addon for openFrameworks, made by Denis Perevalov.
It allows to use PyTorch machine learning capabilities in your (C++) openFrameworks projects. 

PyTorch is a very powerful and popular ML library. Its C++ frontend named LibTorch allows 
to use PyTorch in realtime C++-based applications. Here we adopted it for using in openFrameworks.

## Addon's Features

* Addon works with CPU only (not using CUDA), 64 bit mode.

* Addon uses LibTorch 1.3.1 (downloaded from https://pytorch.org/get-started/locally/).

* Addon is added to your oF project by a regular way using Project Generator.


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

* **example_test** - minimalistic example to check PyTorch is linked properly. 
It's based on https://pytorch.org/cppdocs/installing.html#minimal-example
The example creates a random tensor, prints it in Console and saves to **data** folder as **saved.tensor** file.
This example demonstrates the way for using files made in oF project in your Python's Pytorch code, 
and back, use your Python's Pytorch data in the oF project!
(Note, actually **saved.tensor** is ZIP archive, which you can explore).


## PyTorch C++ documentation 

Using PyTorch C++ is explained here: https://pytorch.org/cppdocs/frontend.html


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
