#pragma once
#include "Arduino.h"
#include <Microchip_24LC256.h>
#include <Streaming.h>

#define LEN 28
#define NUM_PTRS 16
#define VERBOSE 0

typedef struct{
    char str[LEN];
    unsigned int dir_index; //index in FCB_dir
    unsigned int fptr; //offset into file
    unsigned int ptr[NUM_PTRS]; //pointers to data pages on "disk"
} FCB;

class FS {
  public:
    int format();
    void init();
    int create_file(const char *str);
    int open_file(const char *str, FCB *fcb);
    int write_file(FCB *fcb, void* buffer, unsigned int num);
    int read_file(FCB *fcb, void* buffer, unsigned int num);
    void rewind_file(FCB *fcb);
    void list_files();
    void close_file(FCB *fcb);
    int delete_file(const char *str);
    Microchip_24LC256 eeprom;
    byte fcb_dir[PAGE_SIZE];
    
  private:
    //Microchip_24LC256 eeprom;
    byte free_list[PAGE_SIZE];
    //byte fcb_dir[PAGE_SIZE];
    byte current_page[PAGE_SIZE];
    byte bit_mask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

    //helper functions
    int find_empty_page();
    void mark_page(unsigned int page);
    void unmark_page(unsigned int page);
    int find_file(const char *str);
};
