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

unsigned int FileSystem::find_empty_block(){
  uint8_t b, j; 
  unsigned int block;
  int i;

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
  return 0;
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

unsigned int FileSystem::find(char *filename){
  FCB fcb;

  for(int i = 0; i < DIR_SIZE; i++){
    if(fcb_dir[i] != 0){
      disk.read_block(fcb_dir[i], (uint8_t *) &fcb);
      if(strcmp(filename, fcb.filename) == 0){
        return fcb_dir[i]; //file exists, return block #
      }
    }
  } 
  return 0; //nothing found
}

FCB* FileSystem::open(char *filename){
  FCB *fcb;
  fcb = (FCB *) malloc(sizeof(FCB));
  unsigned int block = find(filename);
  if(block == 0){
    //find slot in FCB directory
    int dir_index = 0; 
    for(int i = 0; i < DIR_SIZE; i++){
      if(fcb_dir[i] == 0){
        dir_index = i;
        break;
      }
    }
    //error checking (dir_index == 0).. 

    //get emtpy block 
    block = find_empty_block();
    printf("HERE block = %d\n", block);
    if(block == 0){
      return NULL;
    }

    //set block as occupied
    allocate(block);

    //store block # of FCB in directory
    fcb_dir[dir_index] = block;

    //write FCB directory back to disk
    disk.write_block(1, (uint8_t *) fcb_dir);

    //create FCB
    fcb = (FCB *) malloc(sizeof(FCB));
    strncpy(fcb->filename, filename, LEN_FILENAME);
    fcb->offset = 0;
    fcb->fcb_dir_index=block;
    fcb->size = 0;
    for(int i = 0; i < NUM_BLOCKS; i++){
      fcb->ptr[i] = 0;
    }

    //write FCB to disk at given block 
    disk.write_block(block, (uint8_t*) fcb);

    return fcb;

    
  } else {
    disk.read_block(block, (uint8_t *) fcb);
    return fcb;
  }
  return NULL;
}

