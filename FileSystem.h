#pragma once
#include "Disk.h"

#define LEN_FILENAME 84
#define NUM_BLOCKS 1000
#define DIR_SIZE BLOCK_SIZE/sizeof(unsigned int)

//unsigned int 4 bytes 
//
typedef struct {
  char filename[LEN_FILENAME]; //
  unsigned int offset; //ofset into file
  unsigned int fcb_dir_index; //backref to location of FCB
  unsigned int size; //file size
  unsigned int ptrs[NUM_BLOCKS]; //singly-indexed allocation 
} FCB;

class FileSystem {
  public:
    FileSystem();
    ~FileSystem();

    void set_safe_write(bool);
    void reformat(); //reset free_list, FCB_dir
    FCB* open(char *filename); //open file in read/write mode
    void close(FCB *fcb); 
    int  read(FCB *fcb, uint8_t* buffer, unsigned int num);
    int write(FCB *fcb, uint8_t* buffer, unsigned int num);
    void remove(char *filename);
    int seek(FCB *fcb, int offset);
    void ls();

  private:
    bool safe_write;
    Disk disk;
    uint8_t free_list[BLOCK_SIZE];
    unsigned int fcb_dir[DIR_SIZE];
    unsigned int find_empty_block();
    void   allocate(unsigned int block);
    void deallocate(unsigned int block);
    unsigned int find(char *filename);
    bool write_to_disk(unsigned int block, uint8_t *data);
};

