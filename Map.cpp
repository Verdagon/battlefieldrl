#include "Game.h"
#include "utils.h"

#include <cassert>

class MapImpl : public Map {
  Grid<Tile *> mTiles;

public:
  MapImpl(const Coord & initSize, const char *const source[]) :
      mTiles(initSize, nullptr) {

    flare(mTiles.size());

    for (Coord coord : mTiles.size()) {
      flare(coord);

      char character = source[coord.row][coord.col];
      mTiles[coord] = sourceCharToTile(character);

    }

    flare();
  }

  virtual ~MapImpl() {
    for (Coord coord : mTiles.size()) {
      delete mTiles[coord];
      mTiles[coord] = nullptr;
    }
  }

  MapImpl(MapImpl && that) = delete;
  MapImpl(const MapImpl &) = delete;

  Coord size() const override {
    return mTiles.size();
  }

  const Tile * tileAt(Coord coord) const override {
    return mTiles[coord];
  }

private:
  static Tile * sourceCharToTile(char character) {
    using namespace Tiles;

    switch (character) {
      case '.': return new Floor();
      case '#': return new Wall();
    }
    flare("unknown character, code ", (int)character);
    assert(false);
  }
};

Map * Map::create(Coord initSize, const char *const source[]) {
  return new MapImpl(initSize, source);
}
