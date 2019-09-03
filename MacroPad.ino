#include <Keyboard.h>
#include <AceButton.h>
using namespace ace_button;

const String WINDOWS = "windows";
const String OSX = "osx";
const int MODE_ARROWS = 1;
const int MODE_NORMAL = 0;
const int CHERRY_ONE = 2;
const int CHERRY_TWO = 14;
const int CHERRY_THREE = 10;
const int KALIH_ONE = 4;
const int KALIH_TWO = 6;
const int KALIH_THREE = 8;
const int NUM_BUTTONS = 6;
const int LED_PIN = 17;

const int pins[] = {CHERRY_ONE, CHERRY_TWO, CHERRY_THREE, KALIH_ONE, KALIH_TWO, KALIH_THREE};

String os = WINDOWS;
int mode = MODE_NORMAL;

AceButton buttons[NUM_BUTTONS];
void handleEvent(AceButton *, uint8_t, uint8_t);
void handleDoubleClick(AceButton *, uint8_t);
void handleSingleClick(AceButton *, uint8_t);
void handleLongClick(AceButton *, uint8_t);

void setup()
{

  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);

  for (uint8_t i = 0; i < NUM_BUTTONS; i++)
  {
    // Button uses the built-in pull up register.
    pinMode(pins[i], INPUT_PULLUP);

    // initialize the corresponding AceButton
    buttons[i].init(pins[i], HIGH, i);
  }

  ButtonConfig *buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonConfig->setFeature(
      ButtonConfig::kFeatureSuppressClickBeforeDoubleClick);
  // buttonConfig->setFeature(ButtonConfig::kFeatureSuppressAfterClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureSuppressAfterDoubleClick);
  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
  Keyboard.begin();
}

void loop()
{
  for (uint8_t i = 0; i < NUM_BUTTONS; i++)
  {
    buttons[i].check();
  }
}

uint8_t ctrl()
{
  if (os == WINDOWS)
  {
    return KEY_LEFT_CTRL;
  }

  return KEY_LEFT_GUI;
}

void changeOS()
{
  if (os == WINDOWS)
  {
    os = OSX;
    return;
  }

  os = WINDOWS;
}

void changeMode()
{
  if (mode == MODE_ARROWS)
  {
    mode = MODE_NORMAL;
    return;
  }

  mode = MODE_ARROWS;
}

void handleCherryOne()
{
  switch (mode)
  {
  case MODE_NORMAL:
    selectLine();
    break;
  }
}

void handleCherryTwo()
{
  switch (mode)
  {
  case MODE_NORMAL:
    openTerminal();
    break;

  case MODE_ARROWS:
    Keyboard.write(KEY_UP_ARROW);
    break;
  }
}

void handleCherryThree()
{
  Keyboard.print(":AwkwardSealCap:");
}

void handleKalihOne()
{
  switch (mode)
  {
  case MODE_NORMAL:
    cut();
    break;

  case MODE_ARROWS:
    Keyboard.write(KEY_LEFT_ARROW);
    break;
  }
}

void handleKalihTwo()
{
  switch (mode)
  {
  case MODE_NORMAL:
    copy();
    break;

  case MODE_ARROWS:
    Keyboard.write(KEY_DOWN_ARROW);
    break;
  }
}

void handleKalihThree()
{
  switch (mode)
  {
  case MODE_NORMAL:
    paste();
    break;

  case MODE_ARROWS:
    Keyboard.write(KEY_RIGHT_ARROW);
    break;
  }
}

void openTerminal()
{
  Keyboard.write(KEY_LEFT_GUI);
  delay(200);

  if (os == WINDOWS)
  {
    Keyboard.print("cmder");
  }
  else
  {
    Keyboard.print("iterm");
  }

  delay(200);
  Keyboard.write(KEY_RETURN);
}

void cut()
{
  Serial.println("Cutting");
  Keyboard.press(ctrl());
  Keyboard.press('x');
  delay(100);
  Keyboard.releaseAll();
}

void copy()
{
  Serial.println("Copying");
  Keyboard.press(ctrl());
  Keyboard.press('c');
  delay(100);
  Keyboard.releaseAll();
}

void paste()
{
  Serial.println("Pasting");
  Keyboard.press(ctrl());
  Keyboard.press('v');
  delay(100);
  Keyboard.releaseAll();
}

void selectLine()
{
  Keyboard.write(KEY_HOME);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_END);
  delay(100);
  Keyboard.releaseAll();
}

void refresh()
{
  Keyboard.press(ctrl());
  Keyboard.press('r');
  delay(100);
  Keyboard.releaseAll();
}

void handleDoubleClick(AceButton *button, uint8_t id)
{
  switch (id)
  {
  case 2:
    changeOS();
  }
}

void handleLongClick(AceButton *button, uint8_t id)
{
  switch (id)
  {
  case 2:
    changeMode();
  }
}

void handleSingleClick(AceButton *button, uint8_t id)
{
  switch (id)
  {
  case 0:
    handleCherryOne();
    break;
  case 1:
    handleCherryTwo();
    break;
  case 2:
    handleCherryThree();
    break;
  case 3:
    handleKalihOne();
    break;
  case 4:
    handleKalihTwo();
    break;
  case 5:
    handleKalihThree();
    break;
  }
}

void handleEvent(AceButton *button, uint8_t eventType,
                 uint8_t /*buttonState*/)
{

  uint8_t id = button->getId();

  switch (eventType)
  {
  // case AceButton::kEventClicked:
  case AceButton::kEventReleased:
    handleSingleClick(button, id);
    break;

  case AceButton::kEventDoubleClicked:
    handleDoubleClick(button, id);
    break;

  case AceButton::kEventLongPressed:
    handleLongClick(button, id);
    break;
  }
}
