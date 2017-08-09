#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "8080disasm.h"

int main(void){
  printf("8080 disassembler\nRobson Couto 2017\n");

  FILE *file;
  file=fopen("invaders.bin", "rb");
  uint32_t filesize=0;

  if(file==NULL){
    printf("\nFile not found\n");
    return 0;
  }
  //gets file size
  fseek(file, 0, SEEK_END);
  filesize=ftell(file);

  printf("File size:%d\n",filesize );
  //moves file to an allocated buffer
  uint8_t *buffer = (uint8_t *) malloc(filesize);
  fseek(file, 0, SEEK_SET);
  if (!fread(buffer, 1, filesize, file)==filesize){
    printf("Problem moving %d bytes to buffer\n",filesize );
  }

  uint32_t pc=0;
  uint8_t str[50];
  while (pc<filesize) {
    memset(str,0,50);
    pc+= disassemble(buffer,pc,str);
    printf("%s\n",str);
  }
  fclose(file);
}
