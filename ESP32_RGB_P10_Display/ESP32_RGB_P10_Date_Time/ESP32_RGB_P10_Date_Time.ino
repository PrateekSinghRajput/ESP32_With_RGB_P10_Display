/*

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <WiFi.h>
#include <time.h>

// ====== PANEL GEOMETRY ======
#define PANEL_WIDTH   32
#define PANEL_HEIGHT  16
#define PANEL_CHAIN   1

// ====== YOUR PIN MAPPING ======
#define R1_PIN  21
#define G1_PIN  22
#define B1_PIN   5
#define R2_PIN   4
#define G2_PIN  16
#define B2_PIN  17
#define A_PIN   23
#define B_PIN   19
#define C_PIN   18
#define D_PIN   25
#define E_PIN  -1
#define LAT_PIN 27
#define OE_PIN  14
#define CLK_PIN 12

HUB75_I2S_CFG::i2s_pins panelPins = {
  R1_PIN, G1_PIN, B1_PIN,
  R2_PIN, G2_PIN, B2_PIN,
  A_PIN,  B_PIN,  C_PIN,
  D_PIN,  E_PIN,
  LAT_PIN, OE_PIN, CLK_PIN
};

MatrixPanel_I2S_DMA matrix;
HUB75_I2S_CFG mxconfig(PANEL_WIDTH, PANEL_HEIGHT, PANEL_CHAIN, panelPins);

// ====== WIFI & NTP SETTINGS ======
const char* ssid = "Prateek";
const char* password = "justdoelectronics@#12345";
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;  // IST
const int   daylightOffset_sec = 0;

// ====== DISPLAY STATE ======
struct tm timeinfo;
bool timeSynced = false;
unsigned long lastNTPSync = 0;
const unsigned long NTP_SYNC_INTERVAL = 3600000;

void setup() {
  Serial.begin(115200);
  
  matrix = MatrixPanel_I2S_DMA(mxconfig);
  matrix.begin();
  matrix.setBrightness8(100);
  matrix.setTextWrap(false);
  matrix.setTextSize(1);
  matrix.fillScreen(0);
  
  matrix.setTextColor(matrix.color565(0,255,255));
  matrix.setCursor(1, 4);
  matrix.print("Connecting WiFi");
  matrix.setCursor(3, 12);
  matrix.print("NTP Sync...");
  
  connectWiFi();
  syncNTP();
  Serial.println("Setup complete!");
}

void loop() {
  if (millis() - lastNTPSync > NTP_SYNC_INTERVAL) {
    syncNTP();
  }
  
  matrix.fillScreen(0);
  
  if (timeSynced) {
    updateTimeDisplay();
  } else {
    matrix.setTextColor(matrix.color565(255,0,0));
    matrix.setCursor(8, 5);
    matrix.print("TIME ERROR");
    matrix.setCursor(2, 11);
    matrix.print("Check WiFi/NTP");
  }
  
  delay(1000);
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting WiFi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    matrix.fillScreen(0);
    matrix.setTextColor(matrix.color565(0,255,0));
    matrix.setCursor(attempts, 7);
    matrix.print("Connecting");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi OK! IP: " + WiFi.localIP().toString());
    matrix.fillScreen(0);
    matrix.setTextColor(matrix.color565(0,255,0));
    matrix.setCursor(10, 7);
    matrix.print("WiFi OK!");
    delay(1500);
  }
}

void syncNTP() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.print("NTP sync");
  
  time_t now = time(nullptr);
  int attempts = 0;
  while (now < 8 * 3600 * 2 && attempts < 10) {
    delay(2000);
    Serial.print(".");
    now = time(nullptr);
    attempts++;
  }
  
  if (now < 8 * 3600 * 2) {
    Serial.println("\nNTP failed!");
    timeSynced = false;
  } else {
    Serial.println("\nNTP OK!");
    timeSynced = true;
    lastNTPSync = millis();
  }
}

void updateTimeDisplay() {
  if (!getLocalTime(&timeinfo)) {
    timeSynced = false;
    return;
  }
  
  char timeStr[9];   // HH:MM:SS
  char dateStr[9];   // DD/MM/YY
  
  strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);  // Only HH:MM for better fit
  strftime(dateStr, sizeof(dateStr), "%d/%m", &timeinfo);
  
  // Perfect fit layout for 32x16 panel
  uint16_t timeColor = matrix.color565(0, 255, 255);  // Cyan
  uint16_t dateColor = matrix.color565(255, 255, 0);  // Yellow
  uint16_t secColor  = matrix.color565(0, 255, 0);    // Green
  
  // TIME: HH:MM (Size 1, positions 1-15)
  matrix.setTextSize(1);
  matrix.setTextColor(timeColor);
  matrix.setCursor(1, 2);
  matrix.print(timeStr);
  
  // SECONDS: SS (Size 1, positions 18-23)
  int seconds = timeinfo.tm_sec;
  char secStr[3];
  sprintf(secStr, "%02d", seconds);
  matrix.setTextColor(secColor);
  matrix.setCursor(19, 2);
  matrix.print(secStr);
  
  // DATE: Day DD/MM (Size 1, bottom row)
  matrix.setTextColor(dateColor);
  matrix.setCursor(8, 11);
  matrix.print(dateStr);
  
  // DAY NAME (3 letters, left side bottom)
  const char* days[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
  matrix.setTextColor(matrix.color565(255, 128, 0));  // Orange
  matrix.setCursor(1, 11);
  matrix.print(days[timeinfo.tm_wday]);
  
  // WiFi indicator (top right corner)
  if (WiFi.status() == WL_CONNECTED) {
    matrix.fillRect(29, 0, 3, 3, matrix.color565(0,255,0));
    matrix.drawPixel(30, 1, matrix.color565(255,255,255));
  }
  
  // Sync status indicator
  if (millis() - lastNTPSync < 300000) {  // Green dot if synced < 5 min ago
    matrix.drawPixel(0, 15, matrix.color565(0,255,0));
  }
}

*/

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <WiFi.h>
#include <time.h>

// ====== PANEL GEOMETRY ======
#define PANEL_WIDTH   32
#define PANEL_HEIGHT  16
#define PANEL_CHAIN   1

// ====== YOUR PIN MAPPING ======
#define R1_PIN  21
#define G1_PIN  22
#define B1_PIN   5
#define R2_PIN   4
#define G2_PIN  16
#define B2_PIN  17
#define A_PIN   23
#define B_PIN   19
#define C_PIN   18
#define D_PIN   25
#define E_PIN  -1
#define LAT_PIN 27
#define OE_PIN  14
#define CLK_PIN 12

HUB75_I2S_CFG::i2s_pins panelPins = {
  R1_PIN, G1_PIN, B1_PIN,
  R2_PIN, G2_PIN, B2_PIN,
  A_PIN,  B_PIN,  C_PIN,
  D_PIN,  E_PIN,
  LAT_PIN, OE_PIN, CLK_PIN
};

MatrixPanel_I2S_DMA matrix;
HUB75_I2S_CFG mxconfig(PANEL_WIDTH, PANEL_HEIGHT, PANEL_CHAIN, panelPins);

// ====== WIFI & NTP SETTINGS ======
const char* ssid = "Prateek";
const char* password = "justdoelectronics@#12345";
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;  // IST
const int   daylightOffset_sec = 0;

// ====== TIME STATE ======
struct tm timeinfo;
bool timeSynced = false;
unsigned long lastNTPSync = 0;
const unsigned long NTP_SYNC_INTERVAL = 3600000;

// ====== SCROLL STATE (SECOND ROW) ======
char scrollText[40];         // day + date + spaces
int16_t scrollX = PANEL_WIDTH;
unsigned long lastScroll = 0;
const uint16_t scrollSpeed = 80;   // ms between shifts

void setup() {
  Serial.begin(115200);

  matrix = MatrixPanel_I2S_DMA(mxconfig);
  matrix.begin();
  matrix.setBrightness8(80);
  matrix.setTextWrap(false);
  matrix.setTextSize(1);
  matrix.fillScreen(0);

  matrix.setTextColor(matrix.color565(0,255,255));
  matrix.setCursor(0, 0);
  matrix.print("WiFi...");
  matrix.setCursor(0, 8);
  matrix.print("NTP...");

  connectWiFi();
  syncNTP();
}

void loop() {
  if (millis() - lastNTPSync > NTP_SYNC_INTERVAL) {
    syncNTP();
  }

  if (!timeSynced) {
    matrix.fillScreen(0);
    matrix.setTextColor(matrix.color565(255,0,0));
    matrix.setCursor(2, 4);
    matrix.print("NO TIME");
    matrix.setCursor(0, 11);
    matrix.print("WiFi/NTP ERR");
    delay(500);
    return;
  }

  // update time struct
  if (!getLocalTime(&timeinfo)) {
    timeSynced = false;
    return;
  }

  // clear screen each frame
  matrix.fillScreen(0);

  // ---------- ROW 1: STATIC TIME ----------
  char timeStr[9];           // "HH:MM:SS"
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);
  matrix.setTextSize(1);
  matrix.setTextColor(matrix.color565(255,0 , 0));  // cyan
  // centered approximately in 32 px
  int16_t x = (PANEL_WIDTH - (strlen(timeStr) * 6)) / 2;
  if (x < 0) x = 0;
  matrix.setCursor(x, 1);    // first row
  matrix.print(timeStr);

  // ---------- PREPARE ROW 2 TEXT (DAY + DATE) ----------
  // format: "SAT 27-12-2025   "
  char dateLine[30];
  strftime(dateLine, sizeof(dateLine), "%a %d-%m-%Y", &timeinfo);
  snprintf(scrollText, sizeof(scrollText), "%s   ", dateLine); // add spaces for gap

  // ---------- ROW 2: SCROLLING ----------
  if (millis() - lastScroll > scrollSpeed) {
    lastScroll = millis();
    scrollX--;                             // move left
    int16_t textWidth = strlen(scrollText) * 6;  // 6px per char for size=1
    if (scrollX < -textWidth) {
      scrollX = PANEL_WIDTH;              // restart from right
    }
  }

  matrix.setTextColor(matrix.color565(255, 255, 0)); // yellow
  matrix.setCursor(scrollX, 9);  // second row (y ≈ 8–9)
  matrix.print(scrollText);

  // small WiFi indicator in top-right
  if (WiFi.status() == WL_CONNECTED) {
    matrix.drawPixel(31, 0, matrix.color565(0,255,0));
    matrix.drawPixel(30, 1, matrix.color565(0,255,0));
  }

  delay(20);   // overall refresh
}

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting WiFi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(250);
    Serial.print(".");
  }
  Serial.println(WiFi.status() == WL_CONNECTED ? "\nWiFi OK" : "\nWiFi FAIL");
}

void syncNTP() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.print("NTP sync");
  time_t now = time(nullptr);
  int attempts = 0;
  while (now < 8 * 3600 * 2 && attempts < 10) {
    delay(2000);
    Serial.print(".");
    now = time(nullptr);
  }
  if (now < 8 * 3600 * 2) {
    Serial.println("\nNTP FAIL");
    timeSynced = false;
  } else {
    Serial.println("\nNTP OK");
    timeSynced = true;
    lastNTPSync = millis();
  }
}

