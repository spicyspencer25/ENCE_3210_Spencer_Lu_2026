#include <math.h>
#include <limits.h>

const int RED_LED_PIN   = 5;
const int GREEN_LED_PIN = 4;
const int BLUE_LED_PIN  = 6;


const int BUTTON1_PIN = 2;  // D2 = INT0
const int BUTTON2_PIN = 3;  // D3 = INT1

//initialize volatile variables so they can be changed later
//initialize counts for each button, which will be used to determine
//which was pressed more through basic substraction logic
volatile long gButton1Count = 0;
volatile long gButton2Count = 0;
volatile long gButtonDiff = 0;

//debounce timestamps for each button ISR 
volatile unsigned long gLastIsrMs1 = 0;
volatile unsigned long gLastIsrMs2 = 0;

//helper function
void setLeds(bool redOn, bool greenOn, bool blueOn) {
    digitalWrite(RED_LED_PIN,   redOn   ? HIGH : LOW);
    digitalWrite(GREEN_LED_PIN, greenOn ? HIGH : LOW);
    digitalWrite(BLUE_LED_PIN,  blueOn  ? HIGH : LOW);
}

//ISRS to signify button press
void button1ISR() {
    unsigned long now = millis();
    if (now - gLastIsrMs1 < 150) return;   // ADDED debounce gate
    gLastIsrMs1 = now;
    gButton1Count++;
}
void button2ISR() {
    unsigned long now = millis();
    if (now - gLastIsrMs2 < 150) return;   // ADDED debounce gate
    gLastIsrMs2 = now;
    gButton2Count++;

}



void setup() {
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);

    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);

    setLeds(LOW, LOW, LOW);

    //Attach interrupts, specifying that the buttons are the pins, and that the ISR functions
    //above are what will be called in the case of a FALLING state(pressed) button 
    attachInterrupt(digitalPinToInterrupt(BUTTON1_PIN), button1ISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), button2ISR, FALLING);
}


void loop() {
    long diff;

    //allow CPU to read bytes without interruption/ISRs
    noInterrupts();
    //basic subtraction logic begins here with diff, then comparisons later in this function
    diff = gButton1Count - gButton2Count;
    interrupts();

    if (diff > 0) {
        setLeds(HIGH, LOW, LOW);  //button 1 > button 2
    }
    else if (diff == 0) {
        setLeds(LOW, LOW, LOW);   //equal
    }
    else {
        setLeds(LOW, HIGH, LOW);   //button 2 > button 1
    }
}







