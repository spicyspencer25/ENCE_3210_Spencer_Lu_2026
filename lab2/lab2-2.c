const int RED_LED_PIN = 5;
const int GREEN_LED_PIN = 4;
const int BLUE_LED_PIN = 6;
const int BUTTON_PIN = 2;

//setup volatile variables that can be changed later
//define boolean grunnning to determine if sequence should be running
//define gLastIsrMs to store time of last interrupt
volatile bool grunning = false;
volatile unsigned long gLastIsrMs = 0;

//ISR-only flag (set by interrupt, handled in main code)
volatile bool gToggleRequested = false;

//helper function (same)
void setLEDs(bool r, bool g, bool b) {
    digitalWrite(RED_LED_PIN, r);
    digitalWrite(GREEN_LED_PIN, g);
    digitalWrite(BLUE_LED_PIN, b);
  }


//ISR function (DO NOT toggle grunning here)
void buttonISR() {
    gToggleRequested = true;
  }


//replacement for delayAndWatchButton:
//waits for specified time while checking
//grunning state, stopping immediately if grunning becomes false
bool delayWhileRunning(unsigned long ms) {
    unsigned long start = millis();
    while (millis() - start < ms) {

      //handle button press request here (with debounce)
      if (gToggleRequested) {
        noInterrupts();
        gToggleRequested = false;
        interrupts();

        unsigned long now = millis();
        if (now - gLastIsrMs < 200) continue;  // debounce gate
        gLastIsrMs = now;

        grunning = !grunning;
        return false;
      }

      if (!grunning) return false;
    }
    return true;
  }

  //helper function to define pattern
void runColorSequence() {
    setLEDs(HIGH, LOW, LOW); if (!delayWhileRunning(500)) return; //R
    setLEDs(HIGH, HIGH, LOW); if (!delayWhileRunning(500)) return; //RG
    setLEDs(HIGH, HIGH, HIGH); if (!delayWhileRunning(500)) return; //RGB
    setLEDs(LOW, HIGH, HIGH); if (!delayWhileRunning(500)) return; //GB
    setLEDs(LOW, LOW, HIGH); if (!delayWhileRunning(500)) return; //B
    setLEDs(HIGH, LOW, HIGH); if (!delayWhileRunning(500)) return; //RB
}

void setup() {
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  setLEDs(LOW, LOW, LOW);

  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);
}

void loop() {
  // handle button press event once (for both start and stop)
  if (gToggleRequested) {
    noInterrupts();
    gToggleRequested = false;
    interrupts();

    unsigned long now = millis();
    if (now - gLastIsrMs >= 200) {   // debounce gate
      gLastIsrMs = now;
      grunning = !grunning;          // toggle state
    }
  }

  // state behavior
  if (!grunning) {
    setLEDs(LOW, LOW, LOW);
    return;
  }

  runColorSequence();
}

