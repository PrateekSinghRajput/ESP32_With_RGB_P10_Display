# ESP32 with RGB P10 Display 🚀

ESP32 microcontroller interfaced with P10 RGB LED matrix (32x16 pixels, HUB75) for scrolling text, animations, and digital signage. Uses PxMatrix library for high-performance display control.


## ✨ Features
- Full-color RGB display (16-bit 565 format)
- Scrolling text & graphics animations
- Web server for remote text updates
- Adjustable brightness & scan rate
- Double buffering for flicker-free updates
- WiFi AP/Station mode support

## 📋 Hardware Requirements
ESP32 Board
P10 RGB LED panel (16x32, 1/6 scan)
5V 5A external SMPS power supply
HUB75 ribbon cable (16-pin)


## 🔌 Pin Connections
R1_PIN  21
G1_PIN  22
B1_PIN   5
R2_PIN   4
G2_PIN  16
B2_PIN  17
A_PIN   23
B_PIN   19
C_PIN   18
D_PIN   25
E_PIN  -1
LAT_PIN 27
OE_PIN  14
CLK_PIN 12

> **⚠️ Power Warning**: Connect P10 panel directly to 5V 5A external supply. ESP32 5V pin cannot power the display!

## 🛠️ How It Works
1. DATA SHIFTING: RGB data (R1/G1/B1 + R2/G2/B2) shifts in on CLK rising edges
2. ROW SELECT: A/B/C/D pins set binary row address (0-15 for 16 rows)
3. LATCH: LAT rising edge transfers shifted data to selected row
4. DISPLAY: OE LOW enables LEDs for that row (multiplex scanning)
5. REPEAT: Next row address, repeat for all 16 rows (60Hz refresh)
6. 1/8 scan panels: Only use R1/G1/B1 pins (R2/G2/B2 = -1)
7. CLK → Shift RGB data (512 bits per row)
8. LAT ↑ → Latch to row registers  
9. A/B/C/D → Set next row address
10. OE ↓ → Display row (typically 30-70μs)
11. OE ↑ → Blank row, prepare next


## 🎯 Applications
- Digital notice boards
- Shop/Store displays
- Sports scoreboards
- IoT message panels
- Event signage


## 🤝 Contributing
1. Fork the project
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to branch (`git push origin feature/AmazingFeature`)
5. Open Pull Request

## 📄 License
This project is [MIT](LICENSE) licensed - see the [LICENSE](LICENSE) file for details.

---

**⭐ Star this repo if it helped you!**  
*Made with ❤️ for embedded developers by [PrateekSinghRajput](https://github.com/PrateekSinghRajput)*



