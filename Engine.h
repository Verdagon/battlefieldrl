#pragma once

#include "UI.h"
#include "model/Model.h"

class Engine {
  GameUI * mUI;
  SanityCheckedModel * mModel;

public:
  Engine(GameUI * ui, SanityCheckedModel * model);
  void run();

private:
  Coord findRandomOpenLocation() const;
  bool canMoveUnit(Unit * unit, Coord newLocation) const;
  void goblinsUIPhase(); // TODO Inject this into Unit
  void goblinUI(Goblin * goblin); // TODO Inject this into Unit
};
