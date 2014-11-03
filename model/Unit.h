#pragma once

#include "../utils.h"

class Unit {
public:
  virtual ~Unit() { }
  virtual Coord location() const = 0;
};

class Player : public virtual Unit {
public:
  static Player * newPlayer();
};

class Goblin : public virtual Unit {
public:
  static Goblin * newGoblin();
};
