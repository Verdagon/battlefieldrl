#pragma once

#include "utils.h"

class Tile {
public:
  virtual ~Tile() { }
  virtual bool isWalkable() const = 0;
};

namespace Tiles {
  class Wall : public Tile {
  public:
    bool isWalkable() const {
      return false;
    }
  };

  class Floor : public Tile {
  public:
    bool isWalkable() const {
      return true;
    }
  };
}

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

class Map {
public:
  static Map * create(Coord initSize, const char *const source[]);

  virtual Coord size() const = 0;
  virtual const Tile * tileAt(Coord coord) const = 0;
};

class GameView {
public:
  static GameView * createTerminalGameView();

  virtual ~GameView() { }
  virtual void display(const Map * map, Coord playerLocation) = 0;
};
