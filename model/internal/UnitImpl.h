#pragma once

#include "../Unit.h"
#include "../../utils.h"

class UnitImpl : public virtual Unit {
  Coord mLocation;

public:
  UnitImpl() :
      mLocation(0, 0) {
  }

  virtual Coord location() const override {
    return mLocation;
  }

  void setLocation(Coord location) {
    mLocation = location;
  }
};

class PlayerImpl : public virtual UnitImpl, public virtual Player {
};

class GoblinImpl : public virtual UnitImpl, public virtual Goblin {
};
