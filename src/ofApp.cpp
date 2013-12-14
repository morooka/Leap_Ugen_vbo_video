#include "ofApp.h"

const int ofApp::WIDTH;
const int ofApp::HEIGHT;
const int ofApp::NUM_PARTICLES;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetDataPathRoot(getResourcesPath() + "/Contents/Resources/");
    controller.setPolicyFlags(Controller::POLICY_BACKGROUND_FRAMES );
    
    ofBackground(0, 0, 0);
    ofEnableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    myVideo.initGrabber(640,480);
    
    bFixedPosition = false;
    bSound = true;
    ofDisableArbTex();
    
    ofxUGen::s().setup();
}
//--------------------------------------------------------------
void ofApp::update(){
    myVideo.update();
    
    count = count + 0.08f;
    
    if (myVideo.isFrameNew()) {
        unsigned char * pixels = myVideo.getPixels();
        for (int i = 0; i < WIDTH; i++) {
            for (int j = 0; j < HEIGHT; j++) {
                float r = (float)pixels[j * myVideo.width * 3 + i * 3] /256.0;
                float g = (float)pixels[j * myVideo.width * 3 + i * 3 + 1] /256.0;
                float b = (float)pixels[j * myVideo.width * 3 + i * 3 + 2] /256.0;
                
                float brightness = (r + g + b) / 3.0f ;
        
                myColor[j * WIDTH + i] = ofFloatColor(r,g,b,1.0);
                if(bFixedPosition == false)
                myVerts[j * WIDTH +i] = ofVec3f(i-WIDTH/2, j-HEIGHT/2,brightness*finger.tipPosition().z*10);
                else myVerts[j * WIDTH +i] = ofVec3f(i-WIDTH/2, j-HEIGHT/2,0);
            }
        }
        myVbo.setVertexData(myVerts,NUM_PARTICLES,GL_DYNAMIC_DRAW_ARB);
        myVbo.setColorData(myColor,NUM_PARTICLES,GL_DYNAMIC_DRAW_ARB);
    }
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    glEnable(GL_DEPTH_TEST);
    
    if (((hand2.fingers().count() == 4) || (hand2.fingers().count() == 5)) && bSound){
        int r = ofMap(finger.tipPosition().y, 50, 700, 0, 255);
        int g = ofMap((unsigned)finger.tipPosition().z, 0, 180, 0, 255);
        int b = ofMap((unsigned)finger.tipPosition().x, 0, 300, 0, 255);
        ofBackground(r,g,b);
    }else ofBackground(0);
    
    cam.begin();
    ofScale(1,-1,1);
    
    float handPalmX =  hand.palmNormal().x;
    float handPalmZ =  hand.palmNormal().z;
    
    ofRotateZ(handPalmX*-90);
    ofRotateY(handPalmZ*180);
    
    glPointSize(1);
    
    myVbo.draw(GL_POINTS,0,NUM_PARTICLES);
    
    if (bSound) {
        if((hand2.fingers().count() == 0)){
            if ((hand.fingers().count() == 1) || (hand.fingers().count() == 5)) rotateSinewaveDraw();
            if ((hand.fingers().count() == 2) || (hand.fingers().count() == 5)) rotateBassDraw();
            if ((hand.fingers().count() == 3) || (hand.fingers().count() == 5)) whiteNoiseDraw();
            if ((hand.fingers().count() == 4) || (hand.fingers().count() == 5)) myLFSawDraw();
        }
        if ((hand2.fingers().count() == 3) || (hand2.fingers().count() == 5)) myLFPulseDraw();
        if ((hand2.fingers().count() == 2) || (hand2.fingers().count() == 5)) {
            myHarmonicOscDraw();
            glPushMatrix();
            ofRotateY(360*(count/2));
            glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
            myVbo.draw(GL_POINTS,0,NUM_PARTICLES);
            glPopMatrix();
        }
        if ((hand2.fingers().count() == 1) || (hand2.fingers().count() == 5)) {
            glPushMatrix();
            ofTranslate(-WIDTH/2,-HEIGHT/2,-400);
            dustDraw();
            glPopMatrix();
        }
    }
    
    cam.end();

    // フレームを取得
    Frame frame = controller.frame();
    
    for(int i=0; i<frame.hands().count(); i++) {
        hand = frame.hands()[0];
        hand2 = frame.hands()[1];
        for(int j=0; j<hand.fingers().count() + hand2.fingers().count(); j++) {
            finger = frame.fingers()[j];
        }
    }
    
    ofPopMatrix();
    
    for (int i = 0; i < rotateSinewaves.size(); i++) rotateSinewaves[i]->release();
    for (int i = 0; i < bassSounds.size(); i++)      bassSounds[i]->release();
    for (int i = 0; i < whiteNoises.size(); i++)     whiteNoises[i]->release();
    for (int i = 0; i < myLFSaws.size(); i++)        myLFSaws[i]->release();
    for (int i = 0; i < myLFPulses.size();i++)       myLFPulses[i]->release();
    for (int i = 0; i < myHarmonicOsces.size();i++)  myHarmonicOsces[i]->release();
    for (int i = 0; i < dusts.size();i++)            dusts[i]->release();
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'f'){
        ofToggleFullscreen();
    }
    if(key == 'v'){
        myVideo.videoSettings();
    }
    if (key == 's') {
        bSound = !bSound;
    }
    if (key == ' ') {
        bFixedPosition = !bFixedPosition;
    }
}
//--------------------------------------------------------------
void ofApp::rotateSinewaveDraw(){
    deque<rotateSinewave*>::iterator it1 = rotateSinewaves.begin();
    while (it1 != rotateSinewaves.end())
    {
        rotateSinewave *s = *it1;
        s->rotation = 200;
        s->color1=(ofVec3f(253,217,217));
        s->color2=(ofVec3f(220,214,217));
        s->color3=(ofVec3f(253,217,217));
        s->radius =50;
        for(int i = 0; i < NUM_PARTICLES; i+=3000){
        s->draw(myVerts[i].x,myVerts[i].y,myVerts[i].z);
        }
        if (!s->isAlive())
        {
            it1 = rotateSinewaves.erase(it1);
            delete s;
        }
        else
            it1++;
    }
        rotateSinewaves.push_back(new rotateSinewave(finger.tipPosition().x,finger.tipPosition().y,finger.tipPosition().z,70,4000,10000));

    while (rotateSinewaves.size() > 1) {
        rotateSinewaves.pop_front();
    }
    rotateSinewaves.back()->play();
    
}
//--------------------------------------------------------------
void ofApp::rotateBassDraw(){
    deque<rotateBass*>::iterator it2 = bassSounds.begin();
    while (it2 != bassSounds.end())
    {
        rotateBass *s = *it2;
        
        for(int i = 0; i < NUM_PARTICLES; i+=4000){
        s->draw(myVerts[i].x,myVerts[i].y,myVerts[i].z);
        }
        
        if (!s->isAlive())
        {
            it2 = bassSounds.erase(it2);
            delete s;
        }
        else
            it2++;
    }
    bassSounds.push_back(new rotateBass(finger.tipPosition().x,finger.tipPosition().y,finger.tipPosition().z,300,500,true,200));
    
    while (bassSounds.size() > 1) {
        bassSounds.pop_front();
    }
    bassSounds.back()->play();
    
}
//--------------------------------------------------------------
void ofApp::whiteNoiseDraw(){
    deque<whiteNoise*>::iterator it3 = whiteNoises.begin();
    while (it3 != whiteNoises.end())
    {
        whiteNoise *s = *it3;
        
        for(int i = 0; i < NUM_PARTICLES; i+=15000){
        s->draw(myVerts[i].x,myVerts[i].y,myVerts[i].z);
        }
        
        if (!s->isAlive())
        {
            it3 = whiteNoises.erase(it3);
            delete s;
        }
        else
            it3++;
    }
    whiteNoises.push_back(new whiteNoise(finger.tipPosition().x,finger.tipPosition().y,finger.tipPosition().y));
    while (whiteNoises.size() > 1) {
        whiteNoises.pop_front();
    }
    whiteNoises.back()->play();
}
//--------------------------------------------------------------
void ofApp::myLFSawDraw(){
    deque<myLFSaw*>::iterator it7 = myLFSaws.begin();
    while (it7 != myLFSaws.end())
    {
        myLFSaw *s = *it7;
        for(int i = 0; i < NUM_PARTICLES; i+=10000){
            s->draw(myVerts[i].x,myVerts[i].y,myVerts[i].z,finger.tipPosition().x,(400-finger.tipPosition().y),finger.tipPosition().z);
        }
        
        if (!s->isAlive())
        {
            it7 = myLFSaws.erase(it7);
            delete s;
        }
        else
            it7++;
    }
    myLFSaws.push_back(new myLFSaw(ofRandom(ofGetWidth()),ofRandom(ofGetHeight()),0));
    while (myLFSaws.size() >1) {
        myLFSaws.pop_front();
    }
    myLFSaws.back()->play();
}
//--------------------------------------------------------------
void ofApp::myLFPulseDraw(){
    deque<myLFPulse*>::iterator it3 = myLFPulses.begin();
    while (it3 != myLFPulses.end())
    {
        myLFPulse *s = *it3;
        s->draw(myVerts[NUM_PARTICLES/2+WIDTH/2].x,myVerts[NUM_PARTICLES/2].y,myVerts[NUM_PARTICLES/2].z);
        
        if (!s->isAlive())
        {
            it3 = myLFPulses.erase(it3);
            delete s;
        }
        else
            it3++;
    }
    myLFPulses.push_back(new myLFPulse(finger.tipPosition().x,finger.tipPosition().y,finger.tipPosition().y));
    while (myLFPulses.size() > 1) {
        myLFPulses.pop_front();
    }
    myLFPulses.back()->play();
}
//--------------------------------------------------------------
void ofApp::myHarmonicOscDraw(){
    deque<myHarmonicOsc*>::iterator it4 = myHarmonicOsces.begin();   //*ofGetWidth()/320  *ofGetHeight()/240
    while (it4 != myHarmonicOsces.end())
    {
        myHarmonicOsc *s = *it4;
        
        if (!s->isAlive())
        {
            it4 = myHarmonicOsces.erase(it4);
            delete s;
        }
        else
            it4++;
    }
    myHarmonicOsces.push_back(new myHarmonicOsc(finger.tipPosition().x,finger.tipPosition().y,finger.tipPosition().y,true));
    while (myHarmonicOsces.size() > 1) {
        myHarmonicOsces.pop_front();
    }
    myHarmonicOsces.back()->play();
}
//--------------------------------------------------------------
void ofApp::dustDraw(){
    deque<dust*>::iterator it4 = dusts.begin();
    while (it4 != dusts.end())
    {
        dust *s = *it4;
        for (int i = 0; i < 800; i += 100) {
            for (int j =0 ; j < 800; j += 100) {
                for (int k = 0; k < 800; k += 100) {
                     s->draw(i,j,k);
                }
            }
        }
        if (!s->isAlive())
        {
            it4 = dusts.erase(it4);
            delete s;
        }
        else
            it4++;
    }
    dusts.push_back(new dust(finger.tipPosition().x,finger.tipPosition().y));
    while (dusts.size() > 1) {
        dusts.pop_front();
    }
    dusts.back()->play();
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}
//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}
//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}
//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
}
//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
}