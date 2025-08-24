#include "globals.h"
#include "sdmmc_cmd.h"

// ===================== DISPLAY =====================
// Main e-ink display object
GxEPD2_BW<GxEPD2_310_GDEQ031T10, GxEPD2_310_GDEQ031T10::HEIGHT> display(GxEPD2_310_GDEQ031T10(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY));
// Fast full update flag for e-ink
volatile bool GxEPD2_310_GDEQ031T10::useFastFullUpdate = true;
// 256x32 SPI OLED display object
U8G2_SSD1326_ER_256X32_F_4W_HW_SPI u8g2(U8G2_R2, OLED_CS, OLED_DC, OLED_RST);

// ===================== INPUT DEVICES =====================
// Matrix keypad controller
Adafruit_TCA8418 keypad;
// Key layouts
char keysArray[4][10] = {
    { 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p' },
    { 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',   8 },  // 8:BKSP
    {   9, 'z', 'x', 'c', 'v', 'b', 'n', 'm', '.',  13 },  // 9:TAB, 13:CR
    {   0,  17,  18, ' ', ' ', ' ',  19,  20,  21,   0 }   // 17:SHFT, 18:FN, 19:<-, 20:SEL, 21:->
};
char keysArraySHFT[4][10] = {
    { 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P' },
    { 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',   8 },
    {   9, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',',  13 },
    {   0,  17,  18, ' ', ' ', ' ',  19,  20,  21,   0 }
};
char keysArrayFN[4][10] = {
    { '1', '2', '3', '4', '5', '6', '7',  '8',  '9', '0' },
    { '#', '!', '$', ':', ';', '(', ')', '\'', '\"',   8 },
    {  14, '%', '_', '&', '+', '-', '/',  '?',  ',',  13 },
    {   0,  17,  18, ' ', ' ', ' ',  12,    7,    6,   0 }
};
// Capacitive touch slider
Adafruit_MPR121 cap = Adafruit_MPR121();

// ===================== AUDIO =====================
// Buzzer for sound feedback
Buzzer buzzer(17);

// ===================== RTC =====================
// Real-time clock chip
RTC_PCF8563 rtc;
// Day names
const char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

// ===================== USB & STORAGE =====================
// USB mass storage controller
USBMSC msc;
bool mscEnabled = false;          // Is USB MSC active?
sdmmc_card_t* card = nullptr;     // SD card pointer

// ===================== SYSTEM SETTINGS =====================
// Persistent preferences (NVS)
Preferences prefs;
int TIMEOUT;              // Auto sleep timeout (seconds)
bool DEBUG_VERBOSE;       // Extra debug output
bool SYSTEM_CLOCK;        // Show clock on screen
bool SHOW_YEAR;           // Show year in clock
bool SAVE_POWER;          // Enable power saving mode
bool ALLOW_NO_MICROSD;    // Allow running without SD card
bool HOME_ON_BOOT;        // Start home app on boot
int OLED_BRIGHTNESS;      // OLED brightness (0-255)
int OLED_MAX_FPS;         // OLED max FPS

// ===================== SYSTEM STATE =====================
// E-Ink refresh control
volatile int einkRefresh = FULL_REFRESH_AFTER; // Partial/full refresh counter
int OLEDFPSMillis = 0;            // Last OLED FPS update time
int KBBounceMillis = 0;           // Last keyboard debounce time
volatile int timeoutMillis = 0;   // Timeout tracking
volatile int prevTimeMillis = 0;  // Previous time for timeout
volatile bool TCA8418_event = false;  // Keypad interrupt event
volatile bool PWR_BTN_event = false;  // Power button event
volatile bool SHFT = false;           // Shift key state
volatile bool FN = false;             // Function key state
volatile bool newState = false;       // App state changed
bool noTimeout = false;               // Disable timeout
volatile bool OLEDPowerSave = false;  // OLED power save mode
volatile bool disableTimeout = false; // Disable timeout globally
volatile int battState = 0;           // Battery state
volatile int prevBattState = 0;       // Previous battery state
unsigned int flashMillis = 0;         // Flash timing
int prevTime = 0;                     // Previous time (minutes)
uint8_t prevSec = 0;                  // Previous seconds
TaskHandle_t einkHandlerTaskHandle = NULL; // E-Ink handler task
uint8_t partialCounter = 0;           // Counter for partial refreshes
volatile bool forceSlowFullUpdate = false; // Force slow full update

<<<<<<< HEAD
const String appStateNames[] = { "txt", "filewiz", "usb", "bt", "settings", "tasks", "calendar", "journal", "lexicon", "calc" };
const unsigned char *appIcons[10] = { _homeIcons2, _homeIcons3, _homeIcons4, _homeIcons5, _homeIcons6, taskIconTasks0, _homeIcons7, _homeIcons8, _homeIcons9,_homeIcons10};
=======
// ===================== KEYBOARD STATE =====================
char currentKB[4][10];            // Current keyboard layout
KBState CurrentKBState = NORMAL;  // Current keyboard state
>>>>>>> upstream/main

// ===================== FILES & TEXT =====================
volatile bool SDCARD_INSERT = false;  // SD card inserted event
bool noSD = false;                    // No SD card present
volatile bool SDActive = false;       // SD card active
String editingFile;                   // Currently edited file
const GFXfont *currentFont = (GFXfont *)&FreeSerif9pt7b; // Current font
uint8_t maxCharsPerLine = 0;          // Max chars per line (display)
uint8_t maxLines = 0;                 // Max lines per screen
uint8_t fontHeight = 0;               // Font height in pixels
uint8_t lineSpacing = 6;              // Line spacing in pixels
String workingFile = "";              // Working file name
String filesList[MAX_FILES];          // List of files

// ===================== APP STATES =====================
const String appStateNames[] = { "txt", "filewiz", "usb", "bt", "settings", "tasks", "calendar", "journal", "lexicon" }; // App state names
const unsigned char *appIcons[9] = { _homeIcons2, _homeIcons3, _homeIcons4, _homeIcons5, _homeIcons6, taskIconTasks0, _homeIcons7, _homeIcons8, _homeIcons9}; // App icons
AppState CurrentAppState;             // Current app state

// ===================== TXT APP =====================
volatile bool newLineAdded = true;           // New line added in TXT
std::vector<String> allLines;                // All lines in TXT
volatile long int dynamicScroll = 0;         // Dynamic scroll offset
volatile long int prev_dynamicScroll = 0;    // Previous scroll offset
int lastTouch = -1;                          // Last touch event
unsigned long lastTouchTime = 0;             // Last touch time

// ===================== TASKS APP =====================
std::vector<std::vector<String>> tasks;      // Task list

<<<<<<< HEAD
// <HOME.cpp>
HOMEState CurrentHOMEState = HOME_HOME;

// <FILEWIZ.cpp> 
FileWizState CurrentFileWizState = WIZ0_;
String workingFile = "";

// <SETTINGS.cpp>
SettingsState CurrentSettingsState = settings0;

// <CALENDAR.cpp>
CalendarState CurrentCalendarState = MONTH;

// <LEXICON.cpp>
LexState CurrentLexState = MENU;


  // <CALC.cpp>
  CALCState CurrentCALCState = CALC0;
  int calcSwitchedStates = 0;
  int trigType = 1;
  int refresh_count = 0;
  std::vector<String> allLinesCalc;
  std::vector<String> prevTokens;
  String cleanExpression = "";
  String calculatedResult = "";
  String prevLine = "";
  char bufferString[20];
  std::map<String, float> variables= {};
  std::set<String> constantsCalc = {
         "inf", "-inf", "pi", "e", "ans"
  };
  std::set<String> operatorsCalc = {
        "+", "-", "'", "/", "E", "%", "=", "!", "\""
  };
  std::set<String> functionsCalc = {
        // trig
        "sin", "cos", "tan", "asin", "acos", "atan",
        "sinh", "cosh", "tanh", "sec", "csc", "cot", 
        "sech", "csch", "coth", "asec", "acsc", "acot",
        // scientific
        "ln", "log", "sqrt", "cbrt", "abs", "exp",
        "round", "min", "max", "pow", "rand",
        // fun
        "dice", "pick"
  };
  std::map<String, int> precedenceCalc = {
        {":", 0}, {"+", 1}, {"-", 1}, {"'", 2}, {"/", 2}, {"%", 2}, {"E", 3}, {"\"", 3}, {"!", 4}, {"~neg~",4}
    };
  std::vector<String>  helpText = {
    "\n",
    "    vvv scroll down vvv\n",
    "This is the help screen\n",
    "\n",
    "press enter to exit help\n",
    "\n",
    "NOTES:\n",
    "  /6 -> EXIT APP\n",
    "\n",
    "  scroll <- ->\n",
    "\n",
    "  ',,' equals '.'\n",
    "\n",
    "    vvv scroll down vvv\n",
    "\n",
    "\n",
    "commands:\n",
    "\n",
    "    '/' + <command> \n",
    " 0 : standard\n",
    "  \n",
    " 1 : programming\n",
    "    (not implemented) \n",
    " 2 : scientific \n",
    "\n",
    " 3 : conversions \n",
    "    (not implemented) \n",
    " 4 : help\n",
    "  \n",
    " 5 : export to txt\n",
    "  \n",
    " 6 : EXIT\n",
    "  \n",
    " rad : radian mode\n",
    "  \n",
    " deg : degree mode\n",
    "  \n",
    " grad : gradian mode\n",
    "  \n",
    "keyboard changes:\n",
    "  default kb state:FUNC\n",
    "  tab && fn(tab) == font\n",
    "  bksp == fn(bskp)\n",
    "  left arrow scroll  ^ \n",
    "  right arrow scroll v \n",
    "operators:\n",
    "  \n",
    " - (unary included)\n",
    " +\n",
    " * (type: ' or a(b))\n",
    " /\n",
    " %\n",
    " !\n",
    " !! repeat prev line\n",
    " ^ (type: \")\n",
    " = (type: :)\n",
    " E (aEb)\n",
    "\n",
    "functions: \n",
    "\n",
    " sin(a) asin(a) sinh(a)\n",
    " csc(a) acsc(a) csch(a)\n",
    " cos(a) acos(a) cosh(a)\n",
    " sec(a) asec(a) sech(a)\n",
    " tan(a) atan(a) tanh(a)\n",
    " cot(a) acot(a) coth(a)\n",
    " sqrt(a)\n",
    " exp(a)     log(a)\n",
    " pow(a,b)   log10(a\n",
    " floor(a)   ceil(a)\n",
    " min(a)     max(a)\n",
    " round(a)\n",
    " abs(a)\n",
    " rand(a,b) from min a -> b\n",
    " dice(a,b) a: num b:sides\n",
    " pick(a,n) a: choices\n",
    "           n: no. choices\n",
    "\n",
    "variables: \n",
    "\n",
    "must start with alpha char \n",
    "excluding constants \n",
    "    ^^^ scroll up ^^^"
  };


// <JOURNAL.cpp>
JournalState CurrentJournalState = J_MENU;

=======
// ===================== HOME APP =====================
HOMEState CurrentHOMEState = HOME_HOME;      // Current home state
>>>>>>> upstream/main
