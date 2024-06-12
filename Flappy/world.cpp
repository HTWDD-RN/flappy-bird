#include "world.hpp"
#include <FastLED.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void shift_left(int a[], size_t n) {
  if (n--) {
    memmove(a, a + 1, n * sizeof(*a));
    a[n] = 0;
  }
}

World::~World() {}

void World::shift() {
  memmove(this->world, this->world + 1,
          sizeof(this->world) - sizeof(this->world[0]));
  memset(this->world + 16 - 1, 0, sizeof(*this->world));
  state = (state + 1) % distance;
  int next_row[16];
  if (state == 0) {
    int start = rand() % (16 - gap);
    for (int i = 0; i < 16; i++) {
      if (i <= start || i > start + gap) {
        this->world[15][i] = 1;
      } else {
        this->world[15][i] = 0;
      }
    }
  }
}

void World::print(CRGB *leds) const {
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      int index;
      if (i % 2 == 0) { // even row
        index = j + (i * 16);
      } else {
        index = (i * 16) + 15 - j;
      }
      if (this->world[i][j] == 1) {
        leds[index] = CRGB::Green;
      } else {
        leds[index] = CRGB::Black;
      }
    }
  }
}

void World::reset(){
  state = 0;
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      this->world[i][j] = 0;
    }
  }
}