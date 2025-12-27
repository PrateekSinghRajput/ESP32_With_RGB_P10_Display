#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

// ====== PANEL GEOMETRY ======
#define PANEL_WIDTH   32
#define PANEL_HEIGHT  16
#define PANEL_CHAIN   1

// ====== PIN MAPPING ======
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

// ====== WIFI CREDENTIALS ======
const char* ssid     = "Project";
const char* password = "12345@#12345";

// ====== WEB SERVER ======
AsyncWebServer server(80);

// ====== CONTROL VARIABLES ======
String row1Text = "justdo electronics   ";
String row2Text = "Design By .........  ";
int16_t textX1 = PANEL_WIDTH;
int16_t textX2 = PANEL_WIDTH;
uint8_t brightness = 80;
int scrollDelay = 50;  // ms
String cloudMode = "local";

// text color from web (default white)
uint8_t textR = 255, textG = 255, textB = 255;

void setup() {
  Serial.begin(115200);

  matrix = MatrixPanel_I2S_DMA(mxconfig);
  matrix.begin();
  matrix.setBrightness8(brightness);
  matrix.setTextWrap(false);
  matrix.setTextSize(1);
  matrix.fillScreen(0);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // ========= MAIN PAGE =========
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    // build hex color string from current RGB
    char colorStr[8];
    sprintf(colorStr, "#%02X%02X%02X", textR, textG, textB);

    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>LED Matrix Control Panel</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      min-height: 100vh;
      padding: 20px;
    }
    .container { max-width: 800px; margin: 0 auto; }
    h1 {
      text-align: center;
      color: white;
      margin-bottom: 30px;
      text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
      font-size: 2.5em;
    }
    .card {
      background: rgba(255,255,255,0.95);
      border-radius: 20px;
      padding: 30px;
      margin-bottom: 20px;
      box-shadow: 0 15px 35px rgba(0,0,0,0.1);
      backdrop-filter: blur(10px);
    }
    .form-group { margin-bottom: 25px; }
    label {
      display: block;
      margin-bottom: 8px;
      font-weight: 600;
      color: #333;
      font-size: 1.1em;
    }
    input[type="text"] {
      width: 100%;
      padding: 15px;
      border: 2px solid #e1e5e9;
      border-radius: 12px;
      font-size: 16px;
      transition: all 0.3s ease;
    }
    input[type="text"]:focus {
      outline: none;
      border-color: #667eea;
      box-shadow: 0 0 0 3px rgba(102,126,234,0.1);
    }
    input[type="range"] {
      width: 100%;
      height: 8px;
      border-radius: 5px;
      background: #e1e5e9;
      outline: none;
      -webkit-appearance: none;
    }
    input[type="range"]::-webkit-slider-thumb {
      -webkit-appearance: none;
      height: 25px;
      width: 25px;
      border-radius: 50%;
      background: #667eea;
      cursor: pointer;
      box-shadow: 0 2px 6px rgba(0,0,0,0.2);
    }
    .range-group { display: flex; align-items: center; gap: 15px; }
    .range-value {
      background: #667eea;
      color: white;
      padding: 8px 15px;
      border-radius: 25px;
      font-weight: bold;
      min-width: 60px;
      text-align: center;
    }
    button {
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      color: white;
      border: none;
      padding: 15px 40px;
      border-radius: 12px;
      font-size: 18px;
      font-weight: 600;
      cursor: pointer;
      transition: all 0.3s ease;
      width: 100%;
    }
    button:hover {
      transform: translateY(-2px);
      box-shadow: 0 10px 25px rgba(102,126,234,0.4);
    }
    .info {
      background: rgba(102,126,234,0.1);
      padding: 20px;
      border-radius: 12px;
      text-align: center;
      margin-top: 20px;
    }
    .ip-display {
      background: rgba(0,255,0,0.2);
      color: #28a745;
      font-size: 1.3em;
      font-weight: bold;
      padding: 15px;
      border-radius: 12px;
      border: 2px solid rgba(40,167,69,0.3);
    }
    select, input[type="color"] {
      width: 100%;
      padding: 12px;
      border-radius: 12px;
      border: 2px solid #e1e5e9;
      font-size: 16px;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>LED Matrix Control</h1>

    <div class="card">
      <form action="/update" method="POST">
        <div class="form-group">
          <label for="row1">Row 1 Text:</label>
          <input type="text" id="row1" name="row1" maxlength="50" value=")rawliteral";

    html += row1Text;
    html += R"rawliteral(">
        </div>

        <div class="form-group">
          <label for="row2">Row 2 Text:</label>
          <input type="text" id="row2" name="row2" maxlength="50" value=")rawliteral";

    html += row2Text;
    html += R"rawliteral(">
        </div>

        <div class="form-group">
          <label>Brightness (0-255):</label>
          <div class="range-group">
            <input type="range" name="brightness" min="10" max="255" value=")rawliteral";

    html += String(brightness);
    html += R"rawliteral(">
            <span class="range-value">)rawliteral";

    html += String(brightness);
    html += R"rawliteral(</span>
          </div>
        </div>

        <div class="form-group">
          <label>Scroll Speed (10-200 ms):</label>
          <div class="range-group">
            <input type="range" name="speed" min="10" max="200" value=")rawliteral";

    html += String(scrollDelay);
    html += R"rawliteral(">
            <span class="range-value">)rawliteral";

    html += String(scrollDelay);
    html += R"rawliteral(ms</span>
          </div>
        </div>

        <div class="form-group">
          <label for="color">Text Color:</label>
          <input type="color" id="color" name="color" value=")rawliteral";

    html += String(colorStr);
    html += R"rawliteral(">
        </div>

        <div class="form-group">
          <label for="cloud">Cloud Mode:</label>
          <select id="cloud" name="cloud">
)rawliteral";

    html += String("<option value=\"local\"") +
            (cloudMode == "local" ? " selected" : "") + ">Local only</option>";
    html += String("<option value=\"blynk\"") +
            (cloudMode == "blynk" ? " selected" : "") + ">Blynk</option>";
    html += String("<option value=\"mqtt\"") +
            (cloudMode == "mqtt" ? " selected" : "") + ">MQTT</option>";

    html += R"rawliteral(
          </select>
        </div>

        <button type="submit">Update Display</button>
      </form>
    </div>

    <div class="info">
      <div class="ip-display">
        Current IP: )rawliteral";

    html += WiFi.localIP().toString();
    html += R"rawliteral(
      </div>
      <p style="color:#666; margin-top:10px; font-size:0.9em;">
        Refresh page to see live updates • Works on mobile too!
      </p>
    </div>
  </div>
</body>
</html>
)rawliteral";

    request->send(200, "text/html", html);
  });

  // ========= FORM HANDLER =========
  server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("row1", true)) {
      row1Text = request->getParam("row1", true)->value() + "   ";
    }
    if (request->hasParam("row2", true)) {
      row2Text = request->getParam("row2", true)->value() + "   ";
    }
    if (request->hasParam("brightness", true)) {
      brightness = request->getParam("brightness", true)->value().toInt();
      matrix.setBrightness8(brightness);
    }
    if (request->hasParam("speed", true)) {
      scrollDelay = request->getParam("speed", true)->value().toInt();
    }
    if (request->hasParam("cloud", true)) {
      cloudMode = request->getParam("cloud", true)->value();
    }
    // color "#RRGGBB"
    if (request->hasParam("color", true)) {
      String c = request->getParam("color", true)->value(); // e.g. "#FF00AA"
      if (c.length() == 7 && c[0] == '#') {
        long val = strtol(c.substring(1).c_str(), NULL, 16);
        textR = (val >> 16) & 0xFF;
        textG = (val >> 8)  & 0xFF;
        textB = val & 0xFF;
      }
    }

    textX1 = PANEL_WIDTH;
    textX2 = PANEL_WIDTH;

    request->redirect("/");
  });

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  int16_t textMin1 = -(row1Text.length() * 8);
  int16_t textMin2 = -(row2Text.length() * 8);

  matrix.fillScreen(0);

  uint16_t textColor = matrix.color565(textR, textG, textB);

  matrix.setTextColor(textColor);
  matrix.setCursor(textX1, 0);
  matrix.print(row1Text.c_str());
  if (--textX1 < textMin1) textX1 = PANEL_WIDTH;

  matrix.setCursor(textX2, 8);
  matrix.print(row2Text.c_str());
  if (--textX2 < textMin2) textX2 = PANEL_WIDTH;

  delay(scrollDelay);
}
