//
//  whiteNoise.h
//  box2d_UGen_Leap
//
//  Created by 諸岡 光男 on 2013/11/02.
//
//
#pragma once

#include "ofxUGen.h"
#include <iostream>

class whiteNoise : public ofxUGen::SynthDef
{
public:
	
	ofVec3f pos;
	
	UGen envgen;
	UGen amp;
    
    
    ofTexture tex;
    int tex_width;
    int tex_height;
    
    float count;
    
    ofVec2f enMove1;
    
    
	whiteNoise(int x,int y,int z)
	{
        
        pos.x = x;
        pos.y = y;
        pos.z = z;
        
		float freq = ofMap(pos.y,0, ofGetHeight(), 1000,10000);
		float pan = ofMap(pos.x, 0,ofGetWidth(), -1, 1);
        float volume = ofMap(pos.z,50, 700, 600, 1);
        
		Env env = Env::perc(0.1,0.1,0.5, EnvCurve::Sine);
		envgen = EnvGen::AR(env);
		envgen.addDoneActionReceiver(this);
		
        amp =SinOsc::AR(1, 0, 0.5, 0.5) *  envgen;
        
        Out(
            Pan2::AR(WhiteNoise::AR(freq/5) * amp/1800, pan)
            );
    }
	
	void draw(int x,int y,int z)
    {
        tex_width = 5;
        tex_height = 50;
        tex.allocate(tex_width,tex_height,GL_RGB);
        
        unsigned char pixels[tex_width*tex_height*3];
        
        for (int i = 0; i < tex_width*tex_height*3; i++)
        {
            pixels[i] = (int)(255 * ofRandomuf());
        }
        tex.loadData(pixels, tex_width,tex_height, GL_RGB);
        
        count = count + 0.2f*amp.getValue()*10;
        
        enMove1.x = std::cos(count)*200*amp.getValue();
        enMove1.y = std::sin(count)*200*amp.getValue();
        
        
        
        // ofSetColor(255,210+ofRandom(-100,100));
        ofSetColor(255);
        tex.draw(x+enMove1.x,y+enMove1.y,z);
    }
    
    bool isAlive()
	{
		return !Out().isNull();
	}
};
