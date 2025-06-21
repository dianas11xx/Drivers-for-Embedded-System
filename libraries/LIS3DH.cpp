#include<iostream>
#include <stdio.h>
#include "LIS3DH.h"
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
using namespace std;

// Define SDA and SCL pins for Feather rp2040
#define F_I2C_SDA_PIN 2
#define F_I2C_SCL_PIN 3

// Define XYZ acceleration register addresses
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D 

// Define bus and ctrl reg1 address
const int ADDRESS = 0x18;
const uint8_t CTRL_REG_1 = 0x20;


/* LIS3DH constructor to instantiate object */
LIS3DH::LIS3DH(void){
    // Set xyz values to 0
    x = y = z = 0.0;
};


/* Initializes the accelerometer, returning
   true on success or false on failure */
bool LIS3DH::init(void){
    // use I2C1, GPIO2 for SDA, and GPIO3 for SCL
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(F_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(F_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(F_I2C_SDA_PIN);
    gpio_pull_up(F_I2C_SCL_PIN);

    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(F_I2C_SDA_PIN, F_I2C_SCL_PIN, GPIO_FUNC_I2C));

    /* Set normal mode, 1.344kHz data rate (ODR bits 1001), 
    and enable xyz */ 
    set_reg(CTRL_REG_1, 0x97);
    return true;
};


/* Set a register on the LIS3DH to the given value */
void LIS3DH::set_reg(uint8_t reg, uint8_t val){
    uint8_t buf[2];
    buf[0] = reg;
    buf[1] = val;
    // Use instance 1 of i2c 
    i2c_write_blocking(i2c1, ADDRESS, buf, 2, false);
};


/* Reads and returns the byte at address reg on the
   accelerometer */ 
uint8_t LIS3DH::read_reg(uint8_t reg){
    uint8_t byte;

    // Write data at address reg (from accelerometer) to data bus
    i2c_write_blocking(i2c1, ADDRESS, &reg, 1, true);
    // Read data from accelerometer to byte
    i2c_read_blocking(i2c1, ADDRESS, &byte, 1, false);

    return byte;
};


/* Updates the class members x, y, and z with the 
   current acceleration values */
void LIS3DH::update(void){
    uint8_t lsb, msb;
    uint16_t x_temp, y_temp, z_temp;

    // Read low and high bytes of XYZ acceleration data
    lsb = read_reg(OUT_X_L);
    msb = read_reg(OUT_X_H);
    x_temp = (msb << 8) | lsb;
    lsb = read_reg(OUT_Y_L);
    msb = read_reg(OUT_Y_H);
    y_temp = (msb << 8) | lsb;
    lsb = read_reg(OUT_Z_L);
    msb = read_reg(OUT_Z_H);
    z_temp = (msb << 8) | lsb;

    // Convert with respect to the value being acceleration
    float scaling = 64 / 0.004f;
    x = (float) ((int16_t) x_temp) / scaling;
    y = (float) ((int16_t) y_temp) / scaling;
    z = (float) ((int16_t) z_temp) / scaling;
};