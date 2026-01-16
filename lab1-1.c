#include <math.h>
#include <limits.h>

long nums[512];

//1a
long twice_square(x) {
    return 2L * x * x; //basic c arithmetic
}
//1b
void fill_arr(void) {
    for (int i = 0; i <= 511; i++) { 
        //loop for the length of the array, calling the previous
        //function each time 
        nums[i] = twice_square(i);
    }
}