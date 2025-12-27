/*

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

// ====== PANEL GEOMETRY ======
#define PANEL_WIDTH   32
#define PANEL_HEIGHT  16
#define PANEL_CHAIN   1

// ====== YOUR PIN MAPPING (EDIT GPIOs TO MATCH WIRING) ======
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


void setup() {
  matrix = MatrixPanel_I2S_DMA(mxconfig);
  matrix.begin();
  matrix.setBrightness8(255);  // Full brightness
  matrix.setTextWrap(false);
  matrix.setTextSize(1);
  matrix.fillScreen(0);
}

void loop() {

  matrix.fillScreen(0);

  // ---- Static color wheel for text ----
  uint8_t r = 0, g = 0, b = 0;
  static uint16_t hue = 0;
  uint16_t h = hue % 768;
  if (h < 256) { r = 255; g = h; b = 0; }
  else if (h < 512) { r = 511 - h; g = 255; b = 0; }
  else { r = 0; g = 255; b = h - 512; }
  
  uint16_t textColor = matrix.color565(r, g, b);

  // ---- STATIC TEXT IN TWO ROWS (NO SCROLLING) ----
  matrix.setTextColor(textColor);
  
  // Row 1: Left-aligned at top
  matrix.setCursor(0, 0);
  matrix.print("justdo");
  
  // Row 2: Right-aligned at bottom  
  matrix.setCursor(0, 8);
  matrix.print("electronic");

  hue = (hue + 7) & 0x3FF;  // Cycle hue for color change
  delay(30);
}

*/

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

// ====== PANEL GEOMETRY ======
#define PANEL_WIDTH   32
#define PANEL_HEIGHT  16
#define PANEL_CHAIN   1

// ====== YOUR PIN MAPPING (EDIT GPIOs TO MATCH WIRING) ======
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

// ====== SCROLLTEXT STATE ======
const char row1Text[] PROGMEM = "justdo electronics   ";  // Extra spaces for smooth scroll
const char row2Text[] PROGMEM = "Design By .........  ";  // Same text, same direction

int16_t textX1 = PANEL_WIDTH;      // Row 1: Starts off right
int16_t textX2 = PANEL_WIDTH;      // Row 2: Starts off right (same timing)
int16_t textMin = -(strlen_P(row1Text) * 8);

void setup() {
  matrix = MatrixPanel_I2S_DMA(mxconfig);
  matrix.begin();
  matrix.setBrightness8(255);
  matrix.setTextWrap(false);
  matrix.setTextSize(1);
  matrix.fillScreen(0);
}

void loop() {
  matrix.fillScreen(0);

  // ---- Color wheel ----
  uint8_t r = 0, g = 0, b = 0;
  static uint16_t hue = 0;
  uint16_t h = hue % 768;
  if (h < 256) { r = 255; g = h; b = 0; }
  else if (h < 512) { r = 511 - h; g = 255; b = 0; }
  else { r = 0; g = 255; b = h - 512; }
  
  uint16_t textColor = matrix.color565(r, g, b);

  // ---- BOTH ROWS: SAME DIRECTION SCROLL (LEFT → RIGHT) ----
  matrix.setTextColor(textColor);
  
  // Row 1: Top line scrolls left to right
  matrix.setCursor(textX1, 0);
  matrix.print(row1Text);
  if (--textX1 < textMin) textX1 = PANEL_WIDTH;
  
  // Row 2: Bottom line scrolls left to right (SAME direction)
  matrix.setCursor(textX2, 8);
  matrix.print(row2Text);
  if (--textX2 < textMin) textX2 = PANEL_WIDTH;

  hue = (hue + 7) & 0x3FF;
  delay(50);
}


