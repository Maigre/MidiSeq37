#include "../Sequencer/Sequencer.h"
#include "../Sequencer/Track.h"
#include "ofxMidi.h"
#include "LPstate.h"
#include "Mode_base.h"

#define COLOR_OFF         12
#define COLOR_RED_LOW     13
#define COLOR_RED         15
#define COLOR_AMBER_LOW   29
#define COLOR_AMBER       63
#define COLOR_YELLOW      62
#define COLOR_GREEN_LOW   28
#define COLOR_GREEN       60

/*
LAUNCHPAD ONE class
*/

class LPad: public ofxMidiListener {

  public:
    LPad(LPstate* s, Mode_base** m, char outport, int n);
    void draw();

  private:

    char colorRG(char red, char green);
    void newMidiMessage(ofxMidiMessage& eventArgs);

    char matrixOUT[8][8];
    char extraBtnsOUT[2][8];

    LPstate* state;
    Mode_base** modes;
    ofxMidiOut padOut;
    ofxMidiIn padIn;

    int offset;
};
