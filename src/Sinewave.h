//
//  Sinewave.h
//  box2d_UGen_Leap
//
//  Created by 諸岡 光男 on 2013/11/01.
//
//
#pragma once

#include "ofxUGen.h"

using namespace ofxUGen;


class Sinewave : public ofxUGen::SynthDef
{
public:
	
	UGen envgen;
	UGen amp;
    
	Sinewave(int * freq,float * pan)
	{
        
		Env env = Env::perc(0.1,0.2,0.1, EnvCurve::Numerical);  //Sine
		envgen = EnvGen::AR(env);
		envgen.addDoneActionReceiver(this);
		
        // amp =SinOsc::AR(ofRandom(4.0), 0, 0.4, 0.4) *  envgen;
        amp =  SinOsc::AR(0, 0, 0.1, 0.1);
        
        Out(
            Pan2::AR(SinOsc::AR(freq) * amp, pan)
            );
	}
	void draw(int x,int y)
	{
        
        ofFill();
        ofSetColor(51,255,153,200);
        ofCircle(x, y, 35*amp.getValue()*ofRandom(0.1,10.0));
    }
    bool isAlive()
	{
		return !Out().isNull();
	}
};

