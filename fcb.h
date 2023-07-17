#pragma once
#include "Disk.h"
#include <unistd.h>
#include <sys/time.h>

#define LEN_FILENAME 100
#define MIME_LEN 100
#define NUM_BLOCKS 971
#define DIR_SIZE BLOCK_SIZE/sizeof(unsigned int)

//unsigned int 4 bytes 
//4096 - (100 + 100 + 4 + 4 + 4) = 3884 / 4 => 971
typedef struct {
  char filename[LEN_FILENAME]; //
  char mime[MIME_LEN];
  unsigned int offset; //ofset into file
  unsigned int fcb_dir_index; //backref to location of FCB
  unsigned int size; //file size
  unsigned int ptrs[NUM_BLOCKS]; //singly-indexed allocation 
} FCB;

