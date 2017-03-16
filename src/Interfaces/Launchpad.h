#include "Track.h"
#include "ofxOsc.h"

class Launchpad {

  public:
    Launchpad(int _portOut, int _portIn);
    void update(Track* track);

  private:

    void send();
    string _smatrix;

    char matrix[8][8][2];
    ofxOscSender sender;

};
