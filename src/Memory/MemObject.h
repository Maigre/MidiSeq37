#pragma once
#include "json/json.h"

class MemObject {
  public:
    Json::Value* mem() {
      return &memory;
    };

    virtual Json::Value memdump() {
      return memory;
    };

    virtual void memload(Json::Value data) {
      memory = data;
    };

  protected:
    Json::Value memory;
};
