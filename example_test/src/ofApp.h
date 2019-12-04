#pragma once

/* 
example_test - minimalistic example to check PyTorch is linked properly. 
It's based on https://pytorch.org/cppdocs/installing.html#minimal-example
The example creates a random tensor, prints it in Console and saves to **data** folder as **saved.tensor** file.
This example demonstrates the way for using files made in oF project in your Python's Pytorch code, 
and back, use your Python's Pytorch data in the oF project!
(Note, actually **saved.tensor** is ZIP archive, which you can explore).
*/

#include "ofMain.h"

#include "ofxPytorch.h"



class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);


};
