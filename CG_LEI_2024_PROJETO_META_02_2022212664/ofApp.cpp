#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    //fundo branco;
    ofBackground(0, 0, 0);

    movement = new Game();

}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle("Space Invaders Proj running @ " + ofToString(int(ofGetFrameRate())));
    movement->update();

}

//--------------------------------------------------------------
void ofApp::draw(){
    movement->draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
    delete movement;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    movement->keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    movement->keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
