#include "ofApp.h"
#include "conf.h"
#include "ofxMidi.h"
#include "Memory/MemFiles.h"

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

  // Create Sequencer
  sequencer = new Sequencer(portout);

  // Load Mem 0
  sequencer->load(0);

  // Start
  sequencer->start();

  // Add beat on first track
  // for (int k=0; k < (RESOLUTION*4); k+=RESOLUTION)
  //   sequencer->track(1)->pattern(1)->addNote(k, 36, RESOLUTION/4-1);

  launchpad = new Launchpad(sequencer);
}

//--------------------------------------------------------------
void ofApp::update() {
  counter = (counter+1) % (60*FRAMERATE);

  // Perfomance
  if (counter % (10*FRAMERATE) == 0) sequencer->progress();

  // auto save
  if (AUTOSAVE)
    if (counter % (5*FRAMERATE) == 0) sequencer->save();
}

//--------------------------------------------------------------
void ofApp::draw(){
  launchpad->draw();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  // if (key == 's') sequencer->save(0);
  //else if (key == 'l') sequencer->load(0);
  if (key == 'a') MemFiles::List();
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
