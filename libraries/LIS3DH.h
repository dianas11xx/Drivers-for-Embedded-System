#ifndef LIS3DH_H
#define LIS3DH_H

#include <stdint.h>


class LIS3DH {

public:
     // xyz acceleration values in units of g
    float x, y, z;

    /* Class constructor to instantiate object */
    LIS3DH(void);

    /* Initializes the accelerometer, returning
    true on success or false on failure*/
    bool init(void);

    /* Set a register on the LIS3DH to the given value */
    void set_reg(uint8_t reg, uint8_t val);

    /* Reads and returns the byte at address reg on the
       accelerometer */ 
    uint8_t read_reg(uint8_t reg);

    /* Updates the class members x, y, and z with the 
       current acceleration values */
    void update(void);

};


#endif