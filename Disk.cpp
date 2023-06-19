#include "Disk.h"

void Disk::write_block(unsigned int block, uint8_t *data){
  unsigned int index = block * BLOCK_SIZE;
  memcpy(&(disk[index]), data, BLOCK_SIZE);
  write_disk_to_file();
}

void Disk::read_block(unsigned int block, uint8_t *data){
  read_disk_from_file();
  unsigned int index = block * BLOCK_SIZE;
  memcpy(data, &(disk[index]), BLOCK_SIZE);


}

void Disk::reformat(){
  memset(disk, 0, DISK_SIZE);
}

void Disk::write_disk_to_file(){
  FILE *fptr = fopen(FILENAME, "w");
  fwrite(disk, 1, DISK_SIZE, fptr);
  fclose(fptr);

}

void Disk::read_disk_from_file(){
  FILE *fptr = fopen(FILENAME, "r");
  fread(disk, 1, DISK_SIZE, fptr);
  fclose(fptr);
}
