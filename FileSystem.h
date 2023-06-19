#pragma once
#include "Disk.h"

#define LEN 256
#define NUM_PTRS 16

typedef struct {
  char name[LEN];
  uint16_t fcb_dir_index; //
  unsigned int offset; 
  unsigned int ptr[NUM_PTRS];
} FCB;

class FileSystem {
  public:
    FileSystem();
    ~FileSystem();

    //disk block containing block #s of all FCBs
    uint16_t fcb_dir[BLOCK_SIZE/2];

  private:
    uint8_t free_list[BLOCK_SIZE];
    uint8_t bit_mask[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
    
    int find_empty_block;
    void   mark_block(unsigned int block);
    void unmark_block(unsigned int block);
    


};
