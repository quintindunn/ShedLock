#include <Keypad.h>
#include <TM1637Display.h>

#define BAUD 9600

#define CODE_LENGTH 4
#define ROLL_FREQUENCY 20000  // Roll once a minute.

#define VALID_CODE_COUNT 32

#define LOCKED_ANGLE 90
#define UNLOCKED_ANGLE 0

#define BUZZER_PIN 13
#define INTERNAL_BTN A0

#define KEYPAD_1 8
#define KEYPAD_2 7
#define KEYPAD_3 6
#define KEYPAD_4 5
#define KEYPAD_5 4
#define KEYPAD_6 3
#define KEYPAD_7 2

#define KEYPAD_BUFFER_SIZE 5
#define KEYPAD_ENTER '#'
#define CLEAR_KEYPAD_ON_FAIL true

#define STEPPER_DIR 12
#define STEPPER_STP 11

#define DISPLAY_DTA 10
#define DISPLAY_CLK 9

#define DISPLAY_BRIGHTNESS 0x0f
#define DISPLAY_DIGIT_COUNT 4

#define MICROSTEPS 32
#define STEPS_PER_REV_BASE 200
#define STEPS_PER_REV STEPS_PER_REV_BASE* MICROSTEPS

#define CW 1
#define CCW 0

#define HOMING_DIRECTION CCW
#define HOMING_PIN A1

#define LOCKING_ANGLE_DIFF 240
#define HOME_AFTER_N_ITERATIONS 50
#define HALL_SENSOR A2

#include "logins.h"

#include "generic.h"
#include "lock_mechanism.h"
#include "codes.h"
#include "keypad.h"


void setup_codes() {
  // Define all codes (index 0 is allocated for the rolling code):
  valid_codes[1] = CODE_1;
  valid_codes[2] = CODE_2;
  valid_codes[3] = CODE_3;
  valid_codes[4] = CODE_4;
  valid_codes[5] = CODE_5;
}


void setup_pins() {
  // Set mode of the pins:
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(DISPLAY_DTA, OUTPUT);
  pinMode(DISPLAY_CLK, OUTPUT);
  pinMode(STEPPER_STP, OUTPUT);
  pinMode(STEPPER_DIR, OUTPUT);

  pinMode(INTERNAL_BTN, INPUT);
  pinMode(HALL_SENSOR, INPUT);
  pinMode(HOMING_PIN, INPUT);
}


void setup() {
  Serial.begin(BAUD);
  Serial.println("Lock Server Starting.");

  // Set pin modes
  setup_pins();

  // Home the lock to know where zero is
  home();

  // Blank all buffers with 'X'
  clear_keypad_buffer();
  clear_codes();

  // Setup the 7 segment display
  setup_display();

  // Set the codes
  setup_codes();
}

void loop() {
  if (killed) {
    Serial.println("killed");
    return;
  }
  run_loop();
}

void run_loop() {
  rolling_code_handler();
  keypad_handler();
  internal_btn_handler();
}