#include "Mode_base16.h"
#include "NotesScale.h"

// LEFT BTNS
#define BTN_STEPS_MUTE      0
#define BTN_STEPS_PAGE      1
#define BTN_STEPS_CHANNEL   4
#define BTN_STEPS_PROGRAM   5
#define BTN_STEPS_NOTEUP    6
#define BTN_STEPS_NOTEDOWN  7

// RIGHT BTNS
#define BTN_COPY    7
#define BTN_PASTE   6

#define MENU_NONE     0
#define MENU_STEPS    1
#define MENU_ZOOM     2
#define MENU_MIDICH   3
#define MENU_SCALE    4
#define MENU_MUTE     5
#define MENU_PROGRAM  6


class Mode_steps16 : public Mode_base16 {
  public:
    Mode_steps16(LPstore* st) : Mode_base16(st) {
      scales = new ScaleSelector();
    };

    void onFocus() {
      Mode_base16::onFocus();
      init_mem();
    }

    // MATRIX pushed
    void inputMatrix(uint x, uint y, bool pushed) {
      if (!pushed) return;

      // SELECT PROGRAM
      if (menu() == MENU_PROGRAM) {

        // pressed: erase program selection
        if (store->buttons->active(ROW_LEFT, BTN_STEPS_PROGRAM)) {
          if (store->pattern()->getProgram() != NULL)
            if (store->pattern()->getProgram()->program == x+16*y) {
              store->pattern()->clearProgram();
              bank(0);
            }
        }

        // select program
        else if (bank() > 0) {
          store->pattern()->setProgram(bank()-1, x+16*y);
          store->track()->playProgram();
        }
      }

      // ADD/REM Note
      else {
        uint length = RESOLUTION / zoom();
        uint step = (x + (page()-1)*store->width*8) * length;

        if (note(y) > 0) {
          Pattern* patt = store->pattern();
          std::vector<MMidiNote*> notes = patt->getNotes(step, length, note(y));

          if (notes.size() == 0) patt->addNote(step, note(y), length-1);
          else for (u_int k=0; k<notes.size(); k++) notes[k]->remove();
        }
      }

    };

    // LEFT pushed
    void inputLeft(uint n, bool pushed){

      // page+length + zoom menu
      if (n == BTN_STEPS_PAGE) {
        menu(MENU_STEPS);
        menu(MENU_ZOOM, pushed);
      }

      // midi channel
      else if (n == BTN_STEPS_CHANNEL) menu(MENU_MIDICH, pushed);

      // program
      else if (n == BTN_STEPS_PROGRAM) menu(MENU_PROGRAM);

      // increase notes
      else if (n == BTN_STEPS_NOTEUP && pushed) {
        if (note(-1) > 0) scalebase( scalebase()+1 );
        if (store->buttons->active(ROW_LEFT, BTN_STEPS_NOTEDOWN)) menu(MENU_SCALE);
      }

      // decrease notes
      else if (n == BTN_STEPS_NOTEDOWN && pushed) {
        if (note(8) > 0) scalebase( scalebase()-1 );
        if (store->buttons->active(ROW_LEFT, BTN_STEPS_NOTEUP)) menu(MENU_SCALE);
      }

    };

    // TOP pushed
    void inputTop(uint n, bool pushed) {
      if (!pushed) return;

      // Page & Length
      if (menu() == MENU_STEPS) {
        if (n < 8) page(n+1);
        else store->pattern()->clock()->setLoopSize(n-7);
      }

      // Zoom select (reset copypage)
      else if (menu() == MENU_ZOOM) {
          zoom(n+1);
          copypage(0);
      }

      // channel select
      else if (menu() == MENU_MIDICH)
        store->track()->setChannel(n+1);

      // program bank select
      else if (menu() == MENU_PROGRAM) bank(n+1);

      // Scale select
      else if (menu() == MENU_SCALE) scale(n);

    };

    // RIGHT pushed
    void inputRight(uint n, bool pushed){
      if (!pushed) return;

      // Subscale select
      if (menu() == MENU_SCALE) {
        if (subscale() == n) menu(MENU_STEPS);
        else subscale(n);
      }

      // Copy page
      else if (n == BTN_COPY) copypage(page());

      // Paste page or erase pattern
      else if (n == BTN_PASTE) {
        // Copy is pressed -> Erase pattern
        if (store->buttons->active(ROW_RIGHT, BTN_COPY)) {
          store->pattern()->clear();
          init_display();
        }
        // Paste
        else if (copypage() > 0 && copypage() != page()) {
          uint stepSize = RESOLUTION/zoom();
          uint tCount = store->width*8*stepSize;
          uint tOrig = (copypage()-1)*tCount;
          uint tDest = (page()-1)*tCount;
          store->pattern()->copyNotes(tOrig, tDest, tCount);
        }
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
      bool isPlaying = track->activePatternIndex() == store->getPatt();

      // No pattern selected: exit edit
      if (patt == NULL || track == NULL) {
        store->setMode(MODE_PATTS);
        return;
      }

      uint active_col = (patt->clock()->beatfraction(zoom())-1);
      uint xShift = (page()-1)*store->width*8;
      uint stepSize = RESOLUTION / zoom();

      // PRGM matrix
      if (menu() == MENU_PROGRAM) {
        for (uint x = 0; x < 16; x++)
          for (uint y = 0; y < 16; y++) {
            matrix[x][y] = COLOR_GREEN_LOW;
            if (store->pattern()->getProgram() != NULL
                  && bank()-1 == store->pattern()->getProgram()->bank
                  && (x+16*y) == store->pattern()->getProgram()->program)
                    matrix[x][y] = COLOR_AMBER;
          }
      }

      // STEPS matrix
      else {

        for (uint x = 0; x < 16; x++) {
          // get notes for current grid
          uint realcol = x+xShift;
          std::vector<MMidiNote*> notes = patt->getNotes(realcol*stepSize, stepSize);

          for (uint y = 0; y < 16; y++) {
              // vertical red bar
              if (isPlaying && realcol == active_col) matrix[x][y] = COLOR_RED;

              // notes green
              for (uint k=0; k<notes.size(); k++)
                if (note(y) > 0 && notes[k]->note == note(y)) {
                  if (matrix[x][y] > COLOR_OFF) matrix[x][y] = COLOR_YELLOW;
                  else matrix[x][y] = COLOR_GREEN;
                }
          }
        }
      }

      // LEFT: Yellow on press
      if (store->buttons->active(ROW_LEFT) != BTN_NONE)
          extraBtns[ROW_LEFT][store->buttons->active(ROW_LEFT)] = COLOR_YELLOW;

      // MENU: Page & Length
      if (menu() == MENU_STEPS) {
        // LEFT
        extraBtns[ROW_LEFT][BTN_STEPS_PAGE] = COLOR_GREEN;

        // TOP: Page
        uint nPages = patt->clock()->beatsloop()*zoom()/(store->width*8);
        uint active_page = active_col/16;
        for (uint k=0; k<8; k++) {
          if (k == page()-1) extraBtns[ROW_TOP][k] = COLOR_RED;
          else if (k < nPages) extraBtns[ROW_TOP][k] = COLOR_GREEN;

          // BLink running page
          if (k == active_page) extraBtns[ROW_TOP][k] = semiblink(extraBtns[ROW_TOP][k]);
        }

        // TOP: Length
        for (uint k=0; k<8; k++)
          if (k < patt->clock()->barsloop()) extraBtns[ROW_TOP][k+8] = COLOR_YELLOW;
      }

      // MENU: Zoom select
      if (menu() == MENU_ZOOM) {
        // TOP
        for (uint k=0; k<store->width*8; k++)
          if (k <= zoom()-1) extraBtns[ROW_TOP][k] = COLOR_YELLOW;
      }

      // MENU: Midi Channel select
      else if (menu() == MENU_MIDICH) {
        // TOP
        for (uint k=0; k<store->width*8; k++)
          if (k == track->getChannel()-1) extraBtns[ROW_TOP][k] = COLOR_RED;
      }

      // MENU: Program select
      else if (menu() == MENU_PROGRAM) {
        // LEFT
        extraBtns[ROW_LEFT][BTN_STEPS_PROGRAM] = COLOR_GREEN;

        // TOP: Bank
        for (uint k=0; k<16; k++) {
          if (k == bank()-1) extraBtns[ROW_TOP][k] = COLOR_AMBER;
          else extraBtns[ROW_TOP][k] = COLOR_GREEN_LOW;
        }
      }

      // MENU: Note Scale
      else if (menu() == MENU_SCALE) {
        // LEFT
        extraBtns[ROW_LEFT][BTN_STEPS_NOTEUP] = COLOR_YELLOW;
        extraBtns[ROW_LEFT][BTN_STEPS_NOTEDOWN] = COLOR_YELLOW;

        // TOP
        for (uint k=0; k<16; k++)
          if (k == scale())
            extraBtns[ROW_TOP][k] = blink(COLOR_YELLOW);
          else if (scales->preset(k) != NULL)
            extraBtns[ROW_TOP][k] = COLOR_GREEN_LOW;

        // RIGHT
        for (uint k=0; k<16; k++)
          if (scales->preset(scale()) != NULL)
            if (k == subscale())
              extraBtns[ROW_RIGHT][k] = blink(COLOR_YELLOW);
            else if (k <= scales->preset(scale())->available())
              extraBtns[ROW_RIGHT][k] = COLOR_GREEN_LOW;
            else
              extraBtns[ROW_RIGHT][k] = COLOR_OFF;

      }

    };

  private:

    ScaleSelector* scales;

    void init_mem() {

      // reset on Focus
      copypage(0);
      menu(MENU_STEPS);
      menu(MENU_STEPS, false);

      if (store->pattern()->getProgram() != NULL)
        bank(store->pattern()->getProgram()->bank+1);
      else bank(0);


      // not reset onFocus / kept from memory
      if (mem_uint("init") == 1) return;
      mem_uint("init",        1);
      init_display();
      scale(0);

      // cout << "Mem init" << endl;
    }

    void init_display() {
      zoom(4);
      page(1);
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

    // ActiveMenu GET/SET
    uint menu() {
      if (mem_uint("tempmenu") != MENU_NONE) return mem_uint("tempmenu");
      else return mem_uint("activemenu");
    }
    void menu(uint sel) { mem_uint("activemenu", sel); }
    void menu(uint sel, bool pushed) {
      if (pushed) mem_uint("tempmenu", sel);
      else mem_uint("tempmenu", MENU_NONE);
    }

    // Zoom GET/SET
    uint zoom() { return mem_uint("zoom"); }
    void zoom(uint sel) { mem_uint("zoom", sel); }

    // Zoom GET/SET
    uint page() { return mem_uint("activepage"); }
    void page(uint sel) { mem_uint("activepage", sel); }

    // Copy GET/SET
    uint copypage() { return mem_uint("copypage"); }
    void copypage(uint sel) { mem_uint("copypage", sel); }

    // Bank GET/SET
    uint bank() { return mem_uint("bank"); }
    void bank(uint sel) { mem_uint("bank", sel); }

    // Scale GET/SET
    uint scale() { return mem_uint("scale"); }
    void scale(uint sel) {
      if (scales->preset(sel) != NULL) {
        mem_uint("scale", sel);
        subscale(0);
      }
    }

    // SubScale GET/SET
    uint subscale() { return mem_uint("subscale"); }
    void subscale(uint sel) {
      if (scales->preset(scale()) != NULL && sel <= scales->preset(scale())->available()) {
        mem_uint("subscale", sel);
        scalebase(scales->preset(scale())->start());
      }
    }

    // Scale basenote GET/SET
    uint scalebase() { return mem_uint("scalebase"); }
    void scalebase(uint sel) { mem_uint("scalebase", sel); }

    // Notes GET/SET
    uint note(uint y) {
      if (scales->preset(scale()) == NULL) return 0;
      vector<uint> notesV = scales->preset(scale())->notes(subscale());
      y = 7-y+scalebase();
      if (y < notesV.size() && y >= 0) return notesV[y];
      return 0;
    }

};
