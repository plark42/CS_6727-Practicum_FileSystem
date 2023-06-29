#include "Disk.h"
#include "FileSystem.h"
#include <cstdio>
#include <sys/stat.h>

FileSystem fileSystem;

void usage(){
    fprintf(stderr, "usage:  ./Main cmd [option]\n");
    fprintf(stderr, "cmd:    --list, --reformat, --read, --write\n");
    fprintf(stderr, "option: filename\n");
    fprintf(stderr, "--write: writes file into simulation\n");
    fprintf(stderr, "--safe_write: safely writes file into simulation\n");
    fprintf(stderr, "--read:   reads file from simulation\n");
    fprintf(stderr, "E.g., ./Main --reformat\n");
    fprintf(stderr, "E.g., ./Main --write /path/to/file.txt file.txt\n");
    fprintf(stderr, "E.g., ./Main --safe_write /path/to/file.txt file.txt\n");
    fprintf(stderr, "E.g., ./Main --read  file.txt /path/to/file.txt\n");
}

void write_into_filesystem(char *path, char *filename){
  FILE *fptr = fopen(path, "r");
  struct stat sb;
  fstat(fileno(fptr), &sb);

  char *contents = (char *) malloc(sb.st_size);
  fread(contents, 1, sb.st_size, fptr);

  FCB *fcb = fileSystem.fs_open(filename);
  fileSystem.fs_write(fcb, (uint8_t*) contents, sb.st_size);
  fileSystem.fs_close(fcb);

  free(contents);
}

void read_from_filesystem(char *filename, char *path){
  FCB *fcb = fileSystem.fs_open(filename);
  if(fcb == NULL){
    fprintf(stderr, "ERROR: no such file %s\n", filename);
    return;
  }

  char *contents = (char *) malloc(fcb->size);
  fileSystem.fs_seek(fcb, 0);
  fileSystem.fs_read(fcb, (uint8_t *) contents, fcb->size);
  fileSystem.fs_close(fcb);

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
    if(strcmp(argv[1], "--write") == 0){
      write_into_filesystem(argv[2], argv[3]);
    } else if(strcmp(argv[1], "--safe_write") == 0){
      fileSystem.set_safe_write(true, "random_forest.pkl");
      write_into_filesystem(argv[2], argv[3]);
    } else if(strcmp(argv[1], "--read") == 0){
      read_from_filesystem(argv[2], argv[3]);
    } else {
      fprintf(stderr, "invalid cmd\n");
      exit(1);
    }
  }
}



