TM1637Display display(DISPLAY_CLK, DISPLAY_DTA);
uint8_t display_data[] = { 0xff, 0xff, 0xff, 0xff };


void display_code(String);

void setup_display() {
  // Setup display:
  display.clear();
  display.setBrightness(DISPLAY_BRIGHTNESS);
}

String valid_codes[VALID_CODE_COUNT];
unsigned long last_roll = 0;

String generate_random_code() {
  String code = "";
  for (int i = 0; i < CODE_LENGTH; i++) {
    code += String(random(10));
  }
  return code;
}


void rolling_code_handler() {
  // Check if the codes need to roll.
  if (millis() - last_roll >= ROLL_FREQUENCY) {
    valid_codes[0] = generate_random_code();

    Serial.print("Displaying: ");
    Serial.println(valid_codes[0]);

    display_code(valid_codes[0]);
    last_roll = millis();
  }
}


void clear_codes() {
  for (int i = 1; i < VALID_CODE_COUNT; i++) {
    valid_codes[i] = "";
    for (int j = 0; j < CODE_LENGTH; j++) {
      valid_codes[i] += "X";
    }
  }
}


bool validate_code(String code) {
  for (int i = 0; i < VALID_CODE_COUNT; i++) {
    if (code == valid_codes[i])
    {
      chirp_loop(20, 10, 15);
      return true;
    }
  }
  chirp(500);
  return false;
}


void display_code(String code) {
  if (CODE_LENGTH != DISPLAY_DIGIT_COUNT)
    return;

  int length = code.length();

  int digits[CODE_LENGTH];

  for (int i = 0; i < length && i < CODE_LENGTH; i++) {
    display_data[i] = display.encodeDigit(code.charAt(i) - '0');
  }

  display.setSegments(display_data);
}