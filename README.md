# ofxPytorch

ofxPytorch is an addon for openframeworks, which allows to use PyTorch machine learning capabilities 
in your (C++) openFrameworks projects. 

## Features

* Addon works with CPU only (not using CUDA)

* Addon uses LibTorch 1.3.1 (downloaded from https://pytorch.org/get-started/locally/)


## Requirements

Addon works in Windows, Visual Studio 2017 C++, openFrameworks 0.10.1.


## Examples


## Build notes
(Useful for build LibTorch without this addon oF)

1. Compile in 64 bit
2. Add to includes: 
   .... libtorch\include
   .... libtorch\include\torch\csrc\api\include
3. Add to linker input:
     caffe2.lib;c10.lib;torch.lib    
4. Set paths to libs
   .... libtorch\lib
5. Copy to project's bin all DLLs.

# Resolving compiling problems

1. Add to the code this directive:
#define _CRT_SECURE_NO_WARNINGS

2. There are may problems with 'std - ambiguous symbol'.
To resolve, just  delete "std::" there. 

In в OrderedList, replace 'std::pair'  by ::pair.

3. In Half.h
replace

        (f < 0 && -static_cast<uint64_t>(f) > limit::max())

by

        (f < 0 && -static_cast<int64_t>(f) > limit::max());

