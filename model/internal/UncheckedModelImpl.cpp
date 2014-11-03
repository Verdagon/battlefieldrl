#include "UncheckedModelImpl.h"

UncheckedModel * UncheckedModel::newUncheckedModel() {
  return new UncheckedModelImpl();
}

VRange<Unit *> UncheckedModelImpl::units() const {
  VIter<Unit *> begin = viter(castingIter<Unit *>(mUnits.begin()));
  VIter<Unit *> end = viter(castingIter<Unit *>(mUnits.end()));
  return vrange(begin, end);
}

Map * UncheckedModelImpl::map() const {
  return mMap;
}

Player * UncheckedModelImpl::player() const {
  for (Unit * unit : mUnits) {
    if (Player * player = dynamic_cast<Player *>(unit)) {
      return player;
    }
  }
  assert(false);
  return nullptr;
}

bool UncheckedModelImpl::locationIsOpen(Coord coord, Unit * unitToIgnoreOrNull = nullptr) const {
  assert(coord.inside(mMap->size()));

  if (!mMap->tileAt(coord)->isWalkable()) {
    return false;
  }
  for (Unit * unit : mUnits) {
    if (unitToIgnoreOrNull != nullptr && unit == unitToIgnoreOrNull) {
      continue;
    }
    if (coord == unit->location()) {
      return false;
    }
  }
  return true;
}

void UncheckedModelImpl::addUnit(Unit * eunit, Coord location) {
  auto unit = toInternal(eunit);
  unit->setLocation(location);
  mUnits.push_back(unit);
}

void UncheckedModelImpl::moveUnit(Unit * eunit, Coord location) {
  auto unit = toInternal(eunit);
  unit->setLocation(location);
}

void UncheckedModelImpl::setMap(Map * emap) {
  assert(mMap == nullptr); // Take this out if we ever support switching maps
  auto map = toInternal(emap);
  mMap = map;
}
