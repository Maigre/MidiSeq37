#include "MMidiEvent.h"
#include "Clock.h"
#include "../Utils/Lockable.h"
#include "../Memory/MemObject.h"

class Pattern : public Lockable, public MemObject {
  public:
    Pattern();

    bool notempty();
    Clock* clock();
    void resize();
    void resize(uint64_t t);

    MMidiNote* addNote(uint tick, uint note, uint duration);
    MMidiNote* addNote(uint tick, uint note, uint duration, uint velocity);
    void copyNotes(uint startCopy, uint startPast, uint count);
    std::vector<MMidiNote*> getNotes(uint start, uint size);
    std::vector<MMidiNote*> getNotes(uint start, uint size, int noteval);

    void cleanNotes(uint tick);

    Json::Value memdump();
    void memload(Json::Value data);

  private:
    Clock* pattclock;
    std::vector<std::vector<MMidiNote*>> notesON;

};
