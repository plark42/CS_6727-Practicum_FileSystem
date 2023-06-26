#include <cstdio>
#include <cstdlib>
#include <unistd.h>

//parent: writes to write end of pipe, reads 
//child : reads from read end, writes to write end

int main(){

  int parent_write_pipe[2];
  int child_write_pipe[2];
  
  pipe(parent_write_pipe);
  pipe(child_write_pipe);

  switch(fork()){
    case 0:
      dup2(child_write_pipe[1], 1);   //writes to child pipe
      dup2(parent_write_pipe[0], 0);  // reads from parent's pipe
      
      //close unused pipes
      close(child_write_pipe[0]);
      close(parent_write_pipe[1]);

      //run model
      execlp("python3", "python3", "predict.py", NULL);
      fprintf(stderr, "ERROR: predict.py failed\n");
      
    default:
      //parent reads from child's pipe
      FILE *f_in = fdopen(child_write_pipe[0], "r");

      //parent writes to it's own pipe
      FILE *f_out = fdopen(parent_write_pipe[1], "w");

      //parent doesn't use..
      close(child_write_pipe[1]);
      close(parent_write_pipe[0]);

      //wait for child
      char rdy;
      printf("WAITING ON CHILD\n");
      fscanf(f_in, "%c", &rdy);
      printf("DONE WAITING\n");

      uint8_t data[4096];
      FILE *fptr = fopen("enc.bin", "r");
      fread(data, 1, 4096, fptr);
      fclose(fptr);

      fwrite(data, 1, 4096, f_out);
      fclose(f_out);

      //read response from pipe
      char resp[100];
      fscanf(f_in, "%s", resp);
      printf("%s\n", resp);

  }

}
