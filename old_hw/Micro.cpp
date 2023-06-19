#include "Microchip_24LC256.h"

Microchip_24LC256::Microchip_24LC256()
{
    Wire.begin();
}

void Microchip_24LC256::write_byte( unsigned int addr, byte b)
{
    wait();
    
    //send START command
    Wire.beginTransmission(I2C_ADDR);
    
    //write address then byte
    Wire.write( highByte(addr));
    Wire.write(  lowByte(addr));
    Wire.write(b);
    Wire.endTransmission(); // send bytes + STOP command
    
    wait();
    
}

byte Microchip_24LC256::read_byte( unsigned int addr)
{
    wait();
    byte b;
    
    //send START command
    Wire.beginTransmission(I2C_ADDR);
    
    //write address to read from
    Wire.write( highByte(addr));
    Wire.write(  lowByte(addr));
    Wire.endTransmission(); //send bytes
    
    //send request to read
    Wire.requestFrom(I2C_ADDR, 1);
    
    //wait for bytes to read, then read bytes
    while (1) {
        if (Wire.available()) {
            b = Wire.read();
            break;
        }
    }
    
    //send STOP cmd
    Wire.endTransmission();
    
    return b;
}

void Microchip_24LC256::write_page( unsigned int page, byte *array)
{
    unsigned int addr = page * PAGE_SIZE;
    unsigned int i;
    
    for (i = 0; i < PAGE_SIZE; i++) {
        write_byte(addr + i, array[i]);
    }
}

void Microchip_24LC256::read_page( unsigned int page, byte *array)
{
    unsigned int addr = page * PAGE_SIZE;
    unsigned int i;
    
    for (i = 0; i < PAGE_SIZE; i++) {
        array[i] = read_byte(addr + i);
    }
}

//poll EEPROM chip...
void Microchip_24LC256::wait()
{
    do {
        Wire.beginTransmission(I2C_ADDR);
    } while (Wire.endTransmission() != 0);
}
