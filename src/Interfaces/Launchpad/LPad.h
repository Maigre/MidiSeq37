#include "../../Sequencer/Sequencer.h"
#include "../../Sequencer/Track.h"
#include "ofxMidi.h"
#include "LPstate.h"
#include "Mode_abstract.h"


/*
LAUNCHPAD ONE class
*/

class LPad: public ofxMidiListener {

  public:
    LPad(LPstate* s, Mode_abstract** m, char outport, uint n);
    void draw();

  private:

    char colorRG(char red, char green);
    void newMidiMessage(ofxMidiMessage& eventArgs);

    char matrixOUT[8][8];
    char extraBtnsOUT[2][8];

    LPstate* state;
    Mode_abstract** modes;
    ofxMidiOut padOut;
    ofxMidiIn padIn;

    uint offset;
};
