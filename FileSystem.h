#pragma once
#include "Disk.h"

#define LEN_FILENAME 52
#define NUM_BLOCKS 2016
//2016 = (4096 - 52 - 4 - 4 - 4) / 2

//unsigned int 4 bytes 

typedef struct {
  char name[LEN_FILENAME];
  unsigned int offset; //ofset into file
  unsigned int fcb_dir_index; //backref to location of FCB
  unsigned int size; //file size
  uint16_t ptr[NUM_BLOCKS]; //singly-indexed allocation 
} FCB;

class FileSystem {
  public:
    FileSystem();
    ~FileSystem();

    void reformat(); //reset free_list, FCB_dir

    //disk block containing block #s of all FCBs
    uint16_t fcb_dir[BLOCK_SIZE/2];

    Disk disk;
    uint8_t free_list[BLOCK_SIZE];
    
    int find_empty_block();
    void   allocate(unsigned int block);
    void deallocate(unsigned int block);
};
