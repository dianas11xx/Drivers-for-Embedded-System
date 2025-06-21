#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "NeoMatrix.h"
using namespace std;


#define NEO_PIN_GPIO7 7


 /* Class constructor. Used to instantiate an object */
NeoMatrix::NeoMatrix(uint8_t width, uint8_t height){
    _width = width;
    _height = height;
    // initilaize vector of size width*height with 0x00
    for(int i = 0; i < (width*height); i++){
        pixBuf.push_back(0x00);
    }
 };


/* Initialize the object, returning true on success
   or false on failure */
bool  NeoMatrix::init(void){

    // Set PWR_PIN to output and drive pin high 
    const uint PWR_PIN = 10;
    gpio_init(PWR_PIN);
    gpio_set_dir(PWR_PIN, GPIO_OUT);
    gpio_put(PWR_PIN, 1);

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, NEO_PIN_GPIO7, 800000, false);

    return true;
};

/* Set the pixel at row row and column col (zero indexed)
   to color*/
void  NeoMatrix::set_pixel(uint8_t row, uint8_t col, uint32_t color){
    int index = (row*_height) + col;
    pixBuf[index] = color;
};


/* Set all elements of the pixel buffer to 0x00 */
void  NeoMatrix::clear_pixels(void){
    for(int i = 0; i < (_width*_height); i++){
        pixBuf[i] = 0x00;
    }
};


/* Write the pixel buffer to the NeoMatrix*/
void NeoMatrix::write(void){
    for(int i = 0; i < pixBuf.size(); i++){
        pio_sm_put_blocking(pio0, 0, pixBuf[i]);
    }
};