#pragma once 
#include <cstring>
#include <stdlib.h>

#define BLOCK_SIZE 4096
#define DISK_SIZE BLOCK_SIZE*BLOCK_SIZE
using namespace std;

class Disk {
  public: 
    void write_block(unsigned int addr, uint8_t *data);
    void  read_block(unsigned int addr, uint8_t *data);
    void reformat();

  private:
    uint8_t disk[DISK_SIZE];
    void save_disk_to_file();
    void read_disk_from_file();
};
