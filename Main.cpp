#include "Disk.h"
#include "FileSystem.h"
#include <cstdio>
#include <sys/stat.h>

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

  FILE *fptr = fopen("../data/plaintext/mov/A.mov", "r");
  struct stat sb;
  fstat(fileno(fptr), &sb);

  char *contents = (char *) malloc(sb.st_size);
  fread(contents, 1, sb.st_size, fptr);

  FCB *fcb = fileSystem.open("A.mov");
  fileSystem.write(fcb, (uint8_t*) contents, sb.st_size);
  fileSystem.close(fcb);

  char *data = (char *) malloc(sb.st_size);
  fcb = fileSystem.open("A.mov");
  fileSystem.seek(fcb, 0);
  fileSystem.read(fcb, (uint8_t *) data, sb.st_size);
  fileSystem.close(fcb);

  for(int i = 0; i < sb.st_size; i++){
    printf("%c", data[i]);
  }



  fclose(fptr);

}

