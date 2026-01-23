#include <math.h>
#include <limits.h>

const int BUTTON1_PIN = 2; 

//initialize arrays of size 10, with one of them filling up here
volatile int x[10] = {3, 7, 12, 12, 9, 5, 2, 2, 4, 8 };
volatile int y[10]; 

//flag to indicate completion
volatile bool gFilterDone = false;

//timing variable to help debounce
volatile unsigned long gLastIsrMs = 0;


void button1ISR() {
    unsigned long now = millis();
    if (now - gLastIsrMs < 150) return;
    gLastIsrMs = now;
  
    //boundary condition: x[-1] = 0 for n=0
    y[0] = 2 * x[0] - 0;
  
    //computer using formulla
    for (int n = 1; n < 10; n++) {
      y[n] = 2 * x[n] - x[n - 1];
    }
  
    //indicate completion
    gFilterDone = true; 
  }

void setup() {
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON1_PIN), button1ISR, FALLING);
  
    //serial output 
    Serial.begin(9600);
}

void loop() {

  if (gFilterDone) {
    noInterrupts();
    gFilterDone = false;
    interrupts();

    Serial.println("y[] updated:");
    for (int i = 0; i < 10; i++) {
      Serial.print("y[");
      Serial.print(i);
      Serial.print("] = ");
      Serial.println((int)y[i]);
    }
  }
}









