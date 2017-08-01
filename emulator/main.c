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
  uint32_t result=0;
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
        result=(state->b<<8)|(state->c);
        result++;
        state->c=(uint8_t)result>>8;
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
        result=(state->b<<8)|((state->c));
        result+=(state->h<<8)|((state->l));
        if(result>0xffff)state->cc.cy=1;
        state->h=(uint8_t)result>>8;
        state->l=(uint8_t)result&0xFF;
        break;
      case 0x0A://LDAX B
        state->a=state->memory[((state->b)<<8)|((state->c))];
        break;
      case 0x0B://DCX B
        result=(state->b<<8)|((state->c));
        result--;
        state->c=(uint8_t)result&0xFF;
        state->b=(uint8_t)result>>8;
        break;
      case 0x0C://INR C
        result=state->c++;
        state->c=(uint8_t)result;
        state->cc.z = ((state->c & 0xff) == 0);
        state->cc.s = ((state->c & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->c);
        break;
      case 0x0D://DCR C
        result=state->c--;
        state->c=(uint8_t)result;
        state->cc.z = ((state->c & 0xff) == 0);
        state->cc.s = ((state->c & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->c);
        break;
      case 0x0E://MVI C, D8
        state->c=opcode[1];
        break;
      case 0x0F://RRC
        result=state->a;
        state->a=state->a>>1;
        state->cc.cy = (result & 0x01);
        state->a=state->a|(state->cc.cy<<7);
        break;
      case 0x10://NOT DOCUMENTED
        break;
      case 0x11://LXI D,D16
        state->d=opcode[2];
        state->e=opcode[1];
        break;
      case 0x12://STAX D;
        state->memory[(state->d<<8)|(state->e&0xff)]=state->a;
        break;
      case 0x13://INX D
        result=(state->d<<8)|(state->e&0xff)+1;
        state->e=result&0xff;
        state->d=result>>8;
        break;
      case 0x14://INR D
        state->d++;
        state->cc.z=(state->d&0xff);
        state->cc.s=(state->d&0x10);
        state->cc.p=~(state->d&0x01);
        state->cc.ac=0;//FIXME
        break;
      case 0x15://DCR D
        state->d--;
        state->cc.z=(state->d&0xff);
        state->cc.s=(state->d&0x10);
        state->cc.p=~(state->d&0x01);
        state->cc.ac=0;//FIXME
        break;
      case 0x16://MVI D, D8
        state->d=opcode[1];
        break;
      case 0x17://RAL
        result=state->a;
        state->a=result<<1;
        state->a=state->a|(state->cc.cy); //bit0 = prev cy
        state->cc.cy = (result & 0x0100); //cy = prev bit7
        break;
      case 0x18://NOT DOCUMENTED
        break;
      case 0x19://DAD D
        result=(state->d<<8)|((state->e));
        result+=(state->h<<8)|((state->l));
        if(result>0xffff)state->cc.cy=1;
        state->h=(uint8_t)result>>8;
        state->l=(uint8_t)result&0xFF;
        break;
      case 0x1a://LDAX D
        state->a=state->memory[(state->d<<8)|((state->e))];
        break;
      case 0x1b://DCX D
        result=(state->d<<8)|((state->e))-1;
        state->d=(uint8_t)result>>8;
        state->e=(uint8_t)result&0xFF;
        break;
      case 0x1c://INR E
        result=state->c++;
        state->e=(uint8_t)result;
        state->cc.z = ((state->e & 0xff) == 0);
        state->cc.s = ((state->e & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->e);
        break;
      case 0x1d://DCR E
        result=state->e--;
        state->e=(uint8_t)result;
        state->cc.z = ((state->e & 0xff) == 0);
        state->cc.s = ((state->e & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->e);
        break;
      case 0x1e://MVI E, D8
        state->e=opcode[1];
        break;
      case 0x1f://RAR
        result=state->a;
        state->a=state->a>>1;
        state->cc.cy = (result & 0x01);
        state->a=state->a|(result&(1<<7));
        break;


      case 0x20://RIM
        break;
      case 0x21://LXI H,D16
        state->h=opcode[2];
        state->l=opcode[1];
        break;
      case 0x22://STAX H;
        //FIXME
        break;
      case 0x23://INX H
        result=(state->h<<8)|(state->l&0xff)+1;
        state->h=result&0xff;
        state->l=result>>8;
        break;
      case 0x24://INR H
        state->h++;
        state->cc.z=(state->h&0xff);
        state->cc.s=(state->h&0x80);
        state->cc.p=~(state->h&0x01);
        state->cc.ac=0;//FIXME
        break;
      case 0x25://DCR H
        state->h--;
        state->cc.z=(state->h&0xff);
        state->cc.s=(state->h&0x80);
        state->cc.p=~(state->h&0x01);
        state->cc.ac=0;//FIXME
        break;
      case 0x26://MVI H, D8
        state->h=opcode[1];
        break;
      case 0x27://DAA
        break;
      case 0x28://NOT DOCUMENTED
        break;
      case 0x29://DAD H
        result=(state->h<<8)|(state->l);//TODO check if HL or HI
        result+=(state->h<<8)|(state->l);
        if(result>0xffff)state->cc.cy=1;
        state->h=(uint8_t)result>>8;
        state->l=(uint8_t)result&0xFF;
        break;
      case 0x2a://LDLD H
        state->h=state->memory[opcode[2]];
        state->l=state->memory[opcode[1]];
        break;
      case 0x2b://DCX H
        result=(state->d<<8)|((state->e))-1;
        state->d=(uint8_t)result>>8;
        state->e=(uint8_t)result&0xFF;
        break;
      case 0x2c://INR L
        result=state->l++;
        state->l=(uint8_t)result;
        state->cc.z = ((state->l & 0xff) == 0);
        state->cc.s = ((state->l & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->l);
        break;
      case 0x2d://DCR L
        result=state->l--;
        state->l=(uint8_t)result;
        state->cc.z = ((state->l & 0xff) == 0);
        state->cc.s = ((state->l & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->l);
        break;
      case 0x2e://MVI L, D8
        state->l=opcode[1];
        break;
      case 0x2f://CMA
        state->a=~state->a;
        break;

      case 0x30://SIM NOTE Special instruction
        break;
      case 0x31://LXI SP,D16
        state->sp=(opcode[2]<<8)|opcode[1];
        break;
      case 0x32://STA ADR;
        state->memory[(opcode[2]<<8)|opcode[1]]=state->a;
        break;
      case 0x33://INX SP
        state->sp++;
        break;
      case 0x34://INR M
        state->memory[(state->h<<8)|(state->l)]++;
        state->cc.z=(state->memory[(state->h<<8)|(state->l)]&0xff);
        state->cc.s=(state->memory[(state->h<<8)|(state->l)]&0x80);
        state->cc.p=~(state->memory[(state->h<<8)|(state->l)]&0x01);
        state->cc.ac=0;//FIXME
        break;
      case 0x35://DCR M
        state->memory[(state->h<<8)|(state->l)]--;
        state->cc.z=(state->memory[(state->h<<8)|(state->l)]&0xff);
        state->cc.s=(state->memory[(state->h<<8)|(state->l)]&0x80);
        state->cc.p=~(state->memory[(state->h<<8)|(state->l)]&0x01);
        state->cc.ac=0;//FIXME
        break;
      case 0x36://MVI H, D8
        state->memory[(state->h<<8)|(state->l)]=opcode[1];
        break;
      case 0x37://STC
        state->cc.cy=1;
        break;
      case 0x38://NOT DOCUMENTED
        break;
      case 0x39://DAD SP
        result=(state->h<<8)|((state->l));//TODO check if HL or HI
        result+=state->sp;
        if(result>0xffff)state->cc.cy=1;
        state->h=(uint8_t)result>>8;
        state->l=(uint8_t)result&0xFF;
        break;
      case 0x3a://LDA ADR
        state->a=state->memory[(opcode[2]<<8)|opcode[1]];
        break;
      case 0x3b://DCX SP
        state->sp++;
        break;
      case 0x3c://INR A
        result=state->a++;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        break;
      case 0x3d://DCR A
        result=state->a--;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        break;
      case 0x3e://MVI L, D8
        state->a=opcode[1];
        break;
      case 0x3f://CMC
        state->cc.cy=~state->cc.cy;
        break;

      case 0x40://
        state->b=state->b;
        break;
      case 0x41://
        state->b=state->c;
        break;
      case 0x42://
        state->b=state->d;
        break;
      case 0x43://
        state->b=state->e;
        break;
      case 0x44://
        state->b=state->h;
        break;
      case 0x45://
        state->b=state->l;
        break;
      case 0x46://
        state->b=state->memory[(state->h<<8)|(state->l)];
        break;
      case 0x47://
        state->b=state->a;
        break;
      case 0x48://
        state->c=state->b;
        break;
      case 0x49://
        state->c=state->c;
        break;
      case 0x4a://
        state->c=state->d;
        break;
      case 0x4b://
        state->c=state->e;
        break;
      case 0x4c://
        state->c=state->h;
        break;
      case 0x4d://
        state->c=state->l;
        break;
      case 0x4e://
        state->c=state->memory[(state->h<<8)|(state->l)];
        break;
      case 0x4f://
        state->c=state->a;
        break;

      case 0x50://
        state->d=state->b;
        break;
      case 0x51://
        state->d=state->c;
        break;
      case 0x52://
        state->d=state->d;
        break;
      case 0x53://
        state->d=state->e;
        break;
      case 0x54://
        state->d=state->h;
        break;
      case 0x55://
        state->d=state->l;
        break;
      case 0x56://
        state->d=state->memory[(state->h<<8)|(state->l)];
        break;
      case 0x57://
        state->d=state->a;
        break;
      case 0x58://
        state->e=state->b;
        break;
      case 0x59://
        state->e=state->c;
        break;
      case 0x5a://
        state->e=state->d;
        break;
      case 0x5b://
        state->e=state->e;
        break;
      case 0x5c://
        state->e=state->h;
        break;
      case 0x5d://
        state->e=state->l;
        break;
      case 0x5e://
        state->e=state->memory[(state->h<<8)|(state->l)];
        break;
      case 0x5f://
        state->e=state->a;
        break;

      case 0x60://
        state->h=state->b;
        break;
      case 0x61://
        state->h=state->c;
        break;
      case 0x62://
        state->h=state->d;
        break;
      case 0x63://
        state->h=state->e;
        break;
      case 0x64://
        state->h=state->h;
        break;
      case 0x65://
        state->h=state->l;
        break;
      case 0x66://
        state->h=state->memory[(state->h<<8)|(state->l)];
        break;
      case 0x67://
        state->h=state->a;
        break;
      case 0x68://
        state->l=state->b;
        break;
      case 0x69://
        state->l=state->c;
        break;
      case 0x6a://
        state->l=state->d;
        break;
      case 0x6b://
        state->l=state->e;
        break;
      case 0x6c://
        state->l=state->h;
        break;
      case 0x6d://
        state->l=state->l;
        break;
      case 0x6e://
        state->l=state->memory[(state->h<<8)|(state->l)];
        break;
      case 0x6f://
        state->l=state->a;
        break;

      case 0x70://
        state->memory[(state->h<<8)|(state->l)]=state->b;
        break;
      case 0x71://
        state->memory[(state->h<<8)|(state->l)]=state->c;
        break;
      case 0x72://
        state->memory[(state->h<<8)|(state->l)]=state->d;
        break;
      case 0x73://
        state->memory[(state->h<<8)|(state->l)]=state->e;
        break;
      case 0x74://
        state->memory[(state->h<<8)|(state->l)]=state->h;
        break;
      case 0x75://
        state->memory[(state->h<<8)|(state->l)]=state->l;
        break;
      case 0x76://HLT
        break;
      case 0x77://
        state->memory[(state->h<<8)|(state->l)]=state->a;
        break;
      case 0x78://
        state->a=state->b;
        break;
      case 0x79://
        state->a=state->c;
        break;
      case 0x7a://
        state->a=state->d;
        break;
      case 0x7b://
        state->a=state->e;
        break;
      case 0x7c://
        state->a=state->h;
        break;
      case 0x7d://
        state->a=state->l;
        break;
      case 0x7e://
        state->a=state->memory[(state->h<<8)|(state->l)];
        break;
      case 0x7f://
        state->a=state->a;
        break;

      case 0x80://
        result=state->a+state->b;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x81:
        result=state->a+state->c;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x82://
        result=state->a+state->d;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x83://
        result=state->a+state->e;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x84://
        result=state->a+state->h;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x85://
        result=state->a+state->l;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x86://
        result=state->a+state->memory[(state->h<<8)|state->l];
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x87://
        result=state->a+state->a;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x88://ADC
        result=state->a+state->b+state->cc.cy;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x89://
        result=state->a+state->c+state->cc.cy;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x8a://
        result=state->a+state->d+state->cc.cy;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x8b://
        result=state->a+state->e+state->cc.cy;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x8c://
        result=state->a+state->h+state->cc.cy;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x8d://
        result=state->a+state->l+state->cc.cy;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x8e://
        result=state->a+state->memory[(state->h<<8)|state->l]+state->cc.cy;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x8f://
        result=state->a+state->a+state->cc.cy;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;


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
