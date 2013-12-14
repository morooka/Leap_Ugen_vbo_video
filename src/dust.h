//
//  dust.h
//  Leap_Ugen_vbo
//
//  Created by 諸岡 光男 on 2013/12/12.
//
//
#pragma once

#include "ofxUGen.h"

class dust : public ofxUGen::SynthDef
{
public:
	
	ofVec2f pos;
	
	UGen envgen;
	UGen amp;
    
    ofTexture tex;
    
    ofImage img;
    
	dust(int x,int y)
	{
        pos.x = x;
        pos.y = y;
        
		float freq = ofMap(pos.y, 0, ofGetHeight(), 100,20000);
		float pan = ofMap(pos.x, 200, ofGetWidth()-200, -1, 1);
        
		Env env = Env::perc(0.1,0.2,1.0, EnvCurve::Sine);
		envgen = EnvGen::AR(env);
		envgen.addDoneActionReceiver(this);
		
        amp =SinOsc::AR(5, 0, 1.0, 1.0) *  envgen;
        
        Out(
            Pan2::AR(Dust2::AR(5,1,0) * amp*3, pan)
            );
    }
    
	void draw(int x,int y,int z)
	{
        ofSetColor(255);
        float aaa = ofRandom(0.5, 1.5);
        ofCircle(x*aaa, y*aaa,z*aaa,1);
    }
 
    
	
	bool isAlive()
	{
		return !Out().isNull();
	}
};

