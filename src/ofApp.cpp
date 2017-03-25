#include "ofApp.h"
#include "conf.h"

//--------------------------------------------------------------
void ofApp::setup(){

  if (FRAMERATE > 60) ofSetVerticalSync(false);
  ofSetFrameRate(FRAMERATE);

  sequencer = new Sequencer(2);
  sequencer->start();

  for (int k=0; k < (RESOLUTION*4*4); k+=RESOLUTION)
    sequencer->track(1)->addNote(k, 48, RESOLUTION-1);

  launchpad1 = new Launchpad(sequencer, 0);
  launchpad2 = new Launchpad(sequencer, 1);
}

//--------------------------------------------------------------
void ofApp::update() {
  //sequencer->progress();
}

//--------------------------------------------------------------
void ofApp::draw(){
  launchpad1->draw();
  launchpad2->draw();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------6
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
