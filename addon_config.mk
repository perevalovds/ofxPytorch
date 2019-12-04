meta:
	ADDON_NAME = ofxPytorch
	ADDON_DESCRIPTION = openFrameworks addon for working with Pytorch (using Libtorch C++ library)
	ADDON_AUTHOR = Denis Perevalov
	ADDON_TAGS = "Machine Learning"
	ADDON_URL = https://github.com/perevalovds/ofxPytorch

common:

	ADDON_INCLUDES_EXCLUDE += libs
	ADDON_SOURCES_EXCLUDE += libs
	ADDON_INCLUDES +=  libs/libtorch/include
	ADDON_INCLUDES +=  libs/libtorch/include/torch/csrc/api/include

osx:
vs:	
	# x64
	ADDON_LIBS += libs/libtorch/lib/caffe2.lib
	ADDON_LIBS += libs/libtorch/lib/c10.lib  
	ADDON_LIBS += libs/libtorch/lib/torch.lib  

	ADDON_DLLS_TO_COPY += libs/libtorch/lib/c10.dll
	#ADDON_DLLS_TO_COPY += libs/libtorch/lib/caffe2_detectron_ops.dll
	#ADDON_DLLS_TO_COPY += libs/libtorch/lib/caffe2_module_test_dynamic.dll
	ADDON_DLLS_TO_COPY += libs/libtorch/lib/libiomp5md.dll
	ADDON_DLLS_TO_COPY += libs/libtorch/lib/libiompstubs5md.dll
	ADDON_DLLS_TO_COPY += libs/libtorch/lib/torch.dll
	
linux64:
linuxarmv6l:
linuxarmv7l:
msys2:
