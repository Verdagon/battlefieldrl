#pragma once

#include "../Model.h"
#include <type_traits>

// This class simply forwards all calls to the inner unchecked model,
// but does a sanity check before and after the call.
class SanityCheckedModelImpl : public SanityCheckedModel {
private:
  UncheckedModel * mInner;

public:
  SanityCheckedModelImpl(UncheckedModel * model) :
      mInner(model) {
    sanityCheck();
  }

  virtual Player * player() const override {
    return sanityChecked([&]() -> Player * {
      return mInner->player();
    });
  }
  virtual Map * map() const override {
    return sanityChecked([&]() -> Map * {
      return mInner->map();
    });
  }
  virtual VRange<Unit *> units() const override {
    return sanityChecked([&]() -> VRange<Unit *> {
      return mInner->units();
    });
  }
  virtual void addUnit(Unit * unit, Coord location) override {
    return sanityChecked([&]{ mInner->addUnit(unit, location); });
  }
  virtual void setMap(Map * map) override {
    return sanityChecked([&]{ mInner->setMap(map); });
  }
  virtual void moveUnit(Unit * unit, Coord location) override {
    return sanityChecked([&]{ mInner->moveUnit(unit, location); });
  }
  virtual bool locationIsOpen(Coord coord, Unit * unitToIgnoreOrNull) const override {
    return sanityChecked([&]() -> bool {
      return mInner->locationIsOpen(coord, unitToIgnoreOrNull);
    });
  }

private:
  template<
      typename Function,
      typename ReturnValue = typename std::result_of<Function()>::type,
      typename = typename std::enable_if<!std::is_same<ReturnValue, void>::value>::type
  >
  ReturnValue sanityChecked(Function func) const {
    sanityCheck();
    auto result = func();
    sanityCheck();
    return result;
  }

  template<
      typename Function,
      typename ReturnValue = typename std::result_of<Function()>::type,
      typename = typename std::enable_if<std::is_same<ReturnValue, void>::value>::type
  >
  void sanityChecked(Function func) const {
    sanityCheck();
    func();
    sanityCheck();
  }

  void sanityCheck() const;
};
