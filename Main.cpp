#include "Disk.h"
#include "FileSystem.h"

int main(){
  FileSystem fileSystem;
  fileSystem.reformat();

  for(int i = 0; i < 1; i++){
    uint8_t b = fileSystem.free_list[i];
    for(uint8_t j = 0; j < 8; j++){
      printf("%d", (b >> j) & 0x01);
    }
    printf("\n");
  }

  printf("%d\n", fileSystem.find_empty_block());
  printf("%d\n", fileSystem.find_empty_block());
  printf("%d\n", fileSystem.find_empty_block());
  printf("%d\n", fileSystem.find_empty_block());
  printf("%d\n", fileSystem.find_empty_block());
}
