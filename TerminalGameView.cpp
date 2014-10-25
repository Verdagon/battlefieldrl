#include "Game.h"
#include "utils.h"

#include <cassert>
#include <iostream>

class TerminalGameView : public GameView {
public:
  void display(const Map * map, Coord playerLocation) override {
    using namespace Tiles;

    Grid<char> intermediateDisplayBuffer(map->size(), '?');

    for (Coord coord : map->size()) {
      typecase(map->tileAt(coord),
        [&](const Floor *) {
          intermediateDisplayBuffer[coord] = '.';
        },
        [&](const Wall *) {
          intermediateDisplayBuffer[coord] = '#';
        });
    }

    intermediateDisplayBuffer[playerLocation] = '@';

    for (int row = 0; row < map->size().row; row++) {
      for (int col = 0; col < map->size().col; col++) {
        std::cout << intermediateDisplayBuffer[Coord(row, col)];
      }
      std::cout << std::endl;
    }
  }
};

GameView * GameView::createTerminalGameView() {
  return new TerminalGameView();
}