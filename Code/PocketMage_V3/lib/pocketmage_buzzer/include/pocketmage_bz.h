//  888888ba  dP     dP d8888888P d8888888P  88888888b  888888ba  //
//  88    `8b 88     88      .d8'      .d8'  88         88    `8b //
// a88aaaa8P' 88     88    .d8'      .d8'   a88aaaa    a88aaaa8P' //
//  88   `8b. 88     88  .d8'      .d8'      88         88   `8b. //
//  88    .88 Y8.   .8P d8'       d8'        88         88     88 //
//  88888888P `Y88888P' Y8888888P Y8888888P  88888888P  dP     dP //

#pragma once
#include <Arduino.h>
#include <Buzzer.h>

enum class Jingle : uint8_t { Startup, Shutdown };

// ===================== BZ CLASS =====================
class PocketmageBZ {
public:
  explicit PocketmageBZ(Buzzer &bz) : buzzer_(bz) {}

  // Main methods
  void wireBZ(Buzzer* hw);
  void playJingle(Jingle jingle);

private:
    Buzzer      &buzzer_;
};

void wireBZ();
void setupBZ();
PocketmageBZ& BZ();