#include "../Sequencer/Sequencer.h"
#include "../Sequencer/Track.h"
#include "ofxMidi.h"

#define COLOR_OFF         12
#define COLOR_RED_LOW     13
#define COLOR_RED         15
#define COLOR_AMBER_LOW   29
#define COLOR_AMBER       63
#define COLOR_YELLOW      62
#define COLOR_GREEN_LOW   28
#define COLOR_GREEN       60

struct LpState {
  char offset;
  char mode;
  char track;
  char page;
  char zoom;
  int note;
};

class Launchpad: public ofxMidiListener {

  public:
    Launchpad(Sequencer* seq, char n);
    void draw();
    void clear();

    void add_step(int step, int note);

    void draw_steps();

  private:

    void push();
    char colorRG(char red, char green);
    void newMidiMessage(ofxMidiMessage& eventArgs);

    LpState state;

    char matrix[8][8];
    char matrixOUT[8][8];

    Sequencer* sequencer;
    ofxMidiOut padOut;
    ofxMidiIn padIn;
};
