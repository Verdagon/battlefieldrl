#include "SanityCheckedModelImpl.h"

SanityCheckedModel * SanityCheckedModel::newCheckedModel(UncheckedModel * inner) {
  return new SanityCheckedModelImpl(inner);
}

void SanityCheckedModelImpl::sanityCheck() const {
  assert(mInner->map() != nullptr);
  assert(mInner->player() != nullptr);

  bool playerInUnits = false;
  for (Unit * unit : mInner->units()) {
    if (unit == dynamic_cast<const Unit *>(mInner->player())) {
      playerInUnits = true;
      break;
    }
  }
  assert(playerInUnits);

  for (Unit * unit : mInner->units()) {
    // Make sure each unit is on free spot (free except for itself)
    assert(mInner->locationIsOpen(unit->location(), unit));
  }
}
