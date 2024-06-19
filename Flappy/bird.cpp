#include "bird.hpp"
#include <FastLED.h>

void Bird::flyUp(bool button_pressed) {
  // Bird stops raising when button is released but should not immediately
  // fall down
  bool old_do_fly_up = do_fly_up;
  this->do_fly_up = button_pressed;
  if (!old_do_fly_up && this->do_fly_up) {
    this->prevent_falling = true;
  }
}

void Bird::flap() {
  if (this->prevent_falling) {
    this->prevent_falling = false;
  } else {
    if (this->do_fly_up) {
      this->position--;
    } else {
      this->position++;
    }
  }

  if (this->position < this->min_position) {
    this->position = this->min_position;
  } else if (this->position >= this->max_position) {
    this->position = this->max_position;
  }
}

void Bird::reset() {
  this->position = this->starting_position;
  this->prevent_falling = false;
}