#include "FS.h"

void FS::close_file(FCB *fcb)
{
  if (VERBOSE) {
    Serial << "Closing file " << fcb->str << endl;
    Serial << "  -> writing FCB to page " << fcb_dir[fcb->dir_index] << endl;
  }

  //write FCB back to page according to FCB_dir
  eeprom.write_page( fcb_dir[ fcb->dir_index ], (byte *) fcb);

}

int FS::create_file(const char *str)
{
  int i;
  int next_fcb = -1;
  unsigned int page;
  FCB fcb;

  if (VERBOSE) Serial << "Creating file " << str << endl;

  //check to see if file exists..
  if ( find_file(str) != -1) {
    Serial << "create_file ERROR: file exists\n";
    return -1;
  }

  // find an empty spot in FCB dir
  for (i = 0; i < PAGE_SIZE; i += sizeof(unsigned int)) {
    if (fcb_dir[i] == 0) {
      next_fcb = i;
      break;
    }
  }

  //error check (no more space for FCBs)
  if ( next_fcb == -1) {
    Serial << "create_file ERROR: no space in FCB directory\n";
    return -1;
  }

  //find an empty page to put FCB structure
  page = find_empty_page();
  if (page == 0) {
    Serial << "create_file ERROR: no empty pages\n";
    return -1;
  }

  //create FCB structure, fill in defaults
  if (VERBOSE) Serial << "  -> FCB directory entry: " << next_fcb << endl;
  fcb.fptr = 0;
  fcb.dir_index = next_fcb;
  memset(fcb.ptr, 0, sizeof(unsigned int) * 16);
  strncpy(fcb.str, str, LEN);

  //put FCB's address in FCB directory
  if (VERBOSE) Serial << "  -> FCB allocated to page " << page << endl;
  fcb_dir[next_fcb] = page;

  if (VERBOSE) Serial << "  -> writing EEPROM page 1 (FCB directory)" << endl;
  eeprom.write_page(1, (byte *) fcb_dir);


  //write contents to EEPROM,
  if (VERBOSE) Serial << "  -> writing FCB to EEPROM page " << page << endl;
  eeprom.write_page(page, (byte *) &fcb);

  return 0;
}


int FS::delete_file(const char *str)
{
  FCB fcb;
  int i;
  int page;

  //get file's page
  if (VERBOSE) Serial << "Deleting file " << str << endl;

  page = find_file(str);
  if (VERBOSE) Serial << "  -> file's FCB found on page " << page << endl;

  if (page == -1) {
    Serial << "delete_file ERROR: file " << str << " does not exist\n";
    return -1;
  }

  //mark FCB's data block pages as free (1) in free_list
  //set FCB pointers to NULL
  eeprom.read_page(page, (byte *) &fcb);

  if (VERBOSE) Serial << "  -> deallocating data block pointers in FCB" << endl;
  for (i = 0; i < 16; i++) {
    if (fcb.ptr[i] != 0) {
      unmark_page(fcb.ptr[i]);
      fcb.ptr[i] = 0;
    }
  }

  // remove FCB entry in fcb_dir
  if (VERBOSE) Serial << "  -> removing FCB directory entry " << fcb.dir_index << endl;
  fcb_dir[ fcb.dir_index ] = 0;

  // add FCB page to free_list
  if (VERBOSE) Serial << "  -> adding FCB page " << page << " to free list" << endl;
  unmark_page(page);

  eeprom.write_page(0, free_list);
  eeprom.write_page(1, fcb_dir);

  return 0;
}

int FS::format()
{
  if (VERBOSE) Serial << "Formatting EEPROM" << endl;

  //set free list to all 1s
  if (VERBOSE) Serial << "  -> resetting free list bit vector (all 1s)" << endl;
  memset(free_list, 0xFF, PAGE_SIZE);

  //indicate that page 0 and 1 are occuppied:
  //  byte 0 = 1111 1100
  if (VERBOSE) Serial << "  -> marking page 0 (bit vector) & 1 (FCB dir) as occuppied" << endl;
  free_list[0] &= 0xFC;

  //put free list on page 0 of EEPROM
  if (VERBOSE) Serial << "  -> writing bit vector to EEPROM page 0" << endl;
  eeprom.write_page( 0, (byte *) free_list);

  //set fcb_directory to all 0s
  if (VERBOSE) Serial << "  -> resetting FCB directory (all 0s)" << endl;
  memset( fcb_dir, 0, PAGE_SIZE);

  //put FCB directory on page 1 of EEPROM
  if (VERBOSE) Serial << "  -> writing FCB directory to EEPROM page 1" << endl;
  eeprom.write_page( 1, (byte *) fcb_dir);

  return 0;
}

void FS::init()
{
  if (VERBOSE) {
    Serial << "Initializing file system" << endl;
    Serial << "  -> reading free list bit vector (page 0) into memory\n";
    Serial << "  -> reading FCB directory (page 1) into memory\n";
  }

  //read free list and FCB directory into memory
  eeprom.read_page( 0, free_list);
  eeprom.read_page( 1, fcb_dir);
}

//returns empty page # (between 0 and 512)
int FS::find_empty_page()
{
  if (VERBOSE) Serial << "Scanning free list for empty page\n";

  unsigned int i, j;
  byte b;

  //read current free list from memory
  eeprom.read_page(0, free_list);

  for (i = 0; i < PAGE_SIZE; i++) {
    b = free_list[i];

    for (j = 0; j < 8; j++) {
      if ( ((b >> j) & 0x01) == 1) {
        if (VERBOSE) Serial << "  -> page " << (i * 8) + j << " is empty\n";
        mark_page((i * 8) + j);
        return (i * 8) + j;
      }
    }
  }

  if (VERBOSE) Serial << "  -> no empty page found";
  return 0;
}

//marks a page on the free list as occuppied
void FS::mark_page(unsigned int page)
{
  if (VERBOSE) Serial << "Marking page " << page << " as occuppied\n";

  unsigned int index;
  unsigned int offset;

  index = page / 8;
  offset = page % 8;

  //set page bit to 0
  free_list[index] &= ~(bit_mask[offset]);

  //write free_list to EEPROM
  eeprom.write_page(0, (byte *) free_list);
}

void FS::unmark_page(unsigned int page)
{
  if (VERBOSE) Serial << "Marking page " << page << " as empty\n";

  unsigned int index;
  unsigned int offset;

  index = page / 8;
  offset = page % 8;

  //set page bit to 1
  free_list[index] |= bit_mask[offset];

  //write free_list to EEPROM
  eeprom.write_page(0, (byte *) free_list);

}

//returns page # of file's FCB (-1 otherwise)
int FS::find_file(const char *str)
{
  if (VERBOSE) Serial << "Finding file " << str << endl;

  int i;
  FCB fcb;

  //go through each entry in FCB directory
  if (VERBOSE) Serial << "  -> scanning FCB directory entries\n";

  for (i = 0; i < PAGE_SIZE; i += sizeof(unsigned int)) {

    //if entry in FCB directory points to something..
    if (fcb_dir[i] != 0) {

      //check the FCB for the given file name
      eeprom.read_page( fcb_dir[i], (byte *) &fcb);
      if (VERBOSE) Serial << "  -> FCB directory entry " << i << " name " << fcb.str << endl;

      if (strcmp( fcb.str, str) == 0) return fcb_dir[i];
    }
  }

  return -1;
}

//reset file offset pointer to 0
void FS::rewind_file(FCB *fcb)
{
  if (VERBOSE) Serial << "Rewinding FCB file pointer to 0\n";
  fcb->fptr = 0;
}

//print names of all files
void FS::list_files()
{
  if (VERBOSE) Serial << "Listing files" << endl;

  int i, j;
  FCB fcb;
  int s = 0;

  if (VERBOSE) Serial << "  -> scanning all FCB directory entries" << endl;
  for (i = 0; i < PAGE_SIZE; i += sizeof(unsigned int)) {
    if (fcb_dir[i] != 0) {
      eeprom.read_page( fcb_dir[i], (byte *) &fcb);
      
      s = 0;
      for(j = 0; j < 16; j++){
        if(fcb.ptr[j] != 0) s += PAGE_SIZE; 
      }
      
      Serial << "File: " << fcb.str << ", " << s << " bytes" << endl;      

    }
  }
}


int FS::open_file(const char *str, FCB *fcb)
{
  if (VERBOSE) Serial << "Opening file " << str << endl;

  int fcb_index;

  fcb_index = find_file(str);
  if (fcb_index == -1) {
    Serial << "open_file ERROR: file not found\n";
    return -1;
  }

  //return an FCB pointer to the page in memory
  if (VERBOSE) Serial << "  -> locating FCB for file" << endl;

  eeprom.read_page( fcb_index, (byte *) fcb);
  return 0;
}


int FS::read_file(FCB *fcb, void *buffer, unsigned int num)
{
  if (VERBOSE) Serial << "Reading file " << fcb->str << endl;

  int i, j;
  byte *buf = (byte *) buffer;

  //get file data block # and offset based on file pointer
  unsigned int fpage = fcb->fptr / PAGE_SIZE;
  unsigned int offset = fcb->fptr % PAGE_SIZE;

  //read page from disk
  if (fpage >= 16) {
    Serial << "read_file ERROR: file too big\n";
    return -1;
  }

  if (fcb->ptr[fpage] == 0) return -1;

  if (VERBOSE) Serial << "  -> reading page " << fpage << " into memory\n";
  eeprom.read_page( fcb->ptr[fpage], current_page);

  i = offset;
  for (j = 0; j < num; j++) {
    if (i >= PAGE_SIZE) {
      //grab "next" page
      if (VERBOSE) Serial << "  -> reading page " << fpage << " into memory\n";
      fpage++;
      if (fcb->ptr[fpage] == 0) return -1;
      eeprom.read_page(fcb->ptr[fpage], current_page);
      i = 0;
    } else {
      buf[j] = current_page[i];
      i++;
      fcb->fptr++;
      if (fcb->fptr > 16 * PAGE_SIZE) {
        Serial << "read_file ERROR: file too big\n";
        return -1;
      }
    }
  }

  return 0;
}

int FS::write_file(FCB *fcb, void* buffer, unsigned int num)
{
  if (VERBOSE) Serial << "Writing file " << fcb->str << endl;

  int i, j;
  int new_page;
  byte *buf = (byte *) buffer;

  //read in current page of memory
  unsigned int fpage = fcb->fptr / PAGE_SIZE;
  unsigned int offset = fcb->fptr % PAGE_SIZE;

  if (fpage >= 16) {
    Serial << "write_file ERROR: file too big\n";
    return -1;
  }

  //check to see if current data block has a page
  if (fcb->ptr[fpage] == 0) {
    new_page = find_empty_page();
    if (VERBOSE) Serial << "  -> allocating empty EEPROM page " << new_page << " for FCB data block " << fpage << endl;

    if (new_page == -1) {
      Serial << "write_file ERROR: could not get new page\n";
      return -1;
    }

    fcb->ptr[fpage] = new_page;
  } else {
    if (VERBOSE) Serial << "  -> reading page " << fcb->ptr[fpage] << " into memory for writing\n";
    eeprom.read_page( fcb->ptr[fpage], current_page);
  }

  //starting at current offset, write buffer into page
  i = offset;

  //go through all bytes in buffer
  for (j = 0; j < num; j++) {

    if (i >= PAGE_SIZE) {
      //write current page to memory
      if (VERBOSE) Serial << "  -> writing FCB data block " << fpage << " to EEPROM page " << fcb->ptr[fpage] << endl;
      eeprom.write_page( fcb->ptr[fpage], (byte *) current_page);

      //get new page
      new_page = find_empty_page();
      if (VERBOSE) Serial << "  -> allocating empty EEPROM page " << new_page << " for FCB data block " << fpage + 1 << endl;

      if (new_page == -1) {
        Serial << "write_file ERROR: no empty pages\n";
        return -1;
      }

      //update FCB
      if (VERBOSE) Serial << "  -> updating FCB data block pointers\n";
      fpage++;
      if (fpage >= 16) {
        Serial << "write_file ERROR: file too large\n";
        return -1;
      }
      fcb->ptr[fpage] = new_page;
      i = 0;
    } else {

      current_page[i] = (byte) buf[j];
      i++;
      fcb->fptr++;
      if (fcb->fptr > PAGE_SIZE * 16) {
        Serial << "write_file ERROR: file too big\n";
        return -1;
      }
    }
  }

  //write current page out to memory
  if(VERBOSE) Serial << "  -> writing FCB data block " << fpage << " to EEPROM page " << fcb->ptr[fpage] << endl;
  eeprom.write_page( fcb->ptr[fpage], (byte *) current_page);

  return 0;
}


