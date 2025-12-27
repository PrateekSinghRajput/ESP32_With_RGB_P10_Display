#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>


// ====== PANEL GEOMETRY ======
#define PANEL_WIDTH   32      // set 32 or 64 etc.
#define PANEL_HEIGHT  16      // set 16, 32, 64 etc.
#define PANEL_CHAIN   1       // number of chained panels


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
#define D_PIN   25   // choose a free GPIO and wire to panel D
#define E_PIN  -1    // no E pin for 1/16‑scan 64×32


#define LAT_PIN 27   // your LAT
#define OE_PIN  14   // your OE
#define CLK_PIN 12   // your CLK


HUB75_I2S_CFG::i2s_pins panelPins = {
  R1_PIN, G1_PIN, B1_PIN,
  R2_PIN, G2_PIN, B2_PIN,
  A_PIN,  B_PIN,  C_PIN,
  D_PIN,  E_PIN,
  LAT_PIN, OE_PIN, CLK_PIN
};


MatrixPanel_I2S_DMA matrix;
HUB75_I2S_CFG mxconfig(
  PANEL_WIDTH,
  PANEL_HEIGHT,
  PANEL_CHAIN,
  panelPins
);


// ====== SCROLLTEXT + BALLS STATE ======
const char str[] PROGMEM = "www.justdoelectronics.com";
int16_t textX   = PANEL_WIDTH;
int16_t textMin = (int16_t)sizeof(str) * -12;
int16_t hue     = 0;


int8_t ball[3][4] = {
  {  3,  0,  1,  1 },
  { 17, 15,  1, -1 },
  { 27,  4, -1,  1 }
};
static const uint16_t PROGMEM ballcolor[3] = {
  0x0080,
  0x0002,
  0x1000
};


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


  // ---- balls ----
  for (uint8_t i = 0; i < 3; i++) {
    matrix.fillCircle(ball[i][0], ball[i][1], 5, pgm_read_word(&ballcolor[i]));
    ball[i][0] += ball[i][2];
    ball[i][1] += ball[i][3];
    if (ball[i][0] <= 0 || ball[i][0] >= (PANEL_WIDTH - 1))  ball[i][2] *= -1;
    if (ball[i][1] <= 0 || ball[i][1] >= (PANEL_HEIGHT - 1)) ball[i][3] *= -1;
  }


  // ---- simple color wheel (approx) ----
  uint8_t r = 0, g = 0, b = 0;
  uint16_t h = hue % 768;  // 0–767
  if      (h < 256) { r = 255;       g = h;          b = 0;   }
  else if (h < 512) { r = 511 - h;  g = 255;        b = 0;   }
  else              { r = 0;        g = 255;        b = h-512; }


  matrix.setTextColor(matrix.color565(r, g, b));
  matrix.setCursor(textX, 1);
  matrix.print(str);


  if (--textX < textMin) textX = PANEL_WIDTH;
  hue = (hue + 7) & 0x3FF;  // wrap


  delay(30);
}
