#pragma once
#include <Arduino.h>
#include <Buzzer.h>

class PocketmageBZ;
void wireBZ();
void setupBZ();
PocketmageBZ& BZ();

// forward-declaration
class Buzzer;   

enum class Jingle : uint8_t { Startup, Shutdown };

class PocketmageBZ {
public:
  explicit PocketmageBZ(Buzzer &bz) : buzzer_(bz) {}

  // Main methods
  void wireBZ(Buzzer* hw);
  void playJingle(Jingle jingle);

private:
    Buzzer      &buzzer_;
};