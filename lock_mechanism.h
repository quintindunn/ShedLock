#define NOHOMING

bool killed = false;

bool door_locked = false;
int iterations = 0;

bool home();

// Rotate the stepper motor with a delay of `int speed` microseconds per step.
void rotate(int degrees, int speed, bool cw) {
  digitalWrite(STEPPER_DIR, (int)cw);

  int steps = (int)((float)STEPS_PER_REV * (float)degrees / 360.0);

  for (int x = 0; x < steps; x++) {
    digitalWrite(STEPPER_STP, HIGH);
    delayMicroseconds(125);
    digitalWrite(STEPPER_STP, LOW);
    delayMicroseconds(125);

    delayMicroseconds(speed);
  }
}

bool is_door_aligned() {
  return !digitalRead(HALL_SENSOR);
}

void lock() {
  if (!is_door_aligned()) {
    Serial.println("Door not aligned!");
    chirp_loop(50, 100, 5);
    return;
  }

  rotate(LOCKING_ANGLE_DIFF, 10, true);
  door_locked = true;
}

void unlock() {
  rotate(LOCKING_ANGLE_DIFF, 10, false);
  door_locked = false;

  // The stepper motor loses its angle after a while, occasionally rehome it.
  if (iterations >= HOME_AFTER_N_ITERATIONS)
    home();
}

// Rotate the stepper motor until it collides with the homing button, setting the starting location.
bool home() {
  Serial.println("homing");
  digitalWrite(STEPPER_DIR, HOMING_DIRECTION);
  int direction_multiplier = HOMING_DIRECTION == CW ? -1 : 1;

  for (int x = 0; x < 360; x++) {
    rotate(1 * direction_multiplier, 10, HOMING_DIRECTION);

    if (!digitalRead(HOMING_PIN)) {
      Serial.println("homing_pressed");
      rotate(4 * -direction_multiplier, 100, CW);
      return false;
    }
  }


  #ifdef DEBUG
    return false;
  #else
    return true;
  #endif
}


void toggle_lock() {
  if (is_door_aligned())
    chirp(10);

  if (door_locked)
    unlock();
  else
    lock();

  iterations++;
}


void internal_btn_handler() {
  // No need for any debounce logic as the time for the stepper motor should suffice.
  if (digitalRead(INTERNAL_BTN))
    toggle_lock();
}