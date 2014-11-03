#pragma once

#include "UnitImpl.h"
#include "MapImpl.h"
#include "../Model.h"
#include <list>

class UncheckedModelImpl : public UncheckedModel {
  std::list<UnitImpl *> mUnits;
  MapImpl * mMap;

public:
  UncheckedModelImpl() :
      mMap(nullptr) { }

  virtual Player * player() const override;
  virtual Map * map() const override;
  virtual VRange<Unit *> units() const override;

  virtual void addUnit(Unit * unit, Coord location) override;
  virtual void moveUnit(Unit * unit, Coord location) override;
  virtual void setMap(Map * map) override;

  virtual bool locationIsOpen(Coord coord, Unit * unitToIgnoreOrNull) const override;

private:
  inline UnitImpl * toInternal(Unit * unit) {
    UnitImpl * result = dynamic_cast<UnitImpl *>(unit);
    assert(result);
    return result;
  }
  inline MapImpl * toInternal(Map * unit) {
    MapImpl * result = dynamic_cast<MapImpl *>(unit);
    assert(result);
    return result;
  }
};
