const int RED_LED_PIN   = 5;
const int GREEN_LED_PIN = 4;
const int BLUE_LED_PIN  = 6;

const int OP_BUTTON_PIN   = 2;  //select operation
const int EXEC_BUTTON_PIN = 3;  //execute operation


int gArrayA[10] = {10, 20, 30, 40, 50,  60,  70,  80,  90, 100};
int gArrayB[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int gArrayC[10]; //result array



int gOpCount = 0;

bool gLastOpButtonState = HIGH;
bool gLastExecButtonState = HIGH;

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


void setup() {
    Serial.begin(9600);
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);

    pinMode(OP_BUTTON_PIN, INPUT_PULLUP);
    pinMode(EXEC_BUTTON_PIN, INPUT_PULLUP);


    setLeds(LOW, LOW, LOW);


    gLastOpButtonState   = digitalRead(OP_BUTTON_PIN);
    gLastExecButtonState = digitalRead(EXEC_BUTTON_PIN);
}

void loop() {
    bool opButtonState = digitalRead(OP_BUTTON_PIN);
    bool execButtonState = digitalRead(EXEC_BUTTON_PIN);

    //op button
    if (gLastOpButtonState == HIGH && opButtonState == LOW) {
    gOpCount++;
        if (gOpCount > 3) gOpCount = 1;
        showSelectedOperationLed();
        delay(200); //debounce
    }
    gLastOpButtonState = opButtonState;

    //execute button
    if (gLastExecButtonState == HIGH && execButtonState == LOW) {
    executeOperation();
    delay(200); //debounce
  }
    gLastExecButtonState = execButtonState;
}



