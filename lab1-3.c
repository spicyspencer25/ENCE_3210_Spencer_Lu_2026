#include <limits.h>



int gArr[50];

int find_second_max(void) { 
    //define two var that will be used to find max 
    int max1 = INT_MIN;
    int max2 = INT_MIN;

    for (int i = 0; i < 50; i++) {
        int v = gArr[i];

        //save current iterated element as v, then make comparisons. 
        //if v is larger than max1, max2 will become the current max1, and
        //max1 will become v, ensuring that max1 is always the larger value
        if (v > max1) {
           max2 = max1; 
           max1 = v;
        }
        //if v is smaller but greater than the current max2, make max2 equal to current v
        else if (v != max1 && v > max2) { 
            max2 = v;
        }
    }
    return max2;
}


void save_results(void2) {
    int second = find_second_max;
    *(gArr + 50) = second;
}


void setup() {
    Serial.begin(9600);
    save_results();
    Serial.print("Second max = ");
    Serial.println(*(gArr + 50));
}

void loop() {}