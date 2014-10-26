#include "UI.h"

#include <cassert>
#include <iostream>

class TerminalGameUI : public GameUI {
public:
  virtual void display(const Map * map, const std::vector<Unit *> & units) override {
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

    for (Unit * unit : units) {
      char character = 0;
      typecase(unit,
          [&](Player * player) {
            character = '@';
          },
          [&](Goblin * goblin) {
            character = 'g';
          });
      intermediateDisplayBuffer[unit->location] = character;
    }

    for (int row = 0; row < map->size().row; row++) {
      for (int col = 0; col < map->size().col; col++) {
        std::cout << intermediateDisplayBuffer[Coord(row, col)];
      }
      std::cout << std::endl;
    }
  }

  PlayerCommand * getPlayerCommand() override {
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
};

GameUI * GameUI::createTerminalGameUI() {
  return new TerminalGameUI();
}