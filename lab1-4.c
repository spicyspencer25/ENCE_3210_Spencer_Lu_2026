#include <limits.h>

#define RED_LED_PIN 5
#define GREEN_LED_PIN 4
#define BLUE_LED_PIN 3

//array to test 
// char gdata[10] = { 'r','r','r','r','b','r','r','g','r','r' };

//create a structure with attributes r, g, and b that are represented via integers
typedef struct {
    int r;
    int g;
    int b;
} RGB;

//function to return an RGB struct after creating it with preset parameters based on the 
//array that is being analyzed 
RGB count_letters(const char *arr, int len) {
    RGB counts = {0, 0, 0};

    for (int i = 0; i < len; i++) {
        if (arr[i] == 'R' || arr[i] == 'r') counts.r++;
        else if (arr[i] == 'G' || arr[i] == 'g') counts.g++;
        else if (arr[i] == 'B' || arr[i] == 'b') counts.b++;
    }
    return counts;
}

//function to help print results
void update_leds(RGB counts) {
    digitalWrite(RED_LED_PIN, counts.r > 0 ? HIGH : LOW);
    digitalWrite(GREEN_LED_PIN, counts.g > 0 ? HIGH : LOW);
    digitalWrite(BLUE_LED_PIN, counts.b > 0 ? HIGH : LOW);
}


void setup() {
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(BLUE_LED_PIN, OUTPUT);
    // RGB result = count_letters(gdata, sizeof(gdata)-1);
    // update_leds(result);
}

void loop() {}

