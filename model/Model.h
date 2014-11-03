#pragma once

#include "../utils.h"
#include "Unit.h"
#include "Map.h"
#include <vector>

class Model {
public:
  virtual ~Model() { }

  virtual Player * player() const = 0;
  virtual Map * map() const = 0;
  virtual VRange<Unit *> units() const = 0;

  virtual bool locationIsOpen(Coord coord, Unit * unitToIgnoreOrNull = nullptr) const = 0;

  virtual void addUnit(Unit * unit, Coord location) = 0;
  virtual void moveUnit(Unit * unit, Coord location) = 0;
  virtual void setMap(Map * map) = 0;
};

class UncheckedModel : public virtual Model {
public:
  static UncheckedModel * newUncheckedModel();
};

class SanityCheckedModel : public virtual Model {
public:
  static SanityCheckedModel * newCheckedModel(UncheckedModel * inner);
};
