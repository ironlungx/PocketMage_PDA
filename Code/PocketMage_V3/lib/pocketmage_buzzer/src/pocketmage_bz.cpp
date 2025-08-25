
#include <pocketmage_bz.h>

void PocketmageBZ::playJingle(Jingle j) {
  buzzer_.begin(0);
  switch (j) {
    case Jingle::Startup:
      buzzer_.sound(NOTE_A8,120);
      buzzer_.sound(NOTE_B8,120);
      buzzer_.sound(NOTE_C8,120);
      buzzer_.sound(NOTE_D8,120);
      break;
    case Jingle::Shutdown:
      buzzer_.sound(NOTE_D8,120);
      buzzer_.sound(NOTE_C8,120);
      buzzer_.sound(NOTE_B8,120);
      buzzer_.sound(NOTE_A8,120);
      break;
  }
  buzzer_.sound(0,80);
  buzzer_.end(0);
}