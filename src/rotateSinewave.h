//
//  rotateSinewave.h
//  box2d_UGen_Leap
//
//  Created by 諸岡 光男 on 2013/11/02.
//
//
#pragma once

#include "ofxUGen.h"

using namespace ofxUGen;

class rotateSinewave : public ofxUGen::SynthDef
{
public:
	
	ofVec3f color1,color2,color3,pos;
    ofVec2f circlePosition;
	
	UGen envgen;
	UGen amp;
    
    float count;
    
    ofVec2f enMove1,enMove2,enMove3;
    
    int rotation,radius;
    
    rotateSinewave(int x,int y,int z,int volume2,int freq1,int freq2)
    {
        pos.x = x;
        pos.y = y;
        pos.z = z;
    
        float freq = ofMap(pos.y,0, ofGetHeight(),freq1,freq2);   //freq1 = 6000,freq2 = 12000
        float pan = ofMap(pos.x, 0,ofGetWidth(), -1, 1);
        float volume = ofMap(pos.z,50, 700, 300, 1);
    
        Env env = Env::perc(0.1,0.2,0.1, EnvCurve::Numerical);  //Sine
        envgen = EnvGen::AR(env);
        envgen.addDoneActionReceiver(this);
    
        amp =SinOsc::AR(ofRandom(4.0), 0, 0.4, 0.4) *  envgen;
        Out(
            Pan2::AR(SinOsc::AR(freq) * amp*volume/volume2, pan)    //volume2=100
            );
    }
    
    void draw(int cF1X,int cF1Y,int z)
	{
        count = count + 0.2f*amp.getValue()*10;
        
        enMove1.x = std::cos(count)*rotation*amp.getValue();    //rotation =2000
        enMove1.y = std::sin(count)*rotation*amp.getValue();
        
        enMove2.x = std::cos(count+400)*rotation*amp.getValue();
        enMove2.y = std::sin(count+400)*rotation*amp.getValue();
        
        enMove3.x = std::cos(count+800)*rotation*amp.getValue();
        enMove3.y = std::sin(count+800)*rotation*amp.getValue();
       
        
        
		ofFill();
		ofSetColor(color1.x,color1.y,color1.z+150);                             //253,217,217
        ofCircle(cF1X+enMove1.x,cF1Y+enMove1.y,z,radius*amp.getValue());   //radius = 200;
        ofSetColor(color1.x,color1.y,color1.z+150);                             //220,214,217
        ofCircle(cF1X+enMove2.x,cF1Y+enMove2.y,z,radius*amp.getValue());
        ofSetColor(color1.x,color1.y,color1.z+150);                             //253,217,217
        ofCircle(cF1X+enMove3.x,cF1Y+enMove3.y,z,radius*amp.getValue());
	}
    
    bool isAlive()
	{
		return !Out().isNull();
	}
};

