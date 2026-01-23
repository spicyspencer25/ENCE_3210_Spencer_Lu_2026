const int RED_LED_PIN   = 5;
const int GREEN_LED_PIN = 4;
const int BLUE_LED_PIN  = 6;

const int OP_BUTTON_PIN   = 2;  //select operation
const int EXEC_BUTTON_PIN = 3;  //execute operation


int gArrayA[10] = {10, 20, 30, 40, 50,  60,  70,  80,  90, 100};
int gArrayB[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int gArrayC[10]; //result array



//op state, defeined w volatile variables
volatile int gOpCount = 0;
volatile bool gOpChanged = false;
volatile bool gExecRequested = false;


//flags set by ISRs to notify loop of a state change
volatile unsigned long gLastOpIsrMs = 0;
volatile unsigned long gLastExecIsrMs = 0;

//led control
void setLeds(bool redOn, bool greenOn, bool blueOn) {
    digitalWrite(RED_LED_PIN, redOn);
    digitalWrite(GREEN_LED_PIN, greenOn);
    digitalWrite(BLUE_LED_PIN, blueOn);
}

void showSelectedOperationLed() {
    if (gOpCount == 1) setLeds(HIGH, LOW, LOW); //add (red)
    else if (gOpCount == 2) setLeds(LOW, HIGH, LOW); //subtract (green)
    else if (gOpCount == 3) setLeds(LOW, LOW, HIGH); //multiply (blue)
    else setLeds(LOW, LOW, LOW);
}

//calculator function logic; 1 = add, 2 = subtract, 3 = multiply
int applyOperation(int x, int y) {
    if (gOpCount == 1) return x + y;
    if (gOpCount == 2) return x - y;
    if (gOpCount == 3) return x * y;
    return 0;
}

void executeOperation(void) {   
    //LEDs off during processing
    setLeds(LOW, LOW, LOW);

    for (int i = 0; i < 10; i++) {
    gArrayC[i] = applyOperation(gArrayA[i], gArrayB[i]);
  }
    Serial.println("Result array:");
    for (int i = 0; i < 10; i++) {
        Serial.print("c[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(gArrayC[i]);
  }

  //All LEDs on when finished
    setLeds(HIGH, HIGH, HIGH);
}

//isr function for select operation. debounces press,
//incrmenets flag and updates G0pChanged so loop may detect change too
void opButtonISR() {
    unsigned long now = millis();
    if (now - gLastOpIsrMs < 150) return; //debounce
    gLastOpIsrMs = now;
  
    gOpCount++;
    if (gOpCount > 3) gOpCount = 1;
    gOpChanged = true;
  }
  
  void execButtonISR() {
    unsigned long now = millis();
    if (now - gLastExecIsrMs < 150) return; // debounce
    gLastExecIsrMs = now;
  
    gExecRequested = true;
  }
  
//isr function for execute operation. debounces press,
//updates GExecRequested so loop may detect change too
void setup() {
    Serial.begin(9600);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);

    pinMode(OP_BUTTON_PIN, INPUT_PULLUP);
    pinMode(EXEC_BUTTON_PIN, INPUT_PULLUP);


    setLeds(LOW, LOW, LOW);


    attachInterrupt(digitalPinToInterrupt(OP_BUTTON_PIN), opButtonISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(EXEC_BUTTON_PIN), execButtonISR, FALLING);
}

void loop() {
    bool opChangedCopy;
    bool execCopy;

    //avoid race conditions 
    //copies bool values to local to be used in condtionals later
    noInterrupts();
    opChangedCopy = gOpChanged;
    execCopy = gExecRequested;
    gOpChanged = false;
    gExecRequested = false;
    interrupts();


    //operation go!
    if (opChangedCopy) {
        showSelectedOperationLed();
    }

    //execute go!
    if (execCopy) {
        executeOperation();
    }
}


