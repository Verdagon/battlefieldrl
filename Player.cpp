#include "Game.h"

Player * Player::create(Coord location) {
  return new Player(location);
}
