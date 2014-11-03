#include "MapImpl.h"
#include "../../utils.h"

#include <cassert>

MapImpl::MapImpl(const Coord & initSize, const char *const source[]) :
    mTiles(initSize, nullptr) {

  for (Coord coord : mTiles.size()) {
    char character = source[coord.row][coord.col];
    mTiles[coord] = sourceCharToTile(character);
  }
}

MapImpl::~MapImpl() {
  for (Coord coord : mTiles.size()) {
    delete mTiles[coord];
    mTiles[coord] = nullptr;
  }
}

Tile * MapImpl::sourceCharToTile(char character) {
  using namespace Tiles;

  switch (character) {
    case '.': return new Floor();
    case '#': return new Wall();
  }
  flare("unknown character, code ", (int)character);
  assert(false);
}

Map * Map::newMap(Coord initSize, const char *const source[]) {
  return new MapImpl(initSize, source);
}
