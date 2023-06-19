#include <Arduino.h>
#include <Wire.h>

#pragma once

#define PAGE_SIZE 64
#define I2C_ADDR 0x50

class Microchip_24LC256
{
    public:
        Microchip_24LC256();
        void write_byte( unsigned int addr, byte b);
        void write_page( unsigned int page, byte *array);
    
        byte  read_byte( unsigned int addr);
        void  read_page( unsigned int page, byte *array);
    
    private:
        void wait();
};
