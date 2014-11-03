#include "UnitImpl.h"
#include "../Unit.h"
#include "../../utils.h"

Player * Player::newPlayer() {
  return new PlayerImpl();
}

Goblin * Goblin::newGoblin() {
  return new GoblinImpl();
}
