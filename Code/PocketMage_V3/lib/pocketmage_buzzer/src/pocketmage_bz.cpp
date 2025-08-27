//  888888ba  dP     dP d8888888P d8888888P  88888888b  888888ba  //
//  88    `8b 88     88      .d8'      .d8'  88         88    `8b //
// a88aaaa8P' 88     88    .d8'      .d8'   a88aaaa    a88aaaa8P' //
//  88   `8b. 88     88  .d8'      .d8'      88         88   `8b. //
//  88    .88 Y8.   .8P d8'       d8'        88         88     88 //
//  88888888P `Y88888P' Y8888888P Y8888888P  88888888P  dP     dP //

#include <pocketmage_bz.h>

// ===================== main functions =====================
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