#pragma once
#include "json/json.h"
#include <mutex>

#define NAMEBASE  "memory-"
#define NAMEEXT   "json"

class MemFiles {
  public:
    static void Write(uint mem, Json::Value data) {
      static std::mutex mutex;
      mutex.lock();
      Json::Value json;
      json["MiniSeq37_v0"] = data;

      Json::StyledWriter styledWriter;
      ofFile file;
      string filename = NAMEBASE+to_string(mem)+"."+NAMEEXT;
      file.open(ofToDataPath(filename), ofFile::WriteOnly);
      file << styledWriter.write(json);
      file.close();
      mutex.unlock();
      //cout << "SAVE " << filename << endl;
    }

    static Json::Value Read(uint mem) {
      static std::mutex mutex;
      mutex.lock();
      Json::Value json;
      Json::Reader reader;
      ofFile file;
      string filename = NAMEBASE+to_string(mem)+"."+NAMEEXT;
      file.open(ofToDataPath(filename), ofFile::ReadOnly);
      reader.parse(file.readToBuffer().getText(), json);
      file.close();

      // Json::StyledWriter styledWriter;
      // cout << styledWriter.write(json);

      //cout << "LOAD " << filename << endl;
      mutex.unlock();
      if (json.isMember("MiniSeq37_v0")) return json["MiniSeq37_v0"];
      else return json;
    }

    static vector<uint> List() {
      vector<uint> list;
      ofDirectory dir(ofToDataPath("."));
      dir.listDir();
      dir.allowExt(NAMEEXT);
      string basename = NAMEBASE;
      for(uint k=0; k<dir.size(); k++)
        if (dir.getName(k).find(NAMEBASE) == 0) {
          string banknum = dir.getName(k).substr(basename.size());
          banknum = banknum.substr(0, banknum.find("."));
          list.push_back(stoi(banknum));
        }
      return list;
    }

};
