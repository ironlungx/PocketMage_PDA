//  oooooooooooo         ooooo ooooo      ooo oooo    oooo  //
//  `888'     `8         `888' `888b.     `8' `888   .8P'   //
//   888                  888   8 `88b.    8   888  d8'     //
//   888oooo8    8888888  888   8   `88b.  8   88888[       //
//   888    "             888   8     `88b.8   888`88b.     //
//   888       o          888   8       `888   888  `88b.   //
//  o888ooooood8         o888o o8o        `8  o888o  o888o  //
#pragma once
#include <Arduino.h>
#include <GxEPD2_BW.h>
#include <vector>

// FONTS
// 3x7
#include "Fonts/Font3x7FixedNum.h"
#include "Fonts/Font4x5Fixed.h"
#include "Fonts/Font5x7Fixed.h"

// 9x7
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>
// 12x7
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>

// Panel type alias
using PanelT   = GxEPD2_310_GDEQ031T10;
using DisplayT = GxEPD2_BW<PanelT, PanelT::HEIGHT>;

class PocketmageEink {
public:
  explicit PocketmageEink(DisplayT& display);

  // Wire up external buffers/state used to read from globals
  void setTextBuffer(std::vector<String>* lines);                 // reference to allLines
  void setEditingFilePtr(String* editingFile);                    // reference to editingFile string
  void setDynamicScroll(volatile long* dynamicScrollPtr);      // reference to dynamicScroll
  void setLineSpacing(uint8_t px);                                // reference to lineSpacing (default 6)
  void setFullRefreshAfter(uint8_t n);                            // reference to FULL_REFRESH_AFTER (default 5)
  void setCurrentFont(const GFXfont* font);                     // reference to currentFont

  void refresh();
  void multiPassRefesh(int passes);
  void setFastFullRefresh(bool setting);
  void statusBar(const String& input, bool fullWindow=false);
  void drawStatusBar(const String& input);
  void computeFontMetrics_();
  void setTXTFont(const GFXfont* font);
  void einkTextDynamic(bool doFull, bool noRefresh=false);
  int  countLines(const String& input, size_t maxLineLength = 29);

  // getters 
  uint8_t maxCharsPerLine() const;
  uint8_t maxLines() const;
  const GFXfont* getCurrentFont();
  
  void forceSlowFullUpdate(bool force);

  uint8_t fullRefreshAfter_ = 5; // FULL_REFRESH_AFTER = 5

private:
  DisplayT& display_;
  std::vector<String>* lines_ = nullptr;
  String* editingFile_ = nullptr;
  const GFXfont* currentFont_ = nullptr;
  volatile long* dynamicScroll_ = nullptr;

  // formerly globals
  uint8_t lineSpacing_ = 6;
  uint8_t partialCounter_ = 0;
  bool    forceSlowFullUpdate_ = false;

  // derived metrics
  uint8_t maxCharsPerLine_ = 0;
  uint8_t maxLines_        = 0;
  uint8_t fontHeight_      = 0;
};