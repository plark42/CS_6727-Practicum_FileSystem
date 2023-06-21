#pragma once 
#include <cstring>
#include <cstdlib>
#include <cstdio>

#define BLOCK_SIZE 4096
#define DISK_SIZE BLOCK_SIZE*BLOCK_SIZE
#define FILENAME "disk.txt"

class Disk {
  public: 
    Disk();
    ~Disk();
    void write_block(unsigned int block, uint8_t *data);
    void  read_block(unsigned int block, uint8_t *data);
    void shred_block(unsigned int block);
    void reformat();

    uint8_t *disk;
    void write_disk_to_file();
    void read_disk_from_file();
};
