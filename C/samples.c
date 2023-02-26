#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>

#define MAX     256
#define BASE    0x40300000  // Proc address base
#define regOut  0x60        // Offset for reading binary input
#define regIn   0x64        // Offset for writing binary output

void Out32(void *adr, int offset, int value) {
    *((uint32_t *)(adr+offset)) = value;
}

int In32(void *adr, int offset) {
    return *((uint32_t *)(adr+offset));
}

int In16(void *adr, int offset) {
    int r;
    r= *((uint32_t *)(adr+offset));
    if (r > 32767) return r-65536;
    return r;
}

/* The main function */
int main(int argc, char **argv) {
  int fd;
  void *adr;
  char *name = "/dev/mem";
  int i,d;
  
  /* open memory device */
  if((fd = open(name, O_RDWR)) < 0) {
    perror("open");
    return 1;
  }

  /* map the memory, start from BASE address, size: _SC_PAGESIZE = 4k */
  adr = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE, MAP_SHARED, fd, BASE);


  /* ADD YOUR CODE HERE */
  FILE *fpt = fopen("receive.csv", "w+");


  /* enable scope, wait 1s and read & print samples */ /* SAMPLE CODE */
  Out32(adr, 0x50, 1);
  
  sleep(1);

  fprintf(fpt, "x, y\n");
  for (i = 0; i < MAX; i++) {
	  d = In16(adr, 0x60);
	  fprintf(fpt, "%d, %d\n",i,d);
  }

  munmap(adr, sysconf(_SC_PAGESIZE));
  return 0;
}
