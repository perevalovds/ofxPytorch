
#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
	cout << "Example using ofxPytorch" << endl;

	//at::Tensor a = at::ones({ 2, 2 }, at::kInt);
	//at::Tensor b = at::randn({ 2, 2 });
	//auto c = a + b.to(at::kInt);
	//std::cout << c << std::endl;

	torch::Tensor tensor = torch::rand({ 2, 3 });
	cout << "Tensor:" << endl;
	cout << tensor << std::endl;

	string file_name = "saved.tensor";
	torch::save(tensor, ofToDataPath(file_name));

	cout << endl;
	cout << "PyTorch works well, the created tensor was saved to '" << file_name << "'" << endl;

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
