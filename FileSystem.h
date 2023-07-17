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

class FileSystem {
  public:
    FileSystem();
    ~FileSystem();

    void set_safe_write(bool, char *);
    void reformat(); //reset free_list, FCB_dir
    FCB* fs_open(char *filename); //open file in read/write mode
    void fs_close(FCB *fcb); 
    int  fs_read(FCB *fcb, uint8_t* buffer, unsigned int num);
    bool fs_write(FCB *fcb, uint8_t* buffer, unsigned int num);
    void fs_delete(char *filename);
    int fs_seek(FCB *fcb, int offset);
    void ls();

  private:
    bool safe_write;
    Disk disk;
    uint8_t free_list[BLOCK_SIZE];
    unsigned int fcb_dir[DIR_SIZE];
    FILE *f_in; //read FROM predictor process
    FILE *f_out; //writes TO predictor process
    char *model;

    unsigned int find_empty_block();
    void   allocate(unsigned int block);
    void deallocate(unsigned int block);
    unsigned int find(char *filename);
    bool write_to_disk(FCB *, unsigned int block, uint8_t *data); //true if write OK
    bool predict(char *mime, uint8_t *block); //true if predict is plaintext
    void setup_predictor(char *model);
};

