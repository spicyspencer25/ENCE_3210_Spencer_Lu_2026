#include <limits.h>


const int RED_LED_PIN   = 5;
const int GREEN_LED_PIN = 4;


//function that finds pattern by iterating through the whole list up until 3 slots before
//the end so the pattern finding-mechanism doesn't break the program by going out of bounds
bool findPattern(const int subject[], int subjectLen, const int pattern[3], int *count) {
    *count = 0;
    for (int i = 0; i <= subjectLen-3; i++) {
        if (subject[i] == pattern[0] && 
        subject[i+1] == pattern[1] &&
        subject[i+2] == pattern[2]) {
            (*count)++; //increases count if 3 elements match 3 elements
        }
    }
    return (*count > 0); //returns count, which will be True if the pattern exists
}



void setup() {
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, LOW);
  
    int subject[] = {1,2,4,7,1,2,4,9};
    int pattern[3] = {1,2,3};
    int count = 0;
  
    int subjectLen = sizeof(subject) / sizeof(subject[0]);
    bool found = findPattern(subject, subjectLen, pattern, &count);
  
    if (found) digitalWrite(GREEN_LED_PIN, HIGH);
    else digitalWrite(RED_LED_PIN, HIGH);
  }
  
void loop() {}


