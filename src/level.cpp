#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "../libraries/LIS3DH.h"
#include "../libraries/NeoMatrix.h"
#include "pico/stdlib.h"

int main(){

    stdio_init_all();

    // Initialize accelerometer 
    LIS3DH acccelerometer = LIS3DH();
    acccelerometer.init();

    // Initialize neomatrix as an 8x8
    NeoMatrix pixels = NeoMatrix(8,8);
    pixels.init();

    // Initialize variables
    uint32_t red = 0x00FF0000;
    uint32_t green = 0xFF000000;

    float min_acc = -1.0;
    float max_acc = 1.0;

    float x, y;
    int row, col;

    while(true){
        // Get current xyz readings
        acccelerometer.update();
        pixels.clear_pixels();

        // Check/readjust if xy values go under/over the min/max
        if(acccelerometer.x > 1.0f){
            x = 1.0f;
        }else if(acccelerometer.x < -1.0f){
            x = -1.0f;
        }else{
            x = acccelerometer.x;
        }

        if(acccelerometer.y > 1.0f){
            y = 1.0f;
        }else if(acccelerometer.y < -1.0f){
            y = -1.0f;
        }else{
            y = acccelerometer.y;
        }

        // Remap x and y to be in the range of 0-7 for the row and col
        row = round((7.0/(max_acc - min_acc)) * (y - min_acc));
        col = round((7.0/(max_acc - min_acc)) * (x - min_acc));

        // If x and y are within+-0.1g, set up 4 center LED's green
        if(acccelerometer.x < 0.1 && acccelerometer.x > -0.1 && acccelerometer.y < 0.1 && acccelerometer.y > -0.1){
            pixels.set_pixel(3, 3, green);
            pixels.set_pixel(3, 4, green);
            pixels.set_pixel(4, 3, green);
            pixels.set_pixel(4, 4, green);
        }else{
            // Set red light in location indicating axis of tilt
            pixels.set_pixel(row, col, red);
        }

        // Write pixels to display
        pixels.write();
        sleep_ms(200);
    }

    return 0;
}

