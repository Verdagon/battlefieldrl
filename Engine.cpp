#include "Engine.h"

Engine::Engine(GameUI * ui, SanityCheckedModel * model) :
    mUI(ui),
    mModel(model) { }

Coord Engine::findRandomOpenLocation() const {
  std::vector<Coord> openLocations;
  for (Coord coord : mModel->map()->size()) {
    if (mModel->locationIsOpen(coord)) {
      openLocations.push_back(coord);
    }
  }

  assert(openLocations.size()); // I dont like this, perhaps we should throw an exception instead
  return openLocations[rand() % openLocations.size()];
}

void Engine::run() {
  using namespace PlayerCommands;

  for (int i = 0; i < 20; i++) {
    mModel->addUnit(Goblin::newGoblin(), findRandomOpenLocation());
  }

  while (true) {
    mUI->display(mModel->map(), mModel->units());

    bool playerRequestedExit = false;
    typecase(mUI->getPlayerCommand(),
      [&](Exit * command) {
        playerRequestedExit = true;
      },
      [&](Move * command) {
        Coord newLocation = mModel->player()->location() + command->delta;
        if (canMoveUnit(mModel->player(), newLocation)) {
          mModel->moveUnit(mModel->player(), newLocation);
        }
      });
    if (playerRequestedExit) {
      break;
    }

    goblinsUIPhase();
  }
}

bool Engine::canMoveUnit(Unit * unit, Coord newLocation) const {
  if (!newLocation.inside(mModel->map()->size())) {
    return false;
  }
  if (!mModel->locationIsOpen(newLocation)) {
    return false;
  }
  return true;
}

void Engine::goblinsUIPhase() {
  for (Unit * unit : mModel->units()) {
    if (Goblin * goblin = dynamic_cast<Goblin *>(unit)) {
      goblinUI(goblin);
    }
  }
}

void Engine::goblinUI(Goblin * goblin) {
  Coord delta;
  switch (rand() % 4) {
  case 0:
    delta = Coord(0, 1);
    break;
  case 1:
    delta = Coord(1, 0);
    break;
  case 2:
    delta = Coord(0, -1);
    break;
  case 3:
    delta = Coord(-1, 0);
    break;
  }

  Coord newLocation = goblin->location() + delta;

  if (canMoveUnit(goblin, newLocation)) {
    mModel->moveUnit(goblin, newLocation);
  }
}
