#include <pocketmage.h>

//Adafruit_TCA8418 keypad;
// Initialization of sd class
static PocketmageKB kb(keypad);

void setupKB() {
  if (!keypad.begin(TCA8418_DEFAULT_ADDR, &Wire)) {
    Serial.println("Error Initializing the Keyboard");
    OLED().oledWord("Keyboard INIT Failed");
    delay(1000);
    while (1);
  }
  keypad.matrix(4, 10);
  wireKB();
  attachInterrupt(digitalPinToInterrupt(KB_IRQ), []{ KB().TCA8418_irq(); }, CHANGE);
  keypad.flush();
  keypad.enableInterrupts();
}

// Wire function 
void wireKB() {
    kb.setTCA8418EventFlag(&TCA8418_event);
    kb.setPrevTimeMillis(&prevTimeMillis);
    kb.setKeyboardStateGetter([] { return static_cast<int>(CurrentKBState); });
}

void IRAM_ATTR KB_irq_handler() {
  kb.TCA8418_irq();
}

// Access for other apps
PocketmageKB& KB() { return kb; }
