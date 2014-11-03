#pragma once

#include "../utils.h"

class Tile {
public:
  virtual ~Tile() { }
  virtual bool isWalkable() const = 0;
};

namespace Tiles {
  class Wall : public Tile {
  public:
    bool isWalkable() const {
      return false;
    }
  };

  class Floor : public Tile {
  public:
    bool isWalkable() const {
      return true;
    }
  };
}
