#ifndef WORLD_HPP
#define WORLD_HPP
#include <FastLED.h>

class World {
private:
  int distance; // distance between obstacles
  int gap;      // gap within the obstacles
  int state;
  int world[16][16];

public:
  World(int gap, int distance) : gap(gap), distance(distance), state(0) {
    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 16; j++) {
        this->world[i][j] = 0;
      }
    }
  }
  ~World(){}
  void shift();
  void print(CRGB *leds) const;
  void reset();
};

#endif // WORLD_HPP