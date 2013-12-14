//
//  myHarmonicOsc.h
//  box2d_UGen_Leap
//
//  Created by 諸岡 光男 on 2013/11/02.
//
//
#pragma once

#include "ofxUGen.h"

class myHarmonicOsc : public ofxUGen::SynthDef
{
public:
	
	ofVec3f pos;
	
	UGen envgen;
	UGen amp;
    
    float count;
    int rotation,radius;
    
    ofVec3f enMove1,enMove2,enMove3;
    
	myHarmonicOsc(int x,int y,int z,bool choise)
	{
		pos.x = x;
		pos.y = y;
        pos.z = z;
		
		float freq = ofMap(pos.y, 0, ofGetHeight(), 50, 150);
		float pan = ofMap(pos.x, 0, ofGetWidth(), -1, 1);
        float volume = ofMap(pos.z,50, 700, 300, 1);
        
		Env env = Env::perc(0.6, 0.5, 0.5, EnvCurve::Welch);  //sine // Numerical :Welch
		envgen = EnvGen::AR(env);
		//envgen.addDoneActionReceiver(this);
		
		//amp = LFPulse::AR() * envgen;   //SinOsc::AR(ofRandom(4,0), 0, 0.5, 0.5) FSinOsc SinOsc LFPulse
        if (choise) {
            amp = SinOsc() * envgen;    //0, 0, 0.5, 0.5
        }else amp = LFPulse::AR() * envgen;
        
        Out(
            Pan2::AR(HarmonicOsc::AR(Buffer(1.0, 0.5, 0.333, 0.25, 0.2, 0.167, 0.143, 0.125),freq)*amp*volume/500, pan)
            );
    }
    
    void draw(int x1,int y1,int z1)
	{
        count = count + 0.2f*amp.getValue()*10;
        
        enMove1.x = std::cos(count)*rotation*amp.getValue();    //rotation =2000
        enMove1.y = std::sin(count)*rotation*amp.getValue();
        enMove1.z = std::tan(count)*rotation*amp.getValue();
        
        enMove2.x = std::cos(count+400)*rotation*amp.getValue();
        enMove2.y = std::sin(count+400)*rotation*amp.getValue();
        enMove2.z = std::tan(count+400)*rotation*amp.getValue();
        
        enMove3.x = std::cos(count+800)*rotation*amp.getValue();
        enMove3.y = std::sin(count+800)*rotation*amp.getValue();
        enMove3.z = std::tan(count+800)*rotation*amp.getValue();
        
        ofSetColor(255,255,204);
        ofLine(x1,y1,z1,enMove1.x,enMove1.y,enMove1.z);
        ofSetColor(255,255,153);
        ofLine(x1,y1,z1,enMove2.x,enMove2.y,enMove2.z);
        ofSetColor(255,255,102);
        ofLine(x1,y1,z1,enMove3.x,enMove3.y,enMove3.z);
        
    }

};
