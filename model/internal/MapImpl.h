#pragma once

#include "../Map.h"

class MapImpl : public Map {
  Grid<Tile *> mTiles;

public:
  MapImpl(const Coord & initSize, const char *const source[]);
  virtual ~MapImpl();
  MapImpl(MapImpl && that) = delete;
  MapImpl(const MapImpl &) = delete;

  Coord size() const override {
    return mTiles.size();
  }

  const Tile * tileAt(Coord coord) const override {
    return mTiles[coord];
  }

private:
  // Should definitely inject this one
  static Tile * sourceCharToTile(char character);
};
