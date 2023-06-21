#include "Disk.h"
#include "FileSystem.h"

/*
 * TODO: 
 * - read in file, write to FileSystem
 *  UNIX> ./Main -r file.pdf 
 *
 * - read from FileSystem, write to file
 *  UNIX> ./Main -w file.pdf 
 */

int main(){
  FileSystem fileSystem;
  fileSystem.reformat();

  printf("=== LS ===\n");
  fileSystem.ls();
  printf("==========\n");

  FCB *fcb = fileSystem.open("test.txt");
  fileSystem.write(fcb, (uint8_t *) "HELLO\0", 6);
  fileSystem.close(fcb);

  printf("=== LS ===\n");
  fileSystem.ls();
  printf("==========\n");

  char data[80];
  fileSystem.seek(fcb, 0);
  fileSystem.read(fcb, (uint8_t *) data, 6);
  printf("%s\n", data);

  fileSystem.remove("test.txt");
  printf("=== LS ===\n");
  fileSystem.ls();
  printf("==========\n");



}
