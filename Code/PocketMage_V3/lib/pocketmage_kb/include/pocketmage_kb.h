#pragma once
#include <Arduino.h>
#include <functional>
#include <utility>

class PocketmageKB;
void wireKB();
void setupKB();
void IRAM_ATTR KB_irq_handler();
PocketmageKB& KB();

// forward-declaration
class Adafruit_TCA8418;   

class PocketmageKB {
public:
  explicit PocketmageKB(Adafruit_TCA8418 &kp) : keypad_(kp) {}

  using KbStateFn = std::function<int()>;

  void setTCA8418EventFlag(volatile bool* flag)      { TCA8418_event_ = flag;}
  void setPrevTimeMillis(volatile int* prevTime)     { prevTimeMillis_ = prevTime;}
  void setKeyboardState(int* kbState)                       { kbState_ = kbState;}
  void setKeyboardStateGetter(KbStateFn fn) { kbStateFn_ = std::move(fn);}

  // Main methods
  void wireKB(Adafruit_TCA8418* hw);
  void TCA8418_irq();
  char updateKeypress();


private:
  Adafruit_TCA8418      &keypad_;
  volatile bool*        TCA8418_event_ = nullptr;
  volatile int*         prevTimeMillis_ =  nullptr;
  int*                  kbState_       = nullptr;
  KbStateFn             kbStateFn_     = nullptr;
  int currentKbState() const;
};