String get_last_n_keypresses(int);
bool validate_code(String);
void clear_keypad_buffer();
void shift_buffer();

// Keypad
const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 3;

// Initialize the keypad:
char KEYPAD_KEYS[KEYPAD_ROWS][KEYPAD_COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' },
  { '*', '0', '#' },
};

byte KEYPAD_ROW_PI[KEYPAD_ROWS] = { KEYPAD_7, KEYPAD_6, KEYPAD_5, KEYPAD_4 };
byte KEYPAD_COL_INS[KEYPAD_COLS] = { KEYPAD_3, KEYPAD_2, KEYPAD_1 };

Keypad keypad = Keypad(makeKeymap(KEYPAD_KEYS), KEYPAD_ROW_PI, KEYPAD_COL_INS, KEYPAD_ROWS, KEYPAD_COLS);

char keypad_buffer[KEYPAD_BUFFER_SIZE];
unsigned int buffer_idx = 0;


void keypad_handler() {
  char key = keypad.getKey();

  if (key == KEYPAD_ENTER) {
    String code = get_last_n_keypresses(CODE_LENGTH);
    Serial.print("Verifying code entry: ");
    Serial.println(code);

    if (validate_code(code)) {
      Serial.print("Valid code entered: ");
      Serial.println(code);
      toggle_lock();
    }
    if (CLEAR_KEYPAD_ON_FAIL) {
      clear_keypad_buffer();
    }

    return;
  }

  if (key) {
    chirp(5);
    if (buffer_idx >= KEYPAD_BUFFER_SIZE - 1)
      shift_buffer();

    keypad_buffer[buffer_idx++] = key;
    keypad_buffer[buffer_idx] = '\0';  // Null terminate the buffer
  }
}

void shift_buffer() {
  for (int i = 1; i < KEYPAD_BUFFER_SIZE; ++i) {
    keypad_buffer[i - 1] = keypad_buffer[i];
  }
  keypad_buffer[KEYPAD_BUFFER_SIZE - 1] = '\0';  // Null terminate the buffer
  buffer_idx--;
}

String get_last_n_keypresses(int n) {
  if (n <= 0) return "";

  if (n >= buffer_idx) {
    return String(keypad_buffer);
  } else {
    return String(keypad_buffer + buffer_idx - n);
  }
}


void clear_keypad_buffer() {
  for (int i = 0; i < KEYPAD_BUFFER_SIZE; i++) {
    keypad_buffer[i] = 'X';
  }
}