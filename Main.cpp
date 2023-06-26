#include "Disk.h"
#include "FileSystem.h"
#include <cstdio>
#include <sys/stat.h>

FileSystem fileSystem;

void usage(){
    fprintf(stderr, "usage:  ./Main cmd [option]\n");
    fprintf(stderr, "cmd:    --list, --reformat, --read, --write\n");
    fprintf(stderr, "option: filename\n");
    fprintf(stderr, "--read:  reads from file into simulation\n");
    fprintf(stderr, "--write: writes from simulation into file\n");
    fprintf(stderr, "E.g., ./Main --reformat\n");
    fprintf(stderr, "E.g., ./Main --read  /path/to/file.txt file.txt\n");
    fprintf(stderr, "E.g., ./Main --write file.txt /path/to/file.txt\n");
}

void read_into_filesystem(char *path, char *filename){
  FILE *fptr = fopen(path, "r");
  struct stat sb;
  fstat(fileno(fptr), &sb);

  char *contents = (char *) malloc(sb.st_size);
  fread(contents, 1, sb.st_size, fptr);

  FCB *fcb = fileSystem.open(filename);
  fileSystem.write(fcb, (uint8_t*) contents, sb.st_size);
  fileSystem.close(fcb);

  free(contents);
}

void write_from_filesystem(char *filename, char *path){
  FCB *fcb = fileSystem.open(filename);
  if(fcb == NULL){
    fprintf(stderr, "ERROR: no such file %s\n", filename);
    return;
  }

  char *contents = (char *) malloc(fcb->size);
  fileSystem.seek(fcb, 0);
  fileSystem.read(fcb, (uint8_t *) contents, fcb->size);
  fileSystem.close(fcb);

  FILE *fptr = fopen(path, "w");
  fwrite(contents, 1, fcb->size, fptr);
  fclose(fptr);

  free(contents);
}

int main(int argc, char **argv){

  if(argc == 1 || argc > 4){
    usage();
    exit(1);
  }

  fileSystem.set_safe_write(true);

  if(argc == 2){
    if(strcmp(argv[1], "--reformat") == 0){
      fileSystem.reformat();
    } else if(strcmp(argv[1], "--list") == 0){
      fileSystem.ls();
    } else {
      fprintf(stderr, "invalid cmd\n");
      exit(1);
    }
  }

  if(argc == 4){
    if(strcmp(argv[1], "--read") == 0){
      read_into_filesystem(argv[2], argv[3]);
    } else if(strcmp(argv[1], "--write") == 0){
      write_from_filesystem(argv[2], argv[3]);
    } else {
      fprintf(stderr, "invalid cmd\n");
      exit(1);
    }

  }
}



