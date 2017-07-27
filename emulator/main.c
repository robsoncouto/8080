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
  state->pc+=1;
  uint16_t result=0;
  switch(*opcode){
      case 0x00: //NOP
        break;
      case 0x01:// LXI B, D16 - 3
        state->c = opcode[1];
        state->b = opcode[2];
        state->pc += 2;
        break;
      case 0x02://STAX B
        state->memory[((state->b)<<8)|((state->c))]=state->a;
        break;
      case 0x03://INX B
        // result=state->c+1;
        // if(result>0xFF){
        //   state->c+=1;
        //   state->b+=1;
        // }else{
        //   state->c+=1;
        // }
        result=(state->b)<<8)|((state->c));
        result++;
        state->c=(uint8_t)result<<8;
        state->b=(uint8_t)result&0xFF;

        break;
      case 0x04://INR B
        // state->b+=1;
        // state->cc.z = ((state->b & 0xff) == 0);
        // state->cc.s = ((state->b & 0x80) != 0);
        // state->cc.cy = (answer > 0xff);
        // state->cc.p = Parity(answer&0xff);
        // break;
      /*....*/
      case 0x05: UnimplementedInstruction(state); break;
      case 0x06://MVI B, D8
        state->b=opcode[2];
        break;
      case 0x07:
        if(state->a&0x80){
          state->a=state->a<<1;
          state->a=state->a|0x01;
          state->cc.cy = 1;
        }else{
          state->a=state->a<<1;
          state->a=state->a&0xFD;
          state->cc.cy = 0;
        }
        break;
      case 0x08://illegal instruction
        break;
      case 0x09:// DAD B
        result=(state->b)<<8)|((state->c));
        result=(state->h)<<8)|((state->l));
        if(result<BC)state->cc.cy=1;//result will wrap aorund
        state->h=(uint8_t)result>>8;
        state->l=(uint8_t)result&0xFF;
        break;
      case 0x0A://LDAX B
        state->a=memory[((state->b)<<8)|((state->c))];
        break;
      case 0x0B://DCX B
        result=(state->b)<<8)|((state->c));
        result--;
        state->b=(uint8_t)result>>8;
        state->c=(uint8_t)result&0xFF;
        break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0x00: UnimplementedInstruction(state); break;
      case 0xff: UnimplementedInstruction(state); break;
  }
  //state->pc+=1;  //for the opcode
}

int main(void){
  State8080 curr_state;
  curr_state.pc=0;

  FILE *file;
  file=fopen("invaders.bin", "rb");
  uint32_t filesize=0;
  printf("8080 emulator\nRobson Couto 2017\n");
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
  curr_state.memory=buffer;
  Emulate8080Op(&curr_state);
}
