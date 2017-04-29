class Mode_base {
  public:
    Mode_base(LPstate* s) { state = s; };
    virtual void inputMatrix(char x, char y, bool pushed) = 0;
    virtual void inputTop(char n, bool pushed) = 0;
    virtual void inputLeft(char n, bool pushed) = 0;
    virtual void inputRight(char n, bool pushed) = 0;

    virtual void draw(char matrix[8][8]) = 0;

  protected:
    Sequencer* sequencer;
    LpState* state;

}
