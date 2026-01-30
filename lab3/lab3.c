
 #include <util/atomic.h>  //For ATOMIC_BLOCK

 const int RED_LED_PIN   = 5;
 const int GREEN_LED_PIN = 4;
 
 //button pins on Kitchen Timer 1.0 board
 const int BUTTON1_PIN = 2;
 const int BUTTON2_PIN = 3;
 
//these here below are the macros that help define/setup the timers
//that are necessary for the LED behavior. timer1 is 15623 due to the
//calculation (16,000,000 / (1024 * 1)) - 1 = 15624, which is 
//CPU freq / prescaler* desired freq - 1
 #define TIMER1_OCR_1HZ 15624     //For 1 Hz with prescaler 1024
 //this sets timer2 to run at 100 Hz base freq, using a similar fomrmula of
 //(16,000,000 / (1024 * 100)) - 1 = 155
 #define TIMER2_OCR_100HZ 155     //For ~100 Hz with prescaler 1024
 //since we want 10 Hz for task 2, we define a divider here
 #define TIMER2_DIVIDER 10        //Divide 100 Hz down to 10 Hz
 
 //we use volatile here since it's modified in ISR then read in main code
 //we want to ensure its read from memory to prevent errors
 volatile unsigned long gSeconds = 0;
 
 //Toggles Green LED and increments counter every sec
 static inline void taskOneISR(void) {
     digitalWrite(GREEN_LED_PIN, !digitalRead(GREEN_LED_PIN));
     gSeconds++;
 }
 
 //Sets up timer one by configuring individual registers
 static void timerOneSetup(void) {
     noInterrupts();
     //clear the hardware registers to known state
     TCCR1A = 0;
     TCCR1B = 0;
     TCNT1 = 0; //reset counter val
     //set compare register for 1 Hz
     OCR1A = TIMER1_OCR_1HZ;


     TCCR1B |= (1 << WGM12); //configure 

     TCCR1B |= (1 << CS12) | (1 << CS10);  // Prescaler 1024

     TIMSK1 |= (1 << OCIE1A);
     interrupts();
 }
 
 //activates ISR @ component TIMER1_COMPA_vect
 ISR(TIMER1_COMPA_vect) {
     taskOneISR();
 }
 
 //Controls red LED, toggling if button is pressed
 static inline void taskTwoISR(void) {
  
     bool button1Pressed = (digitalRead(BUTTON1_PIN) == HIGH);  
     bool button2Pressed = (digitalRead(BUTTON2_PIN) == HIGH);  
     
     if (button1Pressed || button2Pressed) {
         digitalWrite(RED_LED_PIN, HIGH);   //Turn on Red LED
     }
     else {
         digitalWrite(RED_LED_PIN, LOW);    //Turn off Red LED
     }
 }
 
 //Sets up timer two by configuring individual registers
 void timerTwoSetup(void) {
     noInterrupts();
     
     // Clear Timer2 control registers
     TCCR2A = 0;
     TCCR2B = 0;
   
     //Reset Timer2 counter
     TCNT2 = 0;
   
     //Set compare value for ~100 Hz
     OCR2A = TIMER2_OCR_100HZ;
   
     //Configure CTC mode
     TCCR2A |= (1 << WGM21);
   
     //Set prescaler to 1024
     TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
   
     //Enable Timer2 compare interrupt
     TIMSK2 |= (1 << OCIE2A);
   
     interrupts();
 }
//activates ISR @ component TIMER2_COMPA_vect 
 ISR(TIMER2_COMPA_vect) {
     static unsigned char div = 0;  
     //variable counitng Timer2 interrupts
     //to divide 100 Hz down to 10 Hz
     //static var persists past
     //individual calls to ensure it's being checked
     div++;
     if (div >= TIMER2_DIVIDER) {
         div = 0;
         taskTwoISR();
     }
 }
 
 void setup(void) {
     pinMode(GREEN_LED_PIN, OUTPUT);
     pinMode(RED_LED_PIN, OUTPUT);
   
     //Configure BOTH button pins with internal pull-up
     pinMode(BUTTON1_PIN, INPUT_PULLUP);
     pinMode(BUTTON2_PIN, INPUT_PULLUP);
     
     // Initialize LEDs to OFF
     digitalWrite(GREEN_LED_PIN, LOW);
     digitalWrite(RED_LED_PIN, LOW);
   
     // nitialize serial communication
     Serial.begin(115200);
     Serial.println("=== Kitchen Timer Multi-Task Program ===");
     Serial.println("Green LED: Blinks at 1 Hz");
     Serial.println("Red LED: ON when EITHER button is pressed");
     Serial.println("========================================");
   
     //Configure hardware timers
     timerOneSetup();
     timerTwoSetup();
 }
 
 void loop(void) {
     //Local static variable to track last print time
     static unsigned long lastPrint = 0;
     
     //Local copy of shared time counter
     unsigned long now;
     
     //Atomically read shared variable, same as using 
     //nointerrupt then interrupt but cleaner
     ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
         now = gSeconds;
     }
     
     //Print message every 3 seconds
     if ((now - lastPrint) >= 3) {
         lastPrint = now;
         Serial.print("Foreground task: ");
         Serial.print(now);
         Serial.println(" seconds elapsed");
     }
     
     // No delay() used — timers control timing
 }




