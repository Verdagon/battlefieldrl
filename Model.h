#pragma once

#include "utils.h"
#include <vector>

class Unit {
public:
  Coord location;

  Unit(Coord initLocation) :
      location(initLocation) {
  }

  virtual ~Unit() { }
};

class Player : public Unit {
public:
  Player(Coord location) :
      Unit(location) { }

  static Player * create(Coord location);
};

class Goblin : public Unit {
public:
  Goblin(Coord location) :
      Unit(location) { }

  static Goblin * create(Coord location);
};



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

class Map {
public:
  static Map * create(Coord initSize, const char *const source[]);

  virtual Coord size() const = 0;
  virtual const Tile * tileAt(Coord coord) const = 0;
};

class Model {
public:
  virtual ~Model() { }

  virtual Player * player() const = 0;

  virtual Map * map() const = 0;

  virtual vrange<Unit *> units() const = 0;
};
