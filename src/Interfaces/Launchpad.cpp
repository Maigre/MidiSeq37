#include "Launchpad.h"
#include "../conf.h"
#include "../Sequencer/MMidiEvent.h"

Launchpad::Launchpad(int _portOut, int _portIn) {

  sender.setup("localhost", _portOut);

  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++) {
        matrix[x][y][0] = 1;
        matrix[x][y][1] = 1;
    }
}

void Launchpad::update(Track* track) {

  int step = (track->clock()->beat()-1);

  for (int x = 0; x < 8; x++) {
    std::vector<MMidiNote*> notes = track->getNotes(x*RESOLUTION, RESOLUTION);

    for (int y = 0; y < 8; y++) {
        if (x == step%8) matrix[x][y][0] = 1;
        else matrix[x][y][0] = 0;

        matrix[x][y][1] = 0;
        for (int k=0; k<notes.size(); k++)
          if (notes[k]->note == y+43) matrix[x][y][1] = 1;
    }
  }
  send();
}

void Launchpad::send() {

  string smatrix = "";
  smatrix += "[";
  for (int x = 0; x < 8; x++) {
    if (x > 0) smatrix += ",";
    smatrix += "[";
    for (int y = 0; y < 8; y++) {
        if (y > 0) smatrix += ",";
        smatrix += "[";
        smatrix += to_string(matrix[x][y][0]);
        smatrix += ",";
        smatrix += to_string(matrix[x][y][1]);
        smatrix += "]";
    }
    smatrix += "]";
  }
  smatrix += "]";

  if (smatrix == _smatrix) return;
  _smatrix = smatrix;

  ofxOscMessage m;
	m.setAddress("/matrix");
  m.addStringArg(smatrix);
  sender.sendMessage(m, false);

  //cout << smatrix << endl;
}
