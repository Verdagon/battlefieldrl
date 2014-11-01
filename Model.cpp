#include "Model.h"

class ModelImpl {
public:
  Map * mMap;

  std::vector<Unit *> mUnits; // All units, including goblins and player

  virtual vrange<Unit> units() const override {
    return vrangify(mUnits);
  }

  virtual Map * map() const override {
    return mMap;
  }

  virtual Player * player() const {
    for (Unit * unit : mUnits) {
      if (Player * player = dynamic_cast<Player *>(unit)) {
        return player;
      }
    }
    assert(false);
    return nullptr;
  }
};

class SanityCheckedModel {
private:
  Model * mModel;

public:
  SanityCheckedModel(Model * model) :
      mModel(model) {
    sanityCheck();
  }

  virtual const Player * player() const override {
    sanityCheck();
    auto result = mModel->player();
    sanityCheck();
    return result;
  }

  virtual const Map * map() const override {
    sanityCheck();
    auto result = mModel->map();
    sanityCheck();
    return result;
  }

  virtual vrange<const Unit> units() const override {
    sanityCheck();
    auto result = mModel->units();
    sanityCheck();
    return result;
  }

private:
  void sanityCheck() const {
    if (mGameStarted) {
      assert(mPlayer != nullptr);
      assert(mMap != nullptr);
    }

    if (mPlayer != nullptr) {
      bool playerInUnits = false;
      for (Unit * unit : mUnits) {
        if (unit == dynamic_cast<const Unit *>(mPlayer)) {
          playerInUnits = true;
          break;
        }
      }
      assert(playerInUnits);
    }

    for (Unit * unit : mUnits) {
      // Make sure each unit is on free spot (free except for itself)
      assert(locationIsOpen(unit->location, unit));
    }
  }
};
