/*
Scope test for Phase B prototype:
- 10-bit R-2R DAC data bus: D4..D13
- Outputs a repeating ramp 0..1023 to the R-2R ladder
- Also outputs a 1 kHz square wave on D3 for quick scope sanity check

How to use:
1) Probe square wave: scope tip on D3, ground on Arduino GND -> should see 1 kHz square wave.
2) Probe DAC output: scope tip on the analog output of your R-2R ladder (or op-amp output),
ground on Arduino GND -> should see a ramp/staircase waveform.
*/

#include <Arduino.h>

const uint8_t SQUARE_PIN = 3; // quick test output
const uint8_t dacPins[10] = {4,5,6,7,8,9,10,11,12,13}; // 10-bit bus

// Write 10-bit value (0..1023) to R-2R ladder
void writeDAC10(uint16_t value)
{
value &= 0x03FF; // keep only 10 bits

// D4 = bit0 (LSB), D13 = bit9 (MSB)
for (int bit = 0; bit < 10; bit++)
{
digitalWrite(dacPins[bit], (value >> bit) & 1);
}
}

void setup()
{
pinMode(SQUARE_PIN, OUTPUT);
digitalWrite(SQUARE_PIN, LOW);

for (int i = 0; i < 10; i++)
{
pinMode(dacPins[i], OUTPUT);
digitalWrite(dacPins[i], LOW);
}
}

void loop()
{
// ----- 1 kHz square wave on D3 (sanity check) -----
// 1 kHz period = 1000 us, half period = 500 us
digitalWrite(SQUARE_PIN, HIGH);
delayMicroseconds(500);
digitalWrite(SQUARE_PIN, LOW);
delayMicroseconds(500);

// ----- DAC ramp 0..1023 -----
// One ramp step per loop call would be too slow because of the square wave delays,
// so we do a short burst of DAC steps each time.
for (uint16_t code = 0; code < 1024; code++)
{
writeDAC10(code);

// Controls ramp speed:
// - smaller = faster (higher output frequency)
// - bigger = slower (easier to see stair-steps)
delayMicroseconds(80);
}
}
