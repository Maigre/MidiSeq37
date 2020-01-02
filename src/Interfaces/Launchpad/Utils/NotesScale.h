
class NotesScale {
  public:
    NotesScale() {
      // Default scale
      notesScales.resize(16);
      for (uint n=0; n<128; n++)
        notesScales[0].push_back(n);
      startPoint = 36;
    }

    uint available() {
      uint k;
      for (k=0; k<16; k++)
        if (notesScales[k].empty()) return (k-1);
      return k;
    }

    uint start() {
      return startPoint;
    }

    std::vector<uint> notes(uint select=0) {
      if (select < notesScales.size()) return notesScales[select];
      std::vector<uint> empty;
      return empty;
    }

  protected:
    std::vector<std::vector<uint>> notesScales;
    uint startPoint;
};

class AllNotes : public NotesScale {
  public:
    AllNotes() : NotesScale() {}
};

class Airbase99 : public NotesScale {
  public:
    Airbase99() : NotesScale() {
      notesScales[0] = {36, 40, 41, 43, 37, 39, 42, 44, 46, 49, 52};
      notesScales[1] = {36, 41, 43, 40, 37, 39};
      notesScales[2] = {42, 44, 46, 49, 52};
      startPoint = 0;
    }
};

class ScaleSelector {
  public:
    ScaleSelector() {
      for (uint k=0; k<16; k++) presets[k] = NULL;
      presets[0] = new AllNotes();
      presets[8] = new Airbase99();
    }

    NotesScale* preset(uint select) {
      return presets[select];
    }

    NotesScale* presets[16];
};
