#ifndef BIRD_HPP
#define BIRD_HPP
#include <FastLED.h>

class Bird {
private:
  int position;
  int min_position;
  int max_position;
  int starting_position;
  bool do_fly_up;
  bool prevent_falling;

public:
  Bird(int min_position, int max_position, int starting_position) {
    this->min_position = min_position;
    this->max_position = max_position;
    this->starting_position = starting_position;
    this->position = starting_position;
    this->do_fly_up = false;
    this->prevent_falling = false;
  }

  void reset();
  void flyUp(bool button_pressed);
  void flap();
  int getPosition() { return this->position; }
  void print(CRGB *leds) { leds[this->position] = CRGB::Brown; }
  void hide(CRGB *leds) { leds[this->position] = CRGB::Black; }
};

#endif // BIRD_HPP