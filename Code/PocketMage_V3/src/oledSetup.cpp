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

// e-ink text width wrapper work with Eink display
static uint16_t einkMeasureWidth(const String& s) {
  int16_t x1, y1; uint16_t w, h;
  display.getTextBounds(s, 0, 0, &x1, &y1, &w, &h);
  return w;
}

// wire up oled class to globals
void wireOled() {
  oled
    .setAllLines(&allLines)
    .setDynamicScroll(&dynamicScroll)
    .setBattery(&battState, batt_allArray, kBattIconCount)
    .setKeyboardStateGetter([] { return static_cast<int>(CurrentKBState); })
    .setMSC(&mscEnabled)
    .setSD(&SDActive)
    .setScrollBitmap(scrolloled0)
    .setReferenceWidth(display.width())
    .setMeasureTextWidth(einkMeasureWidth)
    .setMaxCharsPerLineEinkGetter([]{ return getEink().maxCharsPerLine(); })
    .setClock(&rtc, &SYSTEM_CLOCK, &SHOW_YEAR, daysOfTheWeek);
}

// oled object reference for other apps
PocketmageOled& getOled() { return oled; }
