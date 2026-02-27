#include <stdio.h>

//changed bitsize to proper size
#define BITSIZE 14

//created helper function to refactor preious global variables Vref, Vin, thresh, quantisized,
//bitval, and bits so multiple Vin values can be tested without values from
//one test crossing over into the next (local, so they reset after every call)
//
void sar_convert(float Vref, float Vin) {
    int bits[BITSIZE];
    float quantized = 0.0f;
    int bitval;

    //set MSB threshold with Vref/2, then keep halfing with each loop
    //that is iterated through - soime good old binary search logic 
    Vref /= 2.0f;
    float thresh = Vref;

    for (int count = 0; count < BITSIZE; count++) {
        Vref /= 2.0f;
        if (Vin > thresh) {
            bitval = 1;
            thresh += Vref; //seach window up if Vin is above threshold
        } else {
            bitval = 0;
            thresh -= Vref; //seach window down if Vin is below threshold
        }
        bits[count] = bitval;
        // multiply by 2 to undo the extra halving done at the start of this iteration
        quantized += 2.0f * Vref * bitval;
    }

    printf("Vin = %.4fV  |  Bits: ", Vin);
    for (int i = 0; i < BITSIZE; i++) printf("%d", bits[i]);
    printf("  |  Quantized = %.6fV  |  Error = %.6fV\n", quantized, Vin - quantized);
}

int main(void) {
    float Vref = 3.0f;
    // replaced hardcoded single Vin with an array to test all four voltages in one run
    float voltages[] = {0.42f, 0.83f, 1.65f, 2.752f};
    int n = sizeof(voltages) / sizeof(voltages[0]);

    printf("14-bit SAR ADC Simulation (Vref = %.1fV)\n", Vref);
    printf("--------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        sar_convert(Vref, voltages[i]);
    }

    return 0;
}



