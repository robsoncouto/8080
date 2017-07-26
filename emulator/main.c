#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

/* This code in an exercice from
emulator101.com*/
typedef struct ConditionCodes {
  uint8_t    z:1;
  uint8_t    s:1;
  uint8_t    p:1;
  uint8_t    cy:1;
  uint8_t    ac:1;
  uint8_t    pad:3;
} ConditionCodes;

typedef struct State8080 {
  uint8_t    a;
  uint8_t    b;
  uint8_t    c;
  uint8_t    d;
  uint8_t    e;
  uint8_t    h;
  uint8_t    l;
  uint16_t    sp;
  uint16_t    pc;
  uint8_t     *memory;
  struct      ConditionCodes      cc;
  uint8_t     int_enable;
} State8080;
void UnimplementedInstruction(State8080* state)
 {
  //pc will have advanced one, so undo that
  printf ("Error: Unimplemented instruction\n");
  exit(1);
 }

int Emulate8080Op(State8080* state){
  unsigned char *opcode = &state->memory[state->pc];
  switch(*opcode){
      case 0x00: UnimplementedInstruction(state); break;
      case 0x01: UnimplementedInstruction(state); break;
      case 0x02: UnimplementedInstruction(state); break;
      case 0x03: UnimplementedInstruction(state); break;
      case 0x04: UnimplementedInstruction(state); break;
      /*....*/
      case 0xfe: UnimplementedInstruction(state); break;
      case 0xff: UnimplementedInstruction(state); break;
  }
  state->pc+=1;  //for the opcode
}

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
  //
  // uint32_t pc=0;
  // while (pc<filesize) {
  //   pc+= disassemble(buffer,pc);
  // }

}
