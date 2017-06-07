#pragma once
#include "Modes.h"
#include "LPstore.h"
#include "TextDisp.h"

class Mode_abstract {
  public:
    Mode_abstract(LPstore* st) {
      store = st;

      memset(matrix,    COLOR_OFF, sizeof(matrix));
      memset(extraBtns, COLOR_OFF, sizeof(extraBtns));

      initText();
    };

    // DRAW
    virtual void refresh() {
      // clear
      memset(matrix,    COLOR_OFF, sizeof(matrix));
      memset(extraBtns, COLOR_OFF, sizeof(extraBtns));
    };

    virtual void inputMatrix(uint x, uint y, bool pushed) {};

    void inputCommand(char row, char btn, bool pushed) {
      store->buttons->record(row, btn, pushed);

      if (row == ROW_TOP)         inputTop(btn, pushed);
      else if (row == ROW_LEFT)   inputLeft(btn, pushed);
      else if (row == ROW_RIGHT)  inputRight(btn, pushed);
      else if (row == ROW_BOTTOM) inputBottom(btn, pushed);
    }

    // copy submatrix
    char** getMatrix(uint offset) {
      char** submatrix;
      submatrix = new char*[8];
      uint xShift = (offset%2)*8;
      uint yShift = (offset/2)*8;
      for (uint x=0; x<8; x++) {
        submatrix[x] = new char[8];
        for (uint y=0; y<8; y++)
          submatrix[x][y] = matrix[x+xShift][y+yShift];
      }
      return submatrix;
    };

    // copy subcmds
    char** getCommands(uint offset) {
      char** subcmds;
      subcmds = new char*[2];
      subcmds[0] = new char[8];
      subcmds[1] = new char[8];

      uint xShift = (offset%2)*8;
      uint yShift = (offset/2)*8;
      if (offset/2 == 0)
        for (uint k=0; k<8; k++)
          subcmds[0][k] = extraBtns[ROW_TOP][k+xShift];
      else
        for (uint k=0; k<8; k++)
          subcmds[0][k] = extraBtns[ROW_BOTTOM][k+xShift];

      if (offset%2 == 0)
        for (uint k=0; k<8; k++)
          subcmds[1][k] = extraBtns[ROW_LEFT][k+yShift];
      else
        for (uint k=0; k<8; k++)
          subcmds[1][k] = extraBtns[ROW_RIGHT][k+yShift];

      return subcmds;
    };

    virtual void onFocus() {
      initText();
    };

  protected:
    LPstore* store;
    char matrix[16][16];
    char extraBtns[16][16];

    TextDisp* text;

    void initText() {
      text = new TextDisp(store->baseclock());
    }

    bool drawText(string t, uint yoffset, uint speed = 0, bool repeat = false) {
      text->set(t);
      if (speed > 0 && !text->scroll(speed, repeat)) return false;
      uint** txt = text->getMatrix(16);
      for (uint x = 0; x < 16; x++)
        for (uint y = 0; y < 4; y++)
            matrix[x][y+yoffset] = txt[x][y];
      return true;
    }

    // called by LPad via State
    virtual void inputLeft(uint n, bool pushed) {};
    virtual void inputTop(uint n, bool pushed) {};
    virtual void inputBottom(uint n, bool pushed) {};
    virtual void inputRight(uint n, bool pushed) {};

    uint blink(uint color) {
      if (store->baseclock()->beatfraction(2)%2 != 0) return color;
      else return COLOR_OFF;
    }

    uint semiblink(uint color) {
      if (store->baseclock()->beatfraction(2)%2 != 0) return color;
      else if (color == COLOR_GREEN) return COLOR_GREEN_LOW;
      else if (color == COLOR_RED) return COLOR_RED_LOW;
      else if (color == COLOR_AMBER) return COLOR_AMBER_LOW;
      else if (color == COLOR_ORANGE) return COLOR_ORANGE_LOW;
      else if (color == COLOR_YELLOW) return COLOR_YELLOW_LOW;
      else return COLOR_OFF;
    }

};
