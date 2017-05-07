#include "Mode_base16.h"

#define BTN_STEPS_PAGE      0
#define BTN_STEPS_CHANNEL   5
#define BTN_STEPS_NOTEUP    6
#define BTN_STEPS_NOTEDOWN  7

#define BTN_COPY    7
#define BTN_PASTE   6

struct Mode_steps_params {
  uint notes[16];
  uint zoom;
  uint activepage;
  uint copypage;
  uint activemenu;
};

class Mode_steps16 : public Mode_base16 {
  public:
    Mode_steps16(LPstate* st) : Mode_base16(st) {

      params = new Mode_steps_params();
      params->zoom = 4;
      params->activepage = 1;
      params->copypage = 0;

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

      Pattern* patt = state->selectedPattern();
      std::vector<MMidiNote*> notes = patt->getNotes(step, length, note);

      if (notes.size() == 0) patt->addNote(step, note, length-1);
      else for (u_int k=0; k<notes.size(); k++) notes[k]->remove();

    };

    // TOP pushed
    void inputTop(uint n, bool pushed) {
      if (!pushed) return;

      // ActiveMenu
      if (state->lastButton(ROW_LEFT) == BTN_NONE) {

        // Page & Length
        if (params->activemenu == BTN_STEPS_PAGE) {
          if (n < 8) params->activepage = n+1;
          else state->selectedPattern()->clock()->setLoopSize(n-7);
        }
      }

      // Zoom select (reset copypage)
      else if (state->lastButton(ROW_LEFT) == BTN_STEPS_PAGE) {
          params->zoom = n+1;
          params->copypage = 0;
      }

      // channel select
      else if (state->lastButton(ROW_LEFT) == BTN_STEPS_CHANNEL)
        state->selectedTrack()->setChannel(n+1);

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

    // RIGHT pushed
    void inputRight(uint n, bool pushed){
      if (!pushed) return;

      // Copy page
      if (n == BTN_COPY)
        params->copypage = params->activepage;

      // Paste page
      else if (n == BTN_PASTE) {
        if (params->copypage == 0 || params->copypage == params->activepage)
          return;

        uint stepSize = RESOLUTION/params->zoom;
        uint tCount = state->width*8*stepSize;
        uint tOrig = (params->copypage-1)*tCount;
        uint tDest = (params->activepage-1)*tCount;
        state->selectedPattern()->copyNotes(tOrig, tDest, tCount);
      }

      // Base menu
      else Mode_base16::inputRight(n, pushed);
    };

    // DRAW full matrix
    void refresh() {

      // Base draw
      Mode_base16::refresh();

      // load
      Pattern* patt = state->selectedPattern();
      Track* track = state->selectedTrack();
      uint height_steps = (state->height*8);

      // STEPS matrix
      uint active_col = (patt->clock()->beatfraction(params->zoom)-1);
      uint xShift = (params->activepage-1)*state->width*8;
      uint stepSize = RESOLUTION/params->zoom;

      for (uint x = 0; x < state->width*8; x++) {
        // get notes for current grid
        uint realcol = x+xShift;
        std::vector<MMidiNote*> notes = patt->getNotes(realcol*stepSize, stepSize);

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

      // ActiveMenu
      else if (params->activemenu != BTN_NONE)
          extraBtns[ROW_LEFT][params->activemenu] = COLOR_GREEN;


      // TOP
      // Zoom select
      if (state->lastButton(ROW_LEFT) == BTN_STEPS_PAGE) {
        for (uint k=0; k<state->width*8; k++)
          if (k <= params->zoom-1) extraBtns[ROW_TOP][k] = COLOR_YELLOW;
      }

      // Channel select
      else if (state->lastButton(ROW_LEFT) == BTN_STEPS_CHANNEL) {
        for (uint k=0; k<state->width*8; k++)
          if (k == track->getChannel()-1) extraBtns[ROW_TOP][k] = COLOR_RED;
      }

      // Page & Length ActiveMenu
      else if (params->activemenu == BTN_STEPS_PAGE) {
        uint nPages = patt->clock()->beatsloop()*params->zoom/(state->width*8);
        uint active_page = active_col/16;
        for (uint k=0; k<8; k++) {
          if (k == params->activepage-1) extraBtns[ROW_TOP][k] = COLOR_RED;
          else if (k < nPages) extraBtns[ROW_TOP][k] = COLOR_GREEN;

          // BLink running page
          if (k == active_page) extraBtns[ROW_TOP][k] = semiblink(extraBtns[ROW_TOP][k]);
        }

        for (uint k=0; k<8; k++)
          if (k < patt->clock()->barsloop()) extraBtns[ROW_TOP][k+8] = COLOR_YELLOW;
      }

    };

  private:
    Mode_steps_params* params;

};
