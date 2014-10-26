#include "Game.h"
#include "UI.h"
#include "utils.h"

#include <iostream>
#include <cassert>
#include <vector>

class Engine {
  GameUI * mUI;
  const Map * mMap;

  Player * mPlayer; // Redundant state, for making code more readable. Should be worth the risk, if the risk is mitigated by sanity checks
  std::vector<Unit *> mUnits; // All units, including goblins and player

  // I dont like this, perhaps we should switch to the builder pattern
  bool mGameStarted;
  mutable bool mInsideSanityCheck;

public:
  Engine(GameUI * ui) :
      mUI(ui),
      mMap(nullptr),
      mPlayer(nullptr),
      mGameStarted(false),
      mInsideSanityCheck(false) { }

  void setMap(const Map * map) {
    mMap = map;
  }

  Coord findRandomOpenLocation() const {
    sanityCheck();

    std::vector<Coord> openLocations;
    for (Coord coord : mMap->size()) {
      if (locationIsOpen(coord)) {
        openLocations.push_back(coord);
      }
    }

    assert(openLocations.size()); // I dont like this, perhaps we should throw an exception instead
    return openLocations[rand() % openLocations.size()];
  }

  void addUnit(Unit * unit) {
    sanityCheck();

    if (Player * player = dynamic_cast<Player *>(unit)) {
      assert(mPlayer == nullptr);
      mPlayer = player;
    }

    mUnits.push_back(unit);

    sanityCheck();
  }

  void run() {
    using namespace PlayerCommands;

    sanityCheck();

    while (true) {
      mUI->display(mMap, mUnits);

      sanityCheck();

      bool playerRequestedExit = false;
      typecase(mUI->getPlayerCommand(),
        [&](Exit * command) {
          playerRequestedExit = true;
        },
        [&](Move * command) {
          Coord newLocation = mPlayer->location + command->delta;
          if (canMoveUnit(mPlayer, newLocation)) {
            mPlayer->location = newLocation;
          }
        });
      if (playerRequestedExit) {
        break;
      }

      goblinsUIPhase();

      sanityCheck();
    }
  }

private:
  bool locationIsOpen(Coord coord, Unit * unitToIgnoreOrNull = nullptr) const {
    sanityCheck();

    assert(coord.inside(mMap->size()));

    if (!mMap->tileAt(coord)->isWalkable()) {
      return false;
    }
    for (Unit * unit : mUnits) {
      if (unitToIgnoreOrNull != nullptr && unit == unitToIgnoreOrNull) {
        continue;
      }
      if (coord == unit->location) {
        return false;
      }
    }
    return true;
  }

  void sanityCheck() const {
    // Protect against infinite recursion. For example, sanityCheck calls locationIsOpen which calls sanityCheck and so on
    if (mInsideSanityCheck) {
      return;
    }

    mInsideSanityCheck = true;

    assert(mUI != nullptr); // That this is even a possibility is a failing of the C++ type system, as opposed to scheme or rust.

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

    mInsideSanityCheck = false;
  }

  bool canMoveUnit(Unit * unit, Coord newLocation) const {
    sanityCheck();

    if (!newLocation.inside(mMap->size())) {
      return false;
    }
    if (!locationIsOpen(newLocation)) {
      return false;
    }
    return true;
  }

  void goblinsUIPhase() {
    sanityCheck();

    for (Unit * unit : mUnits) {
      sanityCheck();

      typecase(unit,
          [&](Player * player) { },
          [&](Goblin * goblin) {
            goblinUI(goblin);
          });
    }

    sanityCheck();
  }

  void goblinUI(Goblin * goblin) {
    Coord delta;
    switch (rand() % 4) {
    case 0:
      delta = Coord(0, 1);
      break;
    case 1:
      delta = Coord(1, 0);
      break;
    case 2:
      delta = Coord(0, -1);
      break;
    case 3:
      delta = Coord(-1, 0);
      break;
    }

    Coord newLocation = goblin->location + delta;

    if (canMoveUnit(goblin, newLocation)) {
      goblin->location = newLocation;
    }
  }
};





int main() {
  GameUI * ui = GameUI::createTerminalGameUI();
  Engine engine(ui);

  // Setup the map
  Coord mapSize(24, 80);
  const char * mapSource[] = {
    "################################################################################",
    "#...................#..........................................................#",
    "#...................#..........................................................#",
    "#...................#..........................................................#",
    "#...................#..........................................................#",
    "#...................#..........................................................#",
    "#...................#..........................................................#",
    "#...................#######....................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#.........................#########............................................#",
    "#.................................#............................................#",
    "#.........................#.......#............................................#",
    "#.........................#.......#............................................#",
    "#.........................#########............................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "################################################################################"
  };
  Map * map = Map::create(mapSize, mapSource);
  engine.setMap(map);

  // Add units
  engine.addUnit(Player::create(Coord(15, 15)));

  for (int i = 0; i < 20; i++) {
    engine.addUnit(Goblin::create(engine.findRandomOpenLocation()));
  }

  // Start the game
  engine.run();
}
