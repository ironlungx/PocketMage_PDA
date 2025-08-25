//    .oooooo.   ooooo        oooooooooooo oooooooooo.    //
//   d8P'  `Y8b  `888'        `888'     `8 `888'   `Y8b   //
//  888      888  888          888          888      888  //
//  888      888  888          888oooo8     888      888  //
//  888      888  888          888    "     888      888  //
//  `88b    d88'  888       o  888       o  888     d88'  //
//   `Y8bood8P'  o888ooooood8 o888ooooood8 o888bood8P'    //     
#include <pocketmage.h>
#include <U8g2lib.h>
#include <GxEPD2_BW.h>
#include <RTClib.h>

constexpr int kBattIconCount = sizeof(batt_allArray) / sizeof(batt_allArray[0]);

// Initialization of oled display class
static PocketmageOled oled(u8g2);

// e-ink text width wrapper to work with Eink display
static uint16_t einkMeasureWidth(const String& s) {
  int16_t x1, y1; uint16_t w, h;
  display.getTextBounds(s, 0, 0, &x1, &y1, &w, &h);
  return w;
}

void setupOled() {
  u8g2.begin();
  u8g2.setBusClock(10000000);
  u8g2.setPowerSave(0);
  u8g2.clearBuffer();
  u8g2.sendBuffer();
  wireOled();

  // SHOW "PocketMage" while DEVICE BOOTS
  OLED().oledWord("   PocketMage   ", true, false);
}

// wire up oled class to globals
void wireOled() {
  oled.setAllLines(&allLines);
  oled.setDynamicScroll(&dynamicScroll);
  oled.setBattery(&battState, batt_allArray, kBattIconCount);
  oled.setKeyboardStateGetter([] { return static_cast<int>(CurrentKBState); });
  oled.setMSC(&mscEnabled);
  oled.setSD(&SDActive);
  oled.setScrollBitmap(scrolloled0);
  oled.setReferenceWidth(display.width());
  oled.setMeasureTextWidth(einkMeasureWidth);
  oled.setMaxCharsPerLineEinkGetter([]{ return EINK().maxCharsPerLine(); });
  oled.setClock(&rtc, &SYSTEM_CLOCK, &SHOW_YEAR, daysOfTheWeek);
}

// oled object reference for other apps
PocketmageOled& OLED() { return oled; }
