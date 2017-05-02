#include "Modes.h"
#include "Mode_base.h"

struct Mode_steps_params {
  uint notes[16];
  uint zoom;
  uint activepage;
  uint activemenu;
};

class Mode_steps : public Mode_base {
  public:
    Mode_steps(LPstate* st) : Mode_base(st) {
      params = new Mode_steps_params();
      params->zoom = 4;
      params->activepage = 1;
      params->activemenu = BTN_STEPS_PAGE;

      int topnote = 43;
      for (uint k=0; k<16; k++)
        params->notes[k] = topnote-k;
    };

    void inputMatrix(uint x, uint y, bool pushed) {
      if (!pushed) return;

      uint note = params->notes[y];
      uint step = (x + (params->activepage-1)*state->width*8) * RESOLUTION / params->zoom;
      uint length = RESOLUTION / params->zoom;

      Track* track = state->activetrack();
      std::vector<MMidiNote*> notes = track->getNotes(step, length, note);

      if (notes.size() == 0) track->addNote(step, note, length-1);
      else for (u_int k=0; k<notes.size(); k++) notes[k]->remove();

    };

    // TOP pushed
    void inputTop(uint n, bool pushed) {
      if (!pushed) return;
      uint width_steps = (state->width*8);

      // ActiveMenu
      if (state->lastButton(ROW_LEFT) == BTN_NONE) {

        // Page & Length
        if (params->activemenu == BTN_STEPS_PAGE)
          if (n < 8) params->activepage = n+1;
          else state->activetrack()->clock()->setLoopSize(n-7);
      }

      // 8-ONLY: Length on 8 width Pad
      else if (width_steps == 8 && state->lastButton(ROW_LEFT) == BTN_STEPS_PAGE)
        state->activetrack()->clock()->setLoopSize(n+1);

      // zoom select
      else if (state->lastButton(ROW_LEFT) == BTN_STEPS_ZOOM)
        params->zoom = n+1;

      // channel select
      else if (state->lastButton(ROW_LEFT) == BTN_STEPS_CHANNEL)
        state->activetrack()->setChannel(n+1);





    };

    // LEFT pushed
    void inputLeft(uint n, bool pushed){
      if (!pushed) return;

      // page+length menu
      if (n == BTN_STEPS_PAGE)
        params->activemenu = BTN_STEPS_PAGE;

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
      uint xShift = (params->activepage-1)*width_steps;
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

      // LEFT
      // Yellow on press
      if (state->lastButton(ROW_LEFT) != BTN_NONE)
          extraBtns[ROW_LEFT][state->lastButton(ROW_LEFT)] = COLOR_YELLOW;

      // Yellow on ActiveMenu
      else if (params->activemenu != BTN_NONE)
          extraBtns[ROW_LEFT][params->activemenu] = COLOR_YELLOW;


      // TOP
      // Zoom select
      if (state->lastButton(ROW_LEFT) == BTN_STEPS_ZOOM) {
        for (uint k=0; k<width_steps; k++)
          if (k <= params->zoom-1) extraBtns[ROW_TOP][k] = COLOR_YELLOW;
      }

      // Channel select
      else if (state->lastButton(ROW_LEFT) == BTN_STEPS_CHANNEL) {
        for (uint k=0; k<width_steps; k++)
          if (k == track->getChannel()-1) extraBtns[ROW_TOP][k] = COLOR_RED;
      }

      // 8-ONLY: Length submenu (on press page if 8 width LP only)
      else if (width_steps == 8 && state->lastButton(ROW_LEFT) == BTN_STEPS_PAGE) {
        char nPages = track->clock()->beatsloop()*params->zoom/width_steps;
        for (uint k=0; k<width_steps; k++)
          if (k < nPages) extraBtns[ROW_TOP][k] = COLOR_AMBER;
      }

      // Page & Length ActiveMenu
      else if (params->activemenu == BTN_STEPS_PAGE) {
        uint nPages = track->clock()->beatsloop()*params->zoom/width_steps;
        uint active_page = active_col/16;
        for (uint k=0; k<8; k++)
          if (k == params->activepage-1) {
            if (k == active_page) extraBtns[ROW_TOP][k] = blink(COLOR_RED);
            else extraBtns[ROW_TOP][k] = COLOR_RED;
          }
          else if (k < nPages) {
            if (k == active_page) extraBtns[ROW_TOP][k] = blink(COLOR_GREEN);
            else extraBtns[ROW_TOP][k] = COLOR_GREEN;
          }

        // 16-ONLY:
        if (width_steps > 8)  // 16 width LP -> draw length alongside pages
          for (uint k=0; k<8; k++)
            if (k < track->clock()->barsloop()) extraBtns[ROW_TOP][k+8] = COLOR_AMBER;
      }

    };

  private:
    Mode_steps_params* params;

};
