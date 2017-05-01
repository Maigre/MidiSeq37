#include "Modes.h"
#include "Mode_base.h"

struct Mode_steps_params {
  uint page;
  uint zoom;
  uint notes[16];
};

class Mode_steps : public Mode_base {
  public:
    Mode_steps(LPstate* st) : Mode_base(st) {
      params = new Mode_steps_params();
      params->page = 1;
      params->zoom = 4;

      int topnote = 43;
      for (uint k=0; k<16; k++)
        params->notes[k] = topnote-k;
    };

    void inputMatrix(uint x, uint y, bool pushed) {
      if (!pushed) return;

      uint note = params->notes[y];
      uint step = (x + (params->page-1)*state->width*8) * RESOLUTION / params->zoom;
      uint length = RESOLUTION / params->zoom;

      Track* track = state->activetrack();
      std::vector<MMidiNote*> notes = track->getNotes(step, length, note);

      if (notes.size() == 0) track->addNote(step, note, length-1);
      else for (u_int k=0; k<notes.size(); k++) notes[k]->remove();

    };

    // TOP pushed
    void inputTop(uint n, bool pushed) {
      if (!pushed) return;

      // page select
      if (state->lastButton(ROW_LEFT) == BTN_STEPS_PAGE) params->page = n+1;

      // length select
      else if (state->lastButton(ROW_LEFT) == BTN_STEPS_LENGTH)
        state->activetrack()->clock()->setLoopSize(n+1);

    };

    // LEFT pushed
    void inputLeft(uint n, bool pushed){
      if (!pushed) return;

      // increase notes
      if (n == BTN_STEPS_NOTEUP)
        for (uint k=0; k<16; k++)
          params->notes[k]++;

      // decrease notes
      else if (n == BTN_STEPS_NOTEDOWN)
        for (uint k=0; k<16; k++)
          params->notes[k]--;

    };

    // DRAW full matrix
    void refresh() {

      // clear
      memset(matrix,    COLOR_OFF, sizeof(matrix));
      memset(extraBtns, COLOR_OFF, sizeof(extraBtns));

      // load
      Track* track = state->activetrack();
      uint width_steps = (state->width*8);
      uint height_steps = (state->height*8);

      // STEPS matrix
      uint active_col = (track->clock()->beatfraction(params->zoom)-1);
      uint xShift = (params->page-1)*width_steps;
      uint stepSize = RESOLUTION/params->zoom;

      for (uint x = 0; x < width_steps; x++) {
        // get notes for current grid
        uint realcol = x+xShift;
        std::vector<MMidiNote*> notes = track->getNotes(realcol*stepSize, stepSize);

        for (uint y = 0; y < height_steps; y++) {
            // vertical red bar
            if (realcol == active_col) matrix[x][y] = COLOR_RED;

            // notes green
            for (uint k=0; k<notes.size(); k++)
              if (notes[k]->note == params->notes[y]) {
                if (matrix[x][y] > COLOR_OFF) matrix[x][y] = COLOR_YELLOW;
                else matrix[x][y] = COLOR_GREEN;
              }
        }
      }

      // LEFT: Yellow on press
      if (state->lastButton(ROW_LEFT) != BTN_NONE)
          extraBtns[ROW_LEFT][state->lastButton(ROW_LEFT)] = COLOR_YELLOW;

      // TOP
      // Page select
      if (state->lastButton(ROW_LEFT) == BTN_STEPS_PAGE) {
        char nPages = track->clock()->beatsloop()*params->zoom/width_steps;
        for (uint k=0; k<width_steps; k++)
          if (k == params->page-1) extraBtns[ROW_TOP][k] = COLOR_RED;
          else if (k < nPages) extraBtns[ROW_TOP][k] = COLOR_GREEN;
          //else extraBtns[ROW_TOP][k] = COLOR_YELLOW;
      }

      // Length select
      else if (state->lastButton(ROW_LEFT) == BTN_STEPS_LENGTH) {
        char nPages = track->clock()->beatsloop()*params->zoom/width_steps;
        for (uint k=0; k<width_steps; k++)
          if (k < nPages) extraBtns[ROW_TOP][k] = COLOR_AMBER;
      }

    };

  private:
    Mode_steps_params* params;

};
