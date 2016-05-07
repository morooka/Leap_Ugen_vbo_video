#pragma once

#include "ofMain.h"
#include "Leap.h"
#include "ofxUGen.h"

#include "Sinewave.h"
#include "rotateSinewave.h"
#include "whiteNoise.h"
#include "rotateBass.h"
#include "myLFSaw.h"
#include "myLFPulse.h"
#include "myHarmonicOsc.h"
#include "bass.h"
#include "dust.h"


using namespace Leap;

class ofApp : public ofBaseApp{
    
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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void exit();
    
    // Leap Motionのコントローラー
    Leap::Controller controller;
    Leap::Finger finger;
    Leap::Hand hand,hand2;
     
    ofEasyCam cam;

    static const int WIDTH = 640;
    static const int HEIGHT = 480;
    static const int NUM_PARTICLES = WIDTH * HEIGHT;
    
    ofVec3f myVerts[NUM_PARTICLES];
    ofVec3f pointVerts[NUM_PARTICLES];

    ofFloatColor myColor[NUM_PARTICLES];
    ofFloatColor pointColor[NUM_PARTICLES];

    ofVideoGrabber myVideo;
    
    ofVbo myVbo;
    ofVbo pointVbo;
    
    float count;
    
    bool bFixedPosition,bSound;
    
    deque <Sinewave*>       sinewaves;
    deque <rotateSinewave*> rotateSinewaves;
    deque <rotateBass*>     bassSounds;
    deque <whiteNoise*>     whiteNoises;
    deque <myLFSaw*>        myLFSaws;
    deque <myLFPulse*>      myLFPulses;
    deque <myHarmonicOsc*>  myHarmonicOsces;
    deque <bass*>           basses;
    deque <dust*>           dusts;
   
    void rotateSinewaveDraw();
    void rotateBassDraw();
    void whiteNoiseDraw();
    void myLFSawDraw();
    void myLFPulseDraw();
    void myHarmonicOscDraw();
    void dustDraw();
    
};
