#pragma once

#include "model/Model.h"

class PlayerCommand {
public:
  virtual ~PlayerCommand() { }
};

namespace PlayerCommands {
  class Exit : public PlayerCommand { };

  class Move : public PlayerCommand {
  public:
    Coord delta;

    Move(Coord initDelta) :
        delta(initDelta) {
    }
  };
};

class GameUI {
public:
  static GameUI * createTerminalGameUI();

  virtual ~GameUI() { }
  // Dont like how this mandates that the units be held in a vector
  // Would prefer some sort of virtual iterator, i think
  // Would also prefer const Unit * instead of Unit *
  virtual void display(const Map * map, VRange<Unit *> units) = 0;

  virtual PlayerCommand * getPlayerCommand() = 0;
};
