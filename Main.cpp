#include "Disk.h"
#include "FileSystem.h"

int main(){
  FileSystem fileSystem;
  //fileSystem.reformat();

/*
  printf("test.txt\n");
  FCB *fcb = fileSystem.open("test.txt");
  if(fcb != NULL){
    printf("open(..) returned fcb at block %d\n", fcb->fcb_dir_index);
  } else {
    printf("NULL!\n");
    exit(0);
  }
  fileSystem.write(fcb, (uint8_t *) "HELLO\0", 6);
  printf("fcb->offset = %d\n", fcb->offset);
  fileSystem.close(fcb);

  printf("file.txt\n");
  fcb = fileSystem.open("file.txt");
  if(fcb != NULL){
    printf("open(..) returned fcb at block %d\n", fcb->fcb_dir_index);
  } else {
    printf("NULL\n");
  }
  

  uint8_t data[10000];
  memset(data, 'A', 10000);
  data[9999] = '\0';
  fileSystem.write(fcb, data, 10000);
  fileSystem.close(fcb);

*/

  fileSystem.ls();

  FCB *fcb = fileSystem.open("file.txt");
  printf("fcb->size = %d\n", fcb->size);
  uint8_t buffer[10*BLOCK_SIZE];
  fileSystem.seek(fcb, 0);
  fileSystem.read(fcb, buffer, fcb->size);//->size);
  for(int i = 0; i < 10*BLOCK_SIZE; i++){
    printf("%c", buffer[i]);
  }
  printf("\n");
  fileSystem.close(fcb);




}
