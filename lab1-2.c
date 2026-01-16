#include <math.h>
#include <limits.h>


float cos_stuff[359];

void find_cos(void) {  
    for (int i = 0; i < 360; i++) {
        const float pi = acosf(-1.0f); //define pi
        float radians = i * (pi/180.0f); //convert degrees to radians
        cos_stuff[i] = cosf(i);
    }
}




