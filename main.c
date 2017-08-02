#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "8080disasm.h"

int main(void){
  FILE *file;
  file=fopen("invaders.bin", "rb");
  uint32_t filesize=0;
  printf("8080 disassembler\nRobson Couto 2017\n");
  if(file==NULL){
    printf("\nFile not found\n");
    return 0;
  }
  fseek(file, 0, SEEK_END);
  filesize=ftell(file);

  printf("File size:%d\n",filesize );
  uint8_t *buffer = (uint8_t *) malloc(filesize);
  fseek(file, 0, SEEK_SET);
  if (!fread(buffer, 1, filesize, file)==filesize){
    printf("Problem moving %d bytes to buffer\n",filesize );
  }

  uint32_t pc=0;
  while (pc<filesize) {
    pc+= disassemble(buffer,pc);
  }

}
