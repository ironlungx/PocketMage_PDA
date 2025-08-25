#include <pocketmage.h>


// Initialization of bz class
static PocketmageBZ bz(buzzer);

void setupBZ() {
  BZ().playJingle(Jingle::Startup);
}


// Wire function 
void wireBZ() {
}

// Access for other apps
PocketmageBZ& BZ() { return bz; }
