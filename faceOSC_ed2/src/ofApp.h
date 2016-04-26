#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "FaceOsc.h"
#include "ofxXmlSettings.h"
#include "ofxOsc.h"
#include "ofxBox2d.h"


class ofApp : public ofBaseApp, public FaceOsc {
public:

	void setup();
	void update();
	void draw();
    
	void keyPressed(int key);
	bool bUseCamera, bPaused;

	int camWidth, camHeight;
	int movieWidth, movieHeight;
	int sourceWidth, sourceHeight;

	ofVideoGrabber cam;
    
	ofxFaceTracker tracker;
	ofMatrix4x4 rotationMatrix;
    
    
    ofMesh customMesh;
    ofVec4f pastMouth[100];
    
    vector<ofVec3f> faceCircle;
    
    

	bool bDrawMesh;
    
    ofImage facebuffer[30];
    int faceCounter;
    ofPoint facePosition;
    float faceSize;
    
    int cMode;
    enum MODE{VER,HOR,BLO};

};
