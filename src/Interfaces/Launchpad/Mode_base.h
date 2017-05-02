#pragma once
#include "LPstate.h"

class Mode_base : public Lockable {
  public:
    Mode_base(LPstate* s) {
      state = s;

      memset(matrix,    COLOR_OFF, sizeof(matrix));
      memset(extraBtns, COLOR_OFF, sizeof(extraBtns));
    };

    virtual void refresh() = 0;
    virtual void inputMatrix(uint x, uint y, bool pushed) = 0;

    void inputCommand(char row, char btn, bool pushed) {
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

  protected:
    LPstate* state;
    char matrix[16][16];
    char extraBtns[4][16];

    virtual void inputLeft(uint n, bool pushed){
      if (pushed) cout << "input left: " << std::to_string(n) << endl;
    };

    virtual void inputTop(uint n, bool pushed){
      if (pushed) cout << "input top: " << std::to_string(n) << endl;
    };

    virtual void inputBottom(uint n, bool pushed) {
      if (pushed) cout << "input bottom: " << std::to_string(n) << endl;
    };

    virtual void inputRight(uint n, bool pushed){
      if (pushed) cout << "input right: " << std::to_string(n) << endl;
    };

    uint blink(uint color) {
      if (state->activetrack()->clock()->beatfraction(2)%2 == 0) return color;
      else return COLOR_OFF;
    }

};
