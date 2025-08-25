#pragma once
#include <Arduino.h>
#include <FS.h>

class PocketmageSD;
void wireSD();
void setupSD();
PocketmageSD& SD();

class PocketmageOled;
class PocketmageEink;

class PocketmageSD {
public:
  explicit PocketmageSD() {}

  // Wire up external buffers/state used to read from globals
  void setFileSys(fs::FS* fileSys)      { fileSys_ = fileSys;}            // reference to fs::FS*
  void setOled(PocketmageOled* oled)     { oled_ = oled;}                // reference to pocketmage oled object
  void setOled(PocketmageOled& oled)     { oled_ = &oled;}                // overloaded reference to pocketmage oled object
  void setEink(PocketmageEink* eink)     { eink_ = eink;}                // reference to pocketmage oled object
  void setEink(PocketmageEink& eink)     { eink_ = &eink;}                // overloaded reference to pocketmage oled object
  void setEditingFile(String* editingFile) { editingFile_ = editingFile;}        // reference to editingFile
  void setFilesList(String* filesList)    {filesList_ = filesList;}                           // reference to filesList
  void setNoSD(volatile bool* noSD)           {noSD_ = noSD;}                           // reference to noSD
  void setNoTimeout(bool* noTimeout)         {noTimeout_ = noTimeout;}                 // reference to noTimeout

  void listDir(fs::FS &fs, const char *dirname);
  void readFile(fs::FS &fs, const char *path);
  String readFileToString(fs::FS &fs, const char *path);
  void writeFile(fs::FS &fs, const char *path, const char *message);
  void appendFile(fs::FS &fs, const char *path, const char *message);
  void renameFile(fs::FS &fs, const char *path1, const char *path2);
  void deleteFile(fs::FS &fs, const char *path);

private:
  fs::FS*                       fileSys_          = nullptr;    // pass &SD_MMC

  PocketmageOled*               oled_             = nullptr;
  PocketmageEink*               eink_             = nullptr;

  uint8_t                       fileIndex_        = 0;
  String                        excludedFiles_[3] = { "/temp.txt", "/settings.txt", "/tasks.txt" };

  // App state
  String*                       editingFile_      = nullptr;
  String*                       filesList_        = nullptr;  // size MAX_FILES

  // Flags / counters
  volatile bool*                noSD_             = nullptr;
  bool*                         noTimeout_        = nullptr; 
};
