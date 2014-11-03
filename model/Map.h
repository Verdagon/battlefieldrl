#pragma once

#include "Tile.h"
#include "../utils.h"
#include <vector>

class Map {
public:
  static Map * newMap(Coord initSize, const char *const source[]);

  virtual Coord size() const = 0;
  virtual const Tile * tileAt(Coord coord) const = 0;
};
