#include "FileSystem.h"

FileSystem::FileSystem(){
  //load free list and fcb dir from disk
  disk.read_block(0, free_list);
  disk.read_block(1, (uint8_t *) fcb_dir);
}

FileSystem::~FileSystem(){

}

void FileSystem::reformat(){
  memset(free_list, 0xFF, BLOCK_SIZE); //set free list to all 1s
  memset(fcb_dir, 0x00, BLOCK_SIZE); //clear fcb dir

  //reformat disk and write changes
  disk.reformat();

  disk.write_block(0, free_list);
  allocate(0);
  
  disk.write_block(1, (uint8_t *) fcb_dir);
  allocate(1);
}

int FileSystem::find_empty_block(){
  uint8_t b, j; 
  int i, block;

  disk.read_block(0, free_list);

  for(i = 0; i < BLOCK_SIZE; i++){
    b = free_list[i]; 
    for(j = 0; j < 8; j++){
      if( ((b >> j) & 0x01) == 1){
        block = (i*8) + j;
        allocate(block);
        return block;
      }
    }
  }
  return -1;
}


void FileSystem::allocate(unsigned int block){
  uint8_t bit_mask[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
  unsigned int index, offset;

  index = block / 8;
  offset = block % 8;

  //set block to 0 (in-use) 
  free_list[index] &= ~(bit_mask[offset]);
  disk.write_block(0, free_list);
}

void FileSystem::deallocate(unsigned int block){
  uint8_t bit_mask[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
  unsigned int index, offset;

  index = block / 8;
  offset = block % 8;

  //set block to 1 (free)
  free_list[index] |= bit_mask[offset];
  disk.write_block(0, free_list);

}
