#include "Disk.h"

void Disk::write_block(unsigned int addr, uint8_t *data){
  
}

void Disk::read_block(unsigned int addr, uint8_t *data){

}

void Disk::reformat(){
  memset(disk, 0, DISK_SIZE);
}

void Disk::save_disk_to_file(){

}

void Disk::read_disk_from_file(){

}
