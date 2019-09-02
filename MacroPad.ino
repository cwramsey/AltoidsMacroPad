#include <Keyboard.h>
#include <AceButton.h>
using namespace ace_button;

const int WINDOWS = 1;
const int OSX = 2;
const int CHERRY_ONE = 2;
const int CHERRY_TWO = 14;
const int CHERRY_THREE = 10;
const int KALIH_ONE = 4;
const int KALIH_TWO = 6;
const int KALIH_THREE = 8;
const int NUM_BUTTONS = 6;
const int LED_PIN = 17;

const int pins[] = {CHERRY_ONE, CHERRY_TWO, CHERRY_THREE, KALIH_ONE, KALIH_TWO, KALIH_THREE};

int os = WINDOWS;


AceButton buttons[NUM_BUTTONS];
void handleEvent(AceButton*, uint8_t, uint8_t);

void setup() {

  pinMode(LED_PIN, OUTPUT); 
  Serial.begin(9600);

  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    // Button uses the built-in pull up register.
    pinMode(pins[i], INPUT_PULLUP);

    // initialize the corresponding AceButton
    buttons[i].init(pins[i], HIGH, i);
  }

  ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  Keyboard.begin();
}

void loop() {
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    buttons[i].check();
  }
}

uint8_t ctrl() {
  if (os == WINDOWS) {
    return KEY_LEFT_CTRL;
  }

  return KEY_LEFT_GUI;
}

void changeOS() {
  if (os == WINDOWS) {
    os = OSX;
    return;
  }

  os = WINDOWS;
}

void copy() {
  Serial.println("Copying");
  Keyboard.press(ctrl());
  Keyboard.press('c');
  delay(100);
  Keyboard.releaseAll();
}

void paste() {
  Serial.println("Pasting");
  Keyboard.press(ctrl());
  Keyboard.press('v');
  delay(100);
  Keyboard.releaseAll();
}

void selectLine() {
  Keyboard.write(KEY_HOME);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_END);
  delay(100);
  Keyboard.releaseAll();
}

void handleEvent(AceButton* button, uint8_t eventType,
    uint8_t /*buttonState*/) {

      uint8_t id = button->getId();

      Serial.println("Button pressed");
      Serial.println(id);

      if (eventType == AceButton::kEventPressed) {
        switch(id) {
          case 0:
            selectLine();
            break;
          case 1:
            Keyboard.print("Cherry Two");
            break;
          case 2:
            Serial.println("Changing os to");
            Serial.println(os);
            changeOS();
            break;
          case 3:
            copy();
            break;
          case 4:
            paste();
            break;
          case 5:
            Keyboard.print("Kalih Three");
            break;
        }
      }
}
