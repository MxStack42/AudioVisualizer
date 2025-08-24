# Stereo Audio Spectrum Visualizer

A real-time stereo audio spectrum analyzer that displays frequency content on a 16×16 LED matrix using dual MSGEQ7 ICs and Arduino.

![Demo](images/demo.gif)

## Features

- **Real-time frequency analysis** across 7 bands (63Hz - 16kHz)
- **True stereo processing** with visual channel separation
- **256 individually addressable RGB LEDs** (WS2812B)
- **Hardware-based spectrum analysis** using MSGEQ7 ICs
- **Dynamic color mapping** from blue (low) through green to red (high amplitude)
- **Professional power management** with dedicated 90W PSU

## System Overview

The visualizer processes stereo audio through two MSGEQ7 graphic equalizer ICs, each handling one channel. An Arduino Uno reads the frequency data and drives a 16×16 LED matrix, creating a dynamic visualization that responds in real-time to your music.

![System Overview](images/IMG_02.jpg)

## Hardware Requirements

### Core Components
- Arduino Uno R3
- 2× MSGEQ7 Graphic Equalizer IC
- 16×16 WS2812B LED Matrix (256 pixels)
- Mean Well 90W Power Supply (5V/18A)

### Support Components
- 2× 3.5mm Stereo Audio Jack
- 2× 200kΩ Resistor
- 2× 22kΩ Resistor  
- 2× 33pF Ceramic Capacitor
- 2× 10nF Ceramic Capacitor
- 4× 0.1µF Ceramic Capacitor
- Breadboard and jumper wires

### Tools Required
- Soldering iron
- Multimeter
- Wire strippers
- Basic hand tools

## Quick Start Guide

### 1. Hardware Setup

1. **Build the circuit** according to the schematic in `/schematics`
2. **Connect the MSGEQ7 ICs:**
   - Left channel: Reset→Pin3, Strobe→Pin2, Output→A0
   - Right channel: Reset→Pin5, Strobe→Pin4, Output→A1
3. **Wire the LED matrix:**
   - Data→Pin6
   - Power→External 5V PSU
   - GND→Common ground
4. **Audio input:** Connect 3.5mm jacks with 22kΩ + 10nF high-pass filters

### 2. Software Installation

1. Install the [Arduino IDE](https://www.arduino.cc/en/software)
2. Install the Adafruit NeoPixel library:
   ```
   Tools → Manage Libraries → Search "Adafruit NeoPixel" → Install
   ```
3. Open `src/sasv.ino`
4. Select your Arduino board and port
5. Upload the code

### 3. First Run

1. **Power sequence:**
   - Connect Arduino via USB
   - Power on the external PSU for LEDs
   - Connect audio source
2. **Adjust parameters if needed:**
   - `noiseFilter`: Increase if seeing noise (default: 90)
   - `brightness`: LED brightness (default: 20, max: 255)
   - `maxMAPlimit`: Maximum column height (default: 16)

## Pin Configuration

| Component | Arduino Pin | Function |
|-----------|------------|----------|
| Left MSGEQ7 Strobe | D2 | Frequency band selection |
| Left MSGEQ7 Reset | D3 | Multiplexer reset |
| Right MSGEQ7 Strobe | D4 | Frequency band selection |
| Right MSGEQ7 Reset | D5 | Multiplexer reset |
| Left Audio Output | A0 | Analog input |
| Right Audio Output | A1 | Analog input |
| LED Matrix Data | D6 | WS2812B data line |

## Display Layout

```
Left Channel                                      Right Channel
[0]  [1]   [2]   [3]  [4]    [5]     [6]  [ ] [ ] [9]  [10]  [11]  [12] [13]   [14]    [15]
63Hz 160Hz 400Hz 1kHz 2.5kHz 6.25kHz 16kHz        63Hz 160Hz 400Hz 1kHz 2.5kHz 6.25kHz 16kHz
```

Columns 7-8 remain dark for visual stereo separation.

## Safety Notice

⚠️ **WARNING:** This project uses mains voltage (230V AC). Electrical work should only be performed by qualified individuals following proper safety procedures.

- The Mean Well PSU requires **adequate ventilation** - never cover during operation
- Maintain **physical separation** between high-voltage and low-voltage sections
- Use proper **strain relief** for all power cables
- Follow local electrical codes and regulations

## Performance Specifications

- **Frequency Bands:** 63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25kHz, 16kHz
- **Refresh Rate:** ~100-120Hz (limited by LED update time)
- **Power Consumption:** 
  - Maximum: 76.8W (all LEDs white)
  - Typical: 10-20W (music visualization)
- **ADC Resolution:** 10-bit (0-1023)
- **Display Resolution:** 7 bands × 16 levels per channel

## Known Limitations

1. **Dynamic Range:** Top 2 pixels rarely illuminate due to MSGEQ7 output characteristics
2. **EMI Sensitivity:** May show artifacts with poor cable routing near power supply

## Documentation

For detailed technical documentation including circuit analysis, safety procedures, and design decisions, see [`documentation/Technical_Documentation.pdf`](documentation/Technical_Documentation.pdf)

## License

This project is released under the MIT License - see LICENSE file for details.

## Author

**Max Strack** - Kaiserslautern, Germany

---

*For questions, improvements, or collaboration opportunities, please open an issue or submit a pull request.*