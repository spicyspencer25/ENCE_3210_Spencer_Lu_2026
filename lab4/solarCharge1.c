#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//analog pins used to read volt sources
#define SOLAR_PIN       A0
#define BATTERY_PIN     A1


//ADC consts for converting raw data to actual voltages
#define VREF            5.0f
#define ADC_RES         1023.0f

//accoutns for voltage divide scaling, sets to 1.0
//if voltage stays under 5V and no divier is used
#define DIVIDER_SCALE   2.0f   
//threshold from spec b/c - solar must exceed this to power MCU
#define SOLAR_THRESHOLD 2.5f

//volatile globals so ISR can write and main loop can read safely
volatile float solarV = 0.0f;
volatile float battV  = 0.0f;


//helper function to convert ADC readings to actual voltage
//applies divider scale so the reuslt refelcts actual battery voltage
float readVoltage(int pin) {
    int raw = analogRead(pin);
    return (raw / ADC_RES) * VREF * DIVIDER_SCALE;
}

//ISR fires every second and samples both voltages
ISR(TIMER1_COMPA_vect) {
    solarV = readVoltage(SOLAR_PIN);
    battV  = readVoltage(BATTERY_PIN);
}

//helper function that handles all OLED output
void updateDisplay(float solarV, float battV, bool solarPoweringMCU, bool batteryCharging) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    //live voltage readings
    display.setCursor(0, 0);
    display.print("Solar:  "); display.print(solarV, 2); display.println("V");
    display.setCursor(0, 12);
    display.print("Battery:"); display.print(battV, 2); display.println("V");

    display.drawLine(0, 23, 127, 23, SSD1306_WHITE);

    //source currently powering the MCU
    display.setCursor(0, 28);
    display.println("MCU powered by:");
    display.setCursor(0, 38);
    display.setTextSize(2);
    display.println(solarPoweringMCU ? "SOLAR" : "BATTERY");

    //whether or not battery is being charged
    display.setTextSize(1);
    display.setCursor(0, 56);
    display.print(batteryCharging ? ">> Battery charging" : "   Battery idle");

    display.display();
}

void setup() {
    Serial.begin(9600);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 not found");
        while (true);
    }
    display.clearDisplay();
    display.display();

    //configure Timer1 CTC mode, prescaler 1024 = 1Hz on 16MHz Arduino
    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
    OCR1A  = 15625;
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A);
    interrupts();
}

void loop() {
    //// prevents ISR from changing values mid-loop
    float solar = solarV;
    float batt  = battV;

    // Spec (b)/(c): if solar is above 2.5V then power MCU
    //otherwise battery takes over
    bool solarPoweringMCU = (solar >= SOLAR_THRESHOLD);

    //if battery voltage is below solar voltage. connect battery
    //to panel o charge to trigger switching element
    bool batteryCharging = (batt < solarV);

    updateDisplay(solarV, battV, solarPoweringMCU, batteryCharging);

    Serial.print("Solar: ");  Serial.print(solarV);
    Serial.print("V  Batt: "); Serial.print(battV);
    Serial.print("V  MCU: "); Serial.print(solarPoweringMCU ? "SOLAR" : "BATTERY");
    Serial.print("  Charging: "); Serial.println(batteryCharging ? "YES" : "NO");
}



