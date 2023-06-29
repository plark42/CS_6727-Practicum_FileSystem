#include "Disk.h"
#include "FileSystem.h"
#include <cstdio>
#include <sys/stat.h>

FileSystem fileSystem;

void usage(){
  fprintf(stderr, "usage: ./Test model.pkl infile.txt\n");
  fprintf(stderr, "attempts to write infile.txt using model.pkl to detect\n");
  exit(1);
}

void write_into_filesystem(char *path, char *filename){
  FILE *fptr = fopen(path, "r");
  struct stat sb;
  fstat(fileno(fptr), &sb);

  char *contents = (char *) malloc(sb.st_size);
  fread(contents, 1, sb.st_size, fptr);

  FCB *fcb = fileSystem.fs_open(filename);
  if(fileSystem.fs_write(fcb, (uint8_t*) contents, sb.st_size)){
    printf("%s SUCCESS\n", path);
  } else {
    printf("%s FAILED\n", path);
  }
  fileSystem.fs_close(fcb);

  free(contents);
}

int main(int argc, char **argv){

  if(argc != 3){
    usage();
  }

  fileSystem.reformat();
  fileSystem.set_safe_write(true, argv[1]);
  write_into_filesystem(argv[2], "TESTFILE");
}



