#include "ofApp.h"

using namespace ofxCv;
using namespace cv;


void ofApp::setup() {
    ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetVerticalSync(true);
    bool bUseCamera = true;
    cam.setDeviceID(0);
    cam.setDesiredFrameRate(30);
    camWidth =  1280;
    camHeight = 720;
    cam.initGrabber(camWidth, camHeight);
    
    bPaused = false;
    ofSetWindowShape(camWidth, camHeight);
    
    
    
    tracker.setRescale(1.);
    tracker.setIterations(5);
    tracker.setClamp(3.);
    tracker.setTolerance(0.01);
    tracker.setAttempts(1);
    bDrawMesh = true;
    
    tracker.setup();
    host = "localhost";
    port = 8338;
    osc.setup(host, port);  ofBackground(0);
    
    //faceBuffer
    for (int i =0 ; i < 30 ; i++) {
        facebuffer[i].allocate(300, 300, OF_IMAGE_COLOR);
    }
    faceCounter = 0;
    
    cMode = 0;
    
}

void ofApp::update() {
	if(bPaused)
		return;

	cam.update();

    if(cam.isFrameNew()) {
		tracker.update(toCv(cam));
        
        rotationMatrix = tracker.getRotationMatrix();

        pastMouth[0].set(tracker.getPosition().x,
                         tracker.getPosition().y + 10*tracker.getScale(),
                         tracker.getGesture(ofxFaceTracker::MOUTH_WIDTH)*tracker.getScale(),
                         tracker.getGesture(ofxFaceTracker::MOUTH_HEIGHT)*tracker.getScale());
        
        ofPoint bpoint = ofPoint(pastMouth[0].x,pastMouth[0].y);
        
        if(pastMouth[0].w > 10){
            
            
            facePosition = ofPoint(tracker.getPosition().x -  20*tracker.getScale(),tracker.getPosition().y-20*tracker.getScale());
            faceSize = 40*tracker.getScale();
        
            if(tracker.getFound()){
                
                facebuffer[faceCounter%30].setFromPixels(cam.getPixels());
                facebuffer[faceCounter%30].crop(facePosition.x,facePosition.y, faceSize,faceSize);
                facebuffer[faceCounter%30].resize(300, 300);
                faceCounter++;
            }
            
        }
        
	}
    
    
}

void ofApp::draw() {
	ofSetColor(255,255,255,255);
	cam.draw(0, 0);

	if(tracker.getFound()) {
        for (int i= 29 ; i >= 0; i--) {
            if(cMode%3 == 0 ){
                facebuffer[(i+faceCounter)%30].drawSubsection(facePosition.x,facePosition.y+((faceSize/30)*i) , faceSize, faceSize/30, 0, 10*i,300,10);
            }else if(cMode%3 == 1){
                facebuffer[(i+faceCounter)%30].drawSubsection(facePosition.x+((faceSize/30)*i),facePosition.y , faceSize/30, faceSize, 10*i, 0,10,300);
                
                
                
            }else if(cMode%3 == 2){
                
                facebuffer[(i+faceCounter)%30].drawSubsection(facePosition.x+((faceSize/5)*(i%5)),facePosition.y+((faceSize/6)*(int(i/6))) , faceSize/5, faceSize/6, 60*(i%5), 50*(int(i/6)),60,50);
                
            }
            
            
        }
       // facebuffer[0].draw(facePosition.x,facePosition.y);
        ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
		if(bDrawMesh) {
		}
	} else {
		ofDrawBitmapString("searching for face...", 10, 20);
	}
    
	if(bPaused) {
		ofSetColor(255, 0, 0);
		ofDrawBitmapString( "paused", 10, 32);
	}
    
    
    
}



void ofApp::keyPressed(int key) {
	switch(key) {
		case 'r':
			tracker.reset();
			break;
		case 'm':
			bDrawMesh = !bDrawMesh;
			break;
            
        case 'p':
			bPaused = !bPaused;
			break;
            
        case 'c':
            cMode++;
            break;
            
	}
}

