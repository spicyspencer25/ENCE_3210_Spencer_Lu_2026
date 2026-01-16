const int RED_LED_PIN   = 5;
const int GREEN_LED_PIN = 4;
const int BLUE_LED_PIN  = 6;
const int BUTTON_PIN    = 2;

//setup global bools to help w button stuff
bool grunning = false;
bool glastButtonState = HIGH;

//helper function
void setLEDs(bool r, bool g, bool b) {
  digitalWrite(RED_LED_PIN, r);
  digitalWrite(GREEN_LED_PIN, g);
  digitalWrite(BLUE_LED_PIN, b);
}

//function to wait and help watch for button press
bool delayAndWatchButton(unsigned long ms) {
    unsigned long start = millis();
    while (millis() - start < ms) {
      bool buttonState = digitalRead(BUTTON_PIN); //read button contiuously
  
      if (glastButtonState == HIGH && buttonState == LOW) {
        grunning = !grunning;  //toggle immediately
        delay(80); //debounce
        glastButtonState = buttonState;
        return false; //tell caller to stop sequence right now
      }
  
      glastButtonState = buttonState;
    }
    return true; //wait normally
  }


//helper function to define pattern
void runColorSequence() {
    setLEDs(HIGH, LOW, LOW); if (!delayAndWatchButton(500)) return; // R
    setLEDs(HIGH, HIGH, LOW); if (!delayAndWatchButton(500)) return; // RG
    setLEDs(HIGH, HIGH, HIGH); if (!delayAndWatchButton(500)) return; // RGB
    setLEDs(LOW, HIGH, HIGH); if (!delayAndWatchButton(500)) return; // GB
    setLEDs(LOW, LOW, HIGH); if (!delayAndWatchButton(500)) return; // B
    setLEDs(HIGH, LOW, HIGH); if (!delayAndWatchButton(500)) return; // RB
}

void setup() {
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  setLEDs(LOW, LOW, LOW);
}

void loop() {
  // If not running, just idle with LEDs off until a press happens
  if (!grunning) {
    setLEDs(LOW, LOW, LOW);

    bool buttonState = digitalRead(BUTTON_PIN);
    if (glastButtonState == HIGH && buttonState == LOW) {
      grunning = true;
      delay(80);
    }
    glastButtonState = buttonState;
    return;
  }
  // Running: keep playing sequence, but sequence can be interrupted by button press
  runColorSequence();
}