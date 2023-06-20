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
  int i = 0;

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
    strcpy(fcb->filename, filename);
    fcb->offset = 0;
    fcb->fcb_dir_index=dir_index;
    fcb->size = 0;
    for(int i = 0; i < NUM_BLOCKS; i++){
      fcb->ptrs[i] = 0;
    }

    //write FCB to disk at given block 
    disk.write_block(block, (uint8_t*) fcb);

    return fcb;

  } else { //fcb was found in directory
    disk.read_block(block, (uint8_t *) fcb);
    return fcb;
  }
  return NULL;
}

int FileSystem::write(FCB *fcb, uint8_t *buffer, unsigned int len){
  //retrieve block of current file offset 
  // allocate block if necessary 
  //start writing bytes into buffer
  // allocate next block if / when necessary 
  uint8_t block_data[BLOCK_SIZE];
  unsigned int new_block;

  unsigned int block_index;
  unsigned int ptrs_index;

  ptrs_index = fcb->offset / BLOCK_SIZE; //determine which block_ptr 
  block_index = fcb->offset % BLOCK_SIZE; //index within that block
                                          
  //if file has no ptrs to allocated blocks
  if(fcb->ptrs[ptrs_index] == 0){
    new_block = find_empty_block();
    //error checking..

    //store allocated block into file's ptrs 
    fcb->ptrs[ptrs_index] = new_block;
  
    //write FCB changes to disk
    disk.write_block(fcb_dir[fcb->fcb_dir_index], (uint8_t *) fcb);

    //clear block_data
    memset(block_data, 0x00, BLOCK_SIZE);

  } else {
    //read block in from disk 
    disk.read_block(fcb->ptrs[ptrs_index], (uint8_t *) block_data);
  }

  //go through each byte in buffer 
  for(unsigned int buffer_index = 0; buffer_index < len; buffer_index++){
    block_data[block_index] = buffer[buffer_index]; 
    
    fcb->offset++;
    block_index++;

    //go past end of current block
    if(block_index >= BLOCK_SIZE){
      block_index = 0;

      //write current block to disk
      disk.write_block(fcb->ptrs[ptrs_index], (uint8_t *) block_data);

      //get next or new block
      ptrs_index++;
      //error checking.. 

      if(fcb->ptrs[ptrs_index] == 0){
        new_block = find_empty_block();
        fcb->ptrs[ptrs_index] = new_block;
        disk.write_block(fcb_dir[fcb->fcb_dir_index], (uint8_t *) fcb);
        memset(block_data, 0x00, BLOCK_SIZE);
      } else {
        disk.read_block(fcb->ptrs[ptrs_index], (uint8_t *) block_data);
      }
    }
  }
  //write block update to disk 
  disk.write_block(fcb->ptrs[ptrs_index], (uint8_t *) block_data);
  
  //write FCB updates to disk 
  disk.write_block(fcb_dir[fcb->fcb_dir_index], (uint8_t *) fcb);

  return 0;
}

void FileSystem::close(FCB *fcb){
  //on close, write FCB to disk 
  disk.write_block(fcb_dir[fcb->fcb_dir_index], (uint8_t *) fcb);
}
