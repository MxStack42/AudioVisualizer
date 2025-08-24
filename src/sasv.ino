#include <Adafruit_NeoPixel.h>

// MSGEQ7 frequency band data (7 bands per channel)
uint16_t leftMSGEQ[7];
uint16_t rightMSGEQ[7];

// Hardware pin assignments
const uint8_t strobePins[2] = {2, 4};   // MSGEQ7 strobe control pins
const uint8_t resetPins[2] = {3, 5};    // MSGEQ7 reset control pins
const uint8_t analogPins[2] = {A0, A1}; // ADC inputs from MSGEQ7 outputs
const uint8_t outputPin = 6;            // WS2812B data pin

// LED matrix and visualization parameters
const uint16_t numLEDs = 256;           // 16x16 matrix
const uint8_t minMAPlimit = 0;          // Minimum amplitude mapping
const uint8_t maxMAPlimit = 16;         // Maximum amplitude mapping (full column height)
const int8_t smoothingOffset = 3;       // Color transition smoothing
const uint16_t noiseFilter = 90;        // ADC noise threshold (out of 1023)
const uint8_t brightness = 20;          // LED brightness level
const uint8_t readDataDelay = 3;        // Delay between frequency band readings

Adafruit_NeoPixel matrix = Adafruit_NeoPixel(numLEDs, outputPin, NEO_GRB + NEO_KHZ800);

void setup() {
    // Configure MSGEQ7 control pins
    for (uint8_t i = 0; i < 2; i++) {
        pinMode(strobePins[i], OUTPUT);
        pinMode(resetPins[i], OUTPUT);
        pinMode(analogPins[i], INPUT);
    }

    // Initialize MSGEQ7 control signals (idle state)
    for (uint8_t i = 0; i < 2; i++) {
        digitalWrite(resetPins[i], LOW);
        digitalWrite(strobePins[i], HIGH);
    }

    matrix.begin();
    matrix.setBrightness(brightness);
    matrix.show();
}

void loop() {
    readData();
    setLEDs();
    matrix.show();
}

void readData() {
    // Reset both MSGEQ7 multiplexers to start from 63Hz band
    for (uint8_t i = 0; i < 2; i++) {
        digitalWrite(resetPins[i], HIGH);
    }
    for (uint8_t i = 0; i < 2; i++) {
        digitalWrite(resetPins[i], LOW);
    }

    // Read all 7 frequency bands sequentially
    for (uint8_t band = 0; band < 7; band++) {
        // Enable MSGEQ7 output for current frequency band
        for (uint8_t i = 0; i < 2; i++) {
            digitalWrite(strobePins[i], LOW);
        }
        
        delayMicroseconds(36); // Wait for MSGEQ7 output to settle (datasheet requirement)

        // Read ADC values from both channels
        leftMSGEQ[band] = analogRead(analogPins[0]);
        rightMSGEQ[band] = analogRead(analogPins[1]);

        // Advance multiplexer to next frequency band
        for (uint8_t i = 0; i < 2; i++) {
            digitalWrite(strobePins[i], HIGH);
        }

        // Apply noise filtering and map to LED column height
        leftMSGEQ[band] = constrain(leftMSGEQ[band], noiseFilter, 1023);
        rightMSGEQ[band] = constrain(rightMSGEQ[band], noiseFilter, 1023);
        leftMSGEQ[band] = map(leftMSGEQ[band], noiseFilter, 1023, minMAPlimit, maxMAPlimit);
        rightMSGEQ[band] = map(rightMSGEQ[band], noiseFilter, 1023, minMAPlimit, maxMAPlimit);

        delay(readDataDelay);
    }
}

void setLEDs() {
    matrix.clear();

    // Left channel visualization (columns 0-6)
    for (uint8_t column = 0; column < 7; column++) {
        for (uint8_t row = 0; row < leftMSGEQ[column]; row++) {
            uint16_t pixelIndex = calcPixelIndex(column, row);
            if (pixelIndex < numLEDs) {
                matrix.setPixelColor(pixelIndex, colorMap(row, 0, maxMAPlimit));
            }
        }
    }

    // Right channel visualization (columns 9-15)
    // Columns 7-8 remain dark for visual stereo separation
    for (uint8_t column = 0; column < 7; column++) {
        for (uint8_t row = 0; row < rightMSGEQ[column]; row++) {
            uint16_t pixelIndex = calcPixelIndex(column + 9, row);
            if (pixelIndex < numLEDs) {
                matrix.setPixelColor(pixelIndex, colorMap(row, 0, maxMAPlimit));
            }
        }
    }
}

// Calculate linear pixel index for zigzag matrix wiring pattern
uint16_t calcPixelIndex(uint8_t column, uint8_t row) {
    if (column % 2 == 0) {
        return (column * 16) + (15 - row); // Even columns: bottom-to-top addressing
    } else {
        return (column * 16) + row;        // Odd columns: top-to-bottom addressing
    }
}

// Map amplitude to color gradient: blue (low) -> green (mid) -> red (high)
uint32_t colorMap(int value, int minValue, int maxValue) {
    int mappedValue = map(value, minValue, maxValue - smoothingOffset, 0, 255);
    mappedValue = constrain(mappedValue, 0, 255);

    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;

    if (mappedValue <= 127) {
        // Lower range: blue to green transition
        blue = 255 - (mappedValue * 2);
        green = mappedValue * 2;
    } else {
        // Upper range: green to red transition
        green = 255 - ((mappedValue - 128) * 2);
        red = (mappedValue - 128) * 2;
    }

    return matrix.Color(red, green, blue);
}