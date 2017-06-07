#pragma once
#include "TextFont44.h"

class TextDisp : public TextFont44 {
  public:
    TextDisp(Clock* c) : TextFont44() {
      set("MiniSeq37");
      clock = c;
      scroller = 0;
    };

    void set(string t) {
      if (t == text) return;
      text = t;
      lastBeat = -1; //disable scroller
      scroller = 0;
      basematrix = new uint*[4*text.size()];
      for (uint k=0; k<text.size(); k++) {
        for (uint c=0; c<4; c++) {
          basematrix[4*k+c] = new uint[4];
          for (uint l=0; l<4; l++)
            basematrix[4*k+c][l] = FontTable[text[k]][l][c];
        }
      }
    }

    uint** getMatrix(uint width) {
      uint** submatrix = new uint*[width];
      for (uint c=0; c<width; c++) {
        submatrix[c] = new uint[4];
        for (uint l=0; l<4; l++)
          if ((c+scroller) >= (4*text.size()) || (c+scroller) < 0) submatrix[c][l] = COLOR_OFF;
          else submatrix[c][l] = (basematrix[c+scroller][l]==1)?COLOR_GREEN:COLOR_OFF;
      }
      return submatrix;
    }

    uint maxScroll() {
      return 4*text.size();
    }

    bool scroll(uint speed, bool repeat) {

      // text scrolled out and no repeat
      if (lastBeat == -2) return false;

      // scrolling did not begin yet
      if (lastBeat == -1) {
        lastBeat = 0; //enable scroller
        scroller = -17;
      }

      // scroll
      uint cbeat = clock->beatfraction(speed);
      if (lastBeat != cbeat) {
        scroller++;
        lastBeat = cbeat;

        // short text scroll to start: lock it here
        if (text.size() <= 4 && scroller > 0 && !repeat) scroller=0;

        // text scrolled out, reset or cancel
        if (scroller > (int)text.size()*4)
          if (repeat) scroller = -16;
          else {
            lastBeat = -2;
            return false;
          }
      }

      return true;
    }

  private:
    string text;
    uint** basematrix;
    Clock* clock;
    int scroller;
    int lastBeat;


};
