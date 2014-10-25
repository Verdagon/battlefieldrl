#include "Game.h"
#include "utils.h"

#include <iostream>
#include <cassert>

class Roguelike {
  GameView * mView;
  const Map * mMap;
  Coord mPlayerLocation;

public:
  Roguelike(GameView * initView, const Map * initMap, const Coord & initPlayerLocation) :
      mView(initView),
      mMap(initMap),
      mPlayerLocation(initPlayerLocation) { }

  void run() {
    using namespace PlayerCommands;

    for (bool running = true; running; ) {
      mView->display(mMap, mPlayerLocation);

      typecase(getPlayerCommand(),
        [&](Exit * command) {
          running = false;
        },
        [&](Move * command) {
          mPlayerLocation = movePlayer(mMap, mPlayerLocation, command->delta);
        });
    }
  }

private:
  // TODO: change this to static or inject
  PlayerCommand * getPlayerCommand() {
    using namespace PlayerCommands;

    while (true) {
      std::cout << "Command? [asdfq] ";
      char input = 0;
      std::cin >> input;
      switch (input) {
        case 'w': return new Move(Coord(-1, 0));
        case 's': return new Move(Coord(1, 0));
        case 'a': return new Move(Coord(0, -1));
        case 'd': return new Move(Coord(0, 1));
        case 'q': return new Exit();
      }

      std::cout << "Invalid input: " << input << std::endl;
    }
  }

  // TODO: change this to static or inject
  static Coord movePlayer(const Map * map, Coord playerLocation, Coord delta) {
    // TODO: Collision code

    return playerLocation + delta;
  }
};





int main() {
  Coord mapSize(24, 80);
  const char * mapSource[] = {
    "################################################################################",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "#..............................................................................#",
    "################################################################################"
  };
  Coord playerStartCoord(15, 15);


  Map * map = Map::create(mapSize, mapSource);

  GameView * view = GameView::createTerminalGameView();

  Roguelike game(view, map, playerStartCoord);

  game.run();
}
