#include "Game.h"

Goblin * Goblin::create(Coord location) {
  return new Goblin(location);
}
