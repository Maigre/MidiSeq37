#include "Mode_base16.h"

#define BTN_STEPS_PAGE      0
#define BTN_STEPS_CHANNEL   5
#define BTN_STEPS_NOTEUP    6
#define BTN_STEPS_NOTEDOWN  7

#define BTN_COPY    7
#define BTN_PASTE   6


class Mode_steps16 : public Mode_base16 {
  public:
    Mode_steps16(LPstore* st) : Mode_base16(st) {
    };

    void onFocus() {
      Mode_base16::onFocus();
      init_mem();
    }

    // MATRIX pushed
    void inputMatrix(uint x, uint y, bool pushed) {
      if (!pushed) return;

      uint note = mem_array_uint("notes", y);
      uint length = RESOLUTION / mem_uint("zoom");
      uint step = (x + (mem_uint("activepage")-1)*store->width*8) * length;


      Pattern* patt = store->pattern();
      std::vector<MMidiNote*> notes = patt->getNotes(step, length, note);

      if (notes.size() == 0) patt->addNote(step, note, length-1);
      else for (u_int k=0; k<notes.size(); k++) notes[k]->remove();

    };

    // TOP pushed
    void inputTop(uint n, bool pushed) {
      if (!pushed) return;

      // ActiveMenu
      if (store->buttons->active(ROW_LEFT) == BTN_NONE) {

        // Page & Length
        if (mem_uint("activemenu") == BTN_STEPS_PAGE) {
          if (n < 8) mem_uint("activepage", n+1);
          else store->pattern()->clock()->setLoopSize(n-7);
        }
      }

      // Zoom select (reset copypage)
      else if (store->buttons->active(ROW_LEFT) == BTN_STEPS_PAGE) {
          mem_uint("zoom", n+1);
          mem_uint("copypage", 0);
      }

      // channel select
      else if (store->buttons->active(ROW_LEFT) == BTN_STEPS_CHANNEL)
        store->track()->setChannel(n+1);

    };

    // LEFT pushed
    void inputLeft(uint n, bool pushed){
      if (!pushed) return;

      // page+length menu
      if (n == BTN_STEPS_PAGE)
        mem_uint("activemenu", BTN_STEPS_PAGE);

      // increase notes
      if (n == BTN_STEPS_NOTEUP)
        for (uint k=0; k<16; k++)
          mem_array_uint("notes", k, mem_array_uint("notes", k)+1);

      // decrease notes
      else if (n == BTN_STEPS_NOTEDOWN)
        for (uint k=0; k<16; k++)
          mem_array_uint("notes", k, mem_array_uint("notes", k)-1);

    };

    // RIGHT pushed
    void inputRight(uint n, bool pushed){
      if (!pushed) return;

      // Copy page
      if (n == BTN_COPY)
        mem_uint("copypage", mem_uint("activepage"));

      // Paste page
      else if (n == BTN_PASTE) {
        if (mem_uint("copypage") == 0 || mem_uint("copypage") == mem_uint("activepage"))
          return;

        uint stepSize = RESOLUTION/mem_uint("zoom");
        uint tCount = store->width*8*stepSize;
        uint tOrig = (mem_uint("copypage")-1)*tCount;
        uint tDest = (mem_uint("activepage")-1)*tCount;
        store->pattern()->copyNotes(tOrig, tDest, tCount);
      }

      // Base menu
      else Mode_base16::inputRight(n, pushed);
    };

    // DRAW full matrix
    void refresh() {

      // Base draw
      Mode_base16::refresh();

      // load
      Pattern* patt = store->pattern();
      Track* track = store->track();
      uint height_steps = (store->height*8);
      bool isPlaying = track->activePatternIndex() == store->getPatt();

      // No pattern selected: exit edit
      if (patt == NULL || track == NULL) {store->setMode(MODE_PATTS); return;}

      // STEPS matrix
      uint active_col = (patt->clock()->beatfraction(mem_uint("zoom"))-1);
      uint xShift = (mem_uint("activepage")-1)*store->width*8;
      uint stepSize = RESOLUTION / mem_uint("zoom");

      for (uint x = 0; x < store->width*8; x++) {
        // get notes for current grid
        uint realcol = x+xShift;
        std::vector<MMidiNote*> notes = patt->getNotes(realcol*stepSize, stepSize);

        for (uint y = 0; y < height_steps; y++) {
            // vertical red bar
            if (isPlaying && realcol == active_col) matrix[x][y] = COLOR_RED;

            // notes green
            for (uint k=0; k<notes.size(); k++)
              if (notes[k]->note == mem_array_uint("notes", y)) {
                if (matrix[x][y] > COLOR_OFF) matrix[x][y] = COLOR_YELLOW;
                else matrix[x][y] = COLOR_GREEN;
              }
        }
      }

      // LEFT
      // Yellow on press
      if (store->buttons->active(ROW_LEFT) != BTN_NONE)
          extraBtns[ROW_LEFT][store->buttons->active(ROW_LEFT)] = COLOR_YELLOW;

      // ActiveMenu
      else if (mem_uint("activemenu") != BTN_NONE)
          extraBtns[ROW_LEFT][mem_uint("activemenu")] = COLOR_GREEN;


      // TOP
      // Zoom select
      if (store->buttons->active(ROW_LEFT) == BTN_STEPS_PAGE) {
        for (uint k=0; k<store->width*8; k++)
          if (k <= mem_uint("zoom")-1) extraBtns[ROW_TOP][k] = COLOR_YELLOW;
      }

      // Channel select
      else if (store->buttons->active(ROW_LEFT) == BTN_STEPS_CHANNEL) {
        for (uint k=0; k<store->width*8; k++)
          if (k == track->getChannel()-1) extraBtns[ROW_TOP][k] = COLOR_RED;
      }

      // Page & Length ActiveMenu
      else if (mem_uint("activemenu") == BTN_STEPS_PAGE) {
        uint nPages = patt->clock()->beatsloop()*mem_uint("zoom")/(store->width*8);
        uint active_page = active_col/16;
        for (uint k=0; k<8; k++) {
          if (k == mem_uint("activepage")-1) extraBtns[ROW_TOP][k] = COLOR_RED;
          else if (k < nPages) extraBtns[ROW_TOP][k] = COLOR_GREEN;

          // BLink running page
          if (k == active_page) extraBtns[ROW_TOP][k] = semiblink(extraBtns[ROW_TOP][k]);
        }

        for (uint k=0; k<8; k++)
          if (k < patt->clock()->barsloop()) extraBtns[ROW_TOP][k+8] = COLOR_YELLOW;
      }

    };

  private:

    void init_mem() {
      if (mem_uint("init") == 1) return;
      mem_uint("init",        1);

      mem_uint("zoom",        4);
      mem_uint("activepage",  1);
      mem_uint("copypage",    0);
      mem_uint("activemenu",  BTN_STEPS_PAGE);

      int topnote = 43;
      for (uint k=0; k<16; k++)
        mem_array_uint("notes", k, topnote-k);

      // cout << "Mem init" << endl;
    }

    // HOOK TO Pattern Memory
    uint mem_uint(string key) {
      return (*store->pattern()->mem())["LPsteps"][key].asUInt();
    }

    void mem_uint(string key, uint val) {
      (*store->pattern()->mem())["LPsteps"][key] = val;
    }

    uint mem_array_uint(string key, uint k) {
      return (*store->pattern()->mem())["LPsteps"][key][k].asUInt();
    }

    void mem_array_uint(string key, uint k, uint val) {
      (*store->pattern()->mem())["LPsteps"][key][k] = val;
    }

};
