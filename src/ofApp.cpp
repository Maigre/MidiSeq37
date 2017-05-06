#include "ofApp.h"
#include "conf.h"
#include "ofxMidi.h"

//--------------------------------------------------------------
void ofApp::setup(){

  if (FRAMERATE > 60) ofSetVerticalSync(false);
  ofSetFrameRate(FRAMERATE);

  // LIST midi port
  ofxMidiOut::listPorts();

  // Find Midi OUT
  uint portout=0;
  for (char port=0; port<ofxMidiOut::getNumPorts(); port++)
    if (ofxMidiOut::getPortName(port).find(MIDI_OUT_NAME) != string::npos) {
      portout = port;
      break;
    }

  sequencer = new Sequencer(16, portout);
  sequencer->start();

  for (int k=0; k < (RESOLUTION*4); k+=RESOLUTION)
    sequencer->track(1)->addNote(k, 36, RESOLUTION-1);

  launchpad = new Launchpad(sequencer);
}

//--------------------------------------------------------------
void ofApp::update() {
  //sequencer->progress();
}

//--------------------------------------------------------------
void ofApp::draw(){
  launchpad->draw();
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
