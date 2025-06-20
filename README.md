# Drivers-for-Embedded-System
## Description

This project creates an I2C driver for the LIS3DH accelerometer and a NeoMatrix display driver. These drivers are delivered as libraries that creates an intuitive and robust interface for application software. A Bubble Level application is included to demonstrate how the libraries could be used for controlling the accelerometer and NeoMatrix display. 

The boards used in this project are:
- Adafruit Feather RP2040 
- Adafruit Propmaker FeatherWing
- Adafruit 8x8 NeoMatrix

## Project Structure
---
### Libraries Folder
The code for the two drivers are defined and declared in the libraries folder. This folder also contains a `generated` folder, which holds the pio header file `ws2812.pio.h` used in the NeoMatrix driver class to setup and write data to the state machine. The ws2812.pio file is used to make this header file.

#### LIS3DH library
The code for the I2C driver is created in the `libraries` folder.
- `LIS3DH.h` is where the LIS3DH class and its functions are declared 
- `LIS3DH.cpp` is where the functions for the LIS3DH class are defined. 

The following functions and variables are avaliable to use from the I2C driver:
- `float x, y, z`: Variables representing the three acceleration values in units of g (Earth's gravitational acceleration)
- `LIS3DH(void)`: Class constructor used to instantiate I2C driver object in project
- `bool init(void)`: Initializes the accelerometer by initializing I2C instance 1, since the GPIO pins for SDA(GPIO2) and SCL(GPIO3) are connected to that instance. It also makes the I2C pins avaliable to picotool and sets the CTRL_REG1 register to normal mode with a 1.344kHz data rate while also enabling xyz.
- `set_reg(uint8_t reg, uint8_t val)`: Sets the given register (reg) to the given value (val) on the LIS3DH.
- `uint8_t read_reg(uint8_t reg)`: Reads the byte at the address the given register is at and returns the data read using I2C protocols to read/write to the data bus. 
- `update(void)`: Updates the class members x, y, z with the current acceleration values by using the `read_reg(register)` to read and return the byte at each XYZ acceleration register addresses. It also scales the raw data before updating the values.

#### NeoMatrix library
The code for the NeoMatrix display driver is created in the `libraries` folder.
- `NeoMatrix.h` is where the NeoMatrix class and its functions are declared 
- `NeoMatrix.cpp` is where the functions for the NeoMatrix class are defined. 

The following functions and variables are avaliable to use from the NeoMatrix display driver:
- `NeoMatrix(uint8_t width, uint8_t height)`: Class constructor used to instantiate the NeoMatrix object by setting the width and height and initializing the pixel buffer, which holds the color of each pixel using a vector.
- `bool init(void)`: Initializes the object by driving the pin that enables the power to the connected NeoPixels high and setting up the PIO peripherial. Returns true on success
- `set_pixel(uint8_t row, uint8_t col, uint32_t color)`: Sets the pixel at the given row and column (zero-indexed) to the given color.
- `clear_pixels(void)`: Sets all pixels in the pixel buffer to 0x00 to clear the color.
- `write(void)`: Writes the pixel buffer to the NeoMatrix.

### Src Folder
This folder holds the code in `level.cpp` used for the Bubble Level example. It imports the `LIS3DH.h` and `NeoMatrix.h` libraries created in the libraries folder. Here is the following structure of the example:
- Instantiate and initalize the accelerometer using the `LIS3DH()` constructor and `init()` functions
- Instantiate and initialize the NeoMatrix display object as an 8x8 using the `NeoMatrix(8, 8)` constructor and `init()` functions for that object.
- Loop forever:
- Update the accelerometer values so that the XYZ values are ready to use using the `update()` function.
- Clear the pixel buffer using the `clear_pixels()` function
- Check/readjust x and y values of accelerometer if they go over/under the max/min values.
- Remap the x and y to be in range of 0-7 for the row and column
- If x and y are within +- 0.1g, set up 4 center LED's to green using the `set_pixel(row, col, green)` function for the corresponding row and col for each pixel.
- Otherwise, set the pixel at the row and col value calculated from the remap to red using the `set_pixel(row, col, red)`
- Finally, write the pixels to display using the `write()` function.

## Prerequisites
---

Since this project uses the interfaces provided by SDK, you have to clone the [pico-sdk repository](https://github.com/raspberrypi/pico-sdk) to your computer. 

### How to get SDK
- `cd` into the directory you want to place this folder in.
- Clone the [pico-sdk](https://github.com/raspberrypi/pico-sdk) repository using the command `git clone https://github.com/raspberrypi/pico-sdk`
- To ensure the tinyusb module is included, `cd` into the pico-sdk directory and run `git submodule update --init`

### Install CMake Toolchain 
To build the project, you'll need CMake, which could be installed via `apt` from the command line. Run:
``` 
    sudo apt update
    sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential 
    sudo apt update
```

## How to Build
---
Once you have installed SDK and the relevant toolchains, you can now build the project!

- Download the ZIP archive and `extract` it into the same folder that the pico-sdk folder is in 
- `cd` into the folder and create a `build` directory using the command `mkdir build`
- `cd` into the `build` folder and set the path using  the command `export PICO_SDK_PATH-=../../pico-sdk`
- Prepare the cmake build directory by running `cmake ..`
- Run `make`

This will create a loadable `level.uf2` file that you can load to your board.

## How to Load Code
---
Once you finished building the project, you can flash the board with the compiled code via the built-in permanent ROM UF2 bootloader
- Reboot the board by holding down the `BOOTSEL` button on the Adafruit Feather rp2040 while clicking the `RESET` button or powering it on.
- Drag or copy the `level.uf2` file from the `build` folder onto the filesystem.
