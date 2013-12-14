//
//  bass.h
//  box2d_UGen_Leap
//
//  Created by 諸岡 光男 on 2013/11/02.
//
//
#pragma once

#include "ofxUGen.h"

class bass : public ofxUGen::SynthDef
{
public:
   	
	UGen envgen;
	UGen amp;
    
    float volume,count;
    
    ofVec3f pos ;
    ofVec2f enMove1,enMove2,enMove3;
    
    bass(int * freq,float * pan)
    {
        /*
         float freq = ofMap(pos.y,0, ofGetHeight(), freq1,freq2);      //80, 400
        float pan = ofMap(pos.x, 0,ofGetWidth(), -1, 1);
        float volume = ofMap(pos.z,50, 700, 200, 1);
         */
        
		Env env = Env::perc(0.3, 0.3, 0.5, EnvCurve::Sine);
		envgen = EnvGen::AR(env);
		envgen.addDoneActionReceiver(this);
		
        
         amp = SinOsc::AR(ofRandom(4.0), 0, 0.3, 0.3);
		
		Out(
			Pan2::AR(SinOsc::AR(freq) * amp , pan)      //volume2 = 100
            );
    }
    
    void draw(int x,int y)
    {
        ofFill();
        ofSetColor(100,100,100,200);
        ofCircle(x, y, 35*amp.getValue()*ofRandom(0.1,10.0));
    }

    
    bool isAlive()
	{
		return !Out().isNull();
	}
};