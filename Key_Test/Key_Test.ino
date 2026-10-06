#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'A', 'B', 'C', 'D'},
  {'1', '2', '3', '#'},
  {'4', '5', '6', '0'},
  {'7', '8', '9', '*'}
};

byte rowPins[ROWS] = {4, 5, 6, 7};
byte colPins[COLS] = {15, 16, 17, 18};

Keypad keypad = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== 4x4 KEYPAD TEST ===");
  Serial.println("Press a key...");
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.print("KEY: ");
    Serial.println(key);
  }
}