//  oooooooooooo         ooooo ooooo      ooo oooo    oooo  //
//  `888'     `8         `888' `888b.     `8' `888   .8P'   //
//   888                  888   8 `88b.    8   888  d8'     //
//   888oooo8    8888888  888   8   `88b.  8   88888[       //
//   888    "             888   8     `88b.8   888`88b.     //
//   888       o          888   8       `888   888  `88b.   //
//  o888ooooood8         o888o o8o        `8  o888o  o888o  //
#include <pocketmage.h>
#include <GxEPD2_BW.h>

// Initialization of eink display class
static PocketmageEink eink(display);

// Wire function 
void wireEink()
{
  eink.setTextBuffer(&allLines);
  eink.setEditingFilePtr(&editingFile);
  eink.setDynamicScroll(&dynamicScroll);
}

// Access for other apps
PocketmageEink& EINK() { return eink; }
