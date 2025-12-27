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

## 🔌 Pin Configuration

**ESP32 + P10 RGB LED Matrix (HUB75) - 1/16 Scan**

| P10 Pin | ESP32 GPIO | Function          | Description |
|---------|------------|-------------------|-------------|
| **R1**  | `GPIO21`   | **Red Data 1**    | Red color - Top half (rows 0-7) |
| **G1**  | `GPIO22`   | **Green Data 1**  | Green color - Top half (rows 0-7) |
| **B1**  | `GPIO5`    | **Blue Data 1**   | Blue color - Top half (rows 0-7) |
| **R2**  | `GPIO4`    | **Red Data 2**    | Red color - Bottom half (rows 8-15) |
| **G2**  | `GPIO16`   | **Green Data 2**  | Green color - Bottom half (rows 8-15) |
| **B2**  | `GPIO17`   | **Blue Data 2**   | Blue color - Bottom half (rows 8-15) |
|---------|------------|-------------------|-------------|
| **A**   | `GPIO23`   | **Row Address A** | Row select bit 0 (2⁰) |
| **B**   | `GPIO19`   | **Row Address B** | Row select bit 1 (2¹) |
| **C**   | `GPIO18`   | **Row Address C** | Row select bit 2 (2²) |
| **D**   | `GPIO25`   | **Row Address D** | Row select bit 3 (2³) |
| **E**   | `-1`       | **Row Address E** | **Not used** (32x16 panel) |
|---------|------------|-------------------|-------------|
| **LAT** | `GPIO27`   | **Latch**         | Data latch (rising edge) |
| **OE**  | `GPIO14`   | **Output Enable** | LED blanking (LOW=ON) |
| **CLK** | `GPIO12`   | **Clock**         | Shift clock (rising edge) |
| **GND** | `GND`      | **Ground**        | Common ground |
| **+5V** | **External**| **Power**       | 5V 5A+ SMPS required |


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



