#include "Disk.h"
#include "FileSystem.h"

int main(){
  FileSystem fileSystem;
  fileSystem.reformat();

  printf("test.txt\n");
  FCB *fcb = fileSystem.open("test.txt");
  if(fcb != NULL){
    printf("open(..) returned fcb at block %d\n", fcb->fcb_dir_index);
  }

  printf("file.txt\n");
  fcb = fileSystem.open("file.txt");
  if(fcb != NULL){
    printf("open(..) returned fcb at block %d\n", fcb->fcb_dir_index);
  }



}
