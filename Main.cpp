#include "Disk.h"

int main(){
  Disk disk;
  uint8_t data[4096]; 

  for(int i = 0; i < 4096; i++){
    char c = 'a' + (i%26);
    memset(data, c, 4096);
    disk.write_block(i, data);
  }

  /*
  for(int i = 0; i < 4096; i++){
    memset(data, '-', 4096);
    disk->read_block(i, data);
    data[4096-1] = '\0';
    printf("%s\n", data);
  }

  */
  //return 0;


}

