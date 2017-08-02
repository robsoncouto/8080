#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#define ret() state->pc = \
(state->memory[state->sp+1]<<8)|(state->memory[state->sp])\
;state->sp=state->sp+2;

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

      case 0x90://
        result=state->a-state->b;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x91:
        result=state->a-state->c;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x92://
        result=state->a-state->d;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x93://
        result=state->a-state->e;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x94://
        result=state->a-state->h;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x95://
        result=state->a-state->l;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x96://
        result=state->a-state->memory[(state->h<<8)|state->l];
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x97://
        result=state->a-state->a;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x98://ADC
        result=state->a-state->b-state->cc.cy;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x99://
        result=state->a-state->c-state->cc.cy;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x9a://
        result=state->a-state->d-state->cc.cy;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x9b://
        result=state->a-state->e-state->cc.cy;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x9c://
        result=state->a-state->h-state->cc.cy;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x9d://
        result=state->a-state->l-state->cc.cy;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x9e://
        result=state->a-state->memory[(state->h<<8)|state->l]-state->cc.cy;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0x9f://
        result=state->a-state->a-state->cc.cy;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;

      case 0xa0://
        result=state->a&state->b;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xa1:
        result=state->a&state->c;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xa2://
        result=state->a&state->d;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xa3://
        result=state->a&state->e;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xa4://
        result=state->a&state->h;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xa5://
        result=state->a&state->l;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xa6://
        result=state->a&state->memory[(state->h<<8)|state->l];
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xa7://
        result=state->a&state->a;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xa8://
        result=state->a^state->b;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xa9:
        result=state->a^state->c;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xaa://
        result=state->a^state->d;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xab://
        result=state->a^state->e;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xac://
        result=state->a^state->h;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xad://
        result=state->a^state->l;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xae://
        result=state->a^state->memory[(state->h<<8)|state->l];
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xaf://
        result=state->a^state->a;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;

      case 0xb0://
        result=state->a|state->b;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xb1:
        result=state->a|state->c;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xb2://
        result=state->a|state->d;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xb3://
        result=state->a|state->e;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xb4://
        result=state->a|state->h;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xb5://
        result=state->a|state->l;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xb6://
        result=state->a|state->memory[(state->h<<8)|state->l];
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xb7://
        result=state->a|state->a;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xb8://
        result=state->a-state->b;
        //state->a=(uint8_t)result;
        state->cc.z = (((uint8_t)result & 0xff) == 0);
        state->cc.s = (((uint8_t)result & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~((uint8_t)result);
        //state->cc.ac =FIXME
        break;
      case 0xb9:
        result=state->a-state->c;
        state->cc.z = (((uint8_t)result & 0xff) == 0);
        state->cc.s = (((uint8_t)result & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~((uint8_t)result);
        break;
      case 0xba://
        result=state->a-state->d;
        state->cc.z = (((uint8_t)result & 0xff) == 0);
        state->cc.s = (((uint8_t)result & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~((uint8_t)result);
        break;
      case 0xbb://
        result=state->a-state->e;
        state->cc.z = (((uint8_t)result & 0xff) == 0);
        state->cc.s = (((uint8_t)result & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~((uint8_t)result);
        break;
      case 0xbc://
        result=state->a-state->h;
        state->cc.z = (((uint8_t)result & 0xff) == 0);
        state->cc.s = (((uint8_t)result & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~((uint8_t)result);
        //state->cc.ac =FIXME
        break;
      case 0xbd://
        result=state->a-state->l;
        state->cc.z = (((uint8_t)result & 0xff) == 0);
        state->cc.s = (((uint8_t)result & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~((uint8_t)result);
        //state->cc.ac =FIXME
        break;
      case 0xbe://
        result=state->a-state->memory[(state->h<<8)|state->l];
        state->cc.z = (((uint8_t)result & 0xff) == 0);
        state->cc.s = (((uint8_t)result & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~((uint8_t)result);
        //state->cc.ac =FIXME
        break;
      case 0xbf://
        result=state->a-state->a;
        state->cc.z = (((uint8_t)result & 0xff) == 0);
        state->cc.s = (((uint8_t)result & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~((uint8_t)result);
        //state->cc.ac =FIXME
        break;
      case 0xc0://RNZ
        if(!state->cc.z) ret();
        break;
      case 0xc1://POP B
        state->c=state->memory[state->sp];
        state->b=state->memory[state->sp+1];
        state->sp=state->sp+2;
        break;
      case 0xc2://JNZ ADR
        if(!state->cc.z) state->pc=(opcode[2]<<8)|opcode[1];
        break;
      case 0xc3://JMP ADR
        state->pc=(opcode[2]<<8)|opcode[1];
        break;
      case 0xc4://CNZ ADR
        if(!state->cc.z){
          //fcall 0
          state->memory[state->sp-1]=state->pc>>8;
          state->memory[state->sp-2]=state->pc&0xff;
          state->sp=state->sp+2;
          state->pc=(opcode[2]<<8)|opcode[1];
        }
        break;
      case 0xc5://PUSH B
        state->memory[state->sp-2]=state->c;
        state->memory[state->sp-1]=state->b;
        state->sp=state->sp-2;
        break;
      case 0xc6://ADI D8
        result=state->a+opcode[1];
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xc7://RST 0
        //function call:
        state->memory[state->sp-1]=state->pc>>8;
        state->memory[state->sp-2]=state->pc&0xff;
        state->sp=state->sp+2;
        state->pc=0;
        break;
      case 0xc8://RZ
        if(state->cc.z) ret();
        break;
      case 0xc9://RET
        ret();
        break;
      case 0xca://JZ ADR
        if(state->cc.z==0) state->pc=(opcode[2]<<8)|opcode[1];
        break;
      case 0xcb://Undefined
        break;
      case 0xcc://CZ ADR
        if(state->cc.z){
          //fcall
          state->memory[state->sp-1]=state->pc>>8;
          state->memory[state->sp-2]=state->pc&0xff;
          state->sp=state->sp+2;
          state->pc=(opcode[2]<<8)|opcode[1];
        }
        break;
      case 0xcd://CALL ADR
        //fcall
        state->memory[state->sp-1]=state->pc>>8;
        state->memory[state->sp-2]=state->pc&0xff;
        state->sp=state->sp+2;
        state->pc=(opcode[2]<<8)|opcode[1];
        break;
      case 0xce://ACI D8
        result=state->a+opcode[1]+state->cc.cy;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xcf://RST 1 (08h)
        state->memory[state->sp-1]=state->pc>>8;
        state->memory[state->sp-2]=state->pc&0xff;
        state->sp=state->sp+2;
        state->pc=0x08;
        break;

      case 0xd0://RNC
        if(!state->cc.cy) ret();
        break;
      case 0xd1://POP D
        state->e=state->memory[state->sp];
        state->d=state->memory[state->sp+1];
        state->sp=state->sp+2;
        break;
      case 0xd2://JNC ADR
        if(!state->cc.cy) state->pc=(opcode[2]<<8)|opcode[1];
        break;
      case 0xd3://OUT D8 NOTE special instruction
        break;
      case 0xd4://CNC ADR
        if(!state->cc.cy){
          //fcall 0
          state->memory[state->sp-1]=state->pc>>8;
          state->memory[state->sp-2]=state->pc&0xff;
          state->sp=state->sp+2;
          state->pc=(opcode[2]<<8)|opcode[1];
        }
        break;
      case 0xd5://PUSH D
        state->memory[state->sp-2]=state->e;
        state->memory[state->sp-1]=state->d;
        state->sp=state->sp-2;
        break;
      case 0xd6://SUI D8
        result=state->a-opcode[1];
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xd7://RST 2 (0x10)
        //function call:
        state->memory[state->sp-1]=state->pc>>8;
        state->memory[state->sp-2]=state->pc&0xff;
        state->sp=state->sp+2;
        state->pc=0x10;
        break;
      case 0xd8://RC
        if(state->cc.cy) ret();
        break;
      case 0xd9://undefined
        break;
      case 0xda://JC ADR
        if(state->cc.cy==0) state->pc=(opcode[2]<<8)|opcode[1];
        break;
      case 0xdb://IN D8 NOTE special instruction
        break;
      case 0xdc://CC ADR
        if(state->cc.cy){
          //fcall
          state->memory[state->sp-1]=state->pc>>8;
          state->memory[state->sp-2]=state->pc&0xff;
          state->sp=state->sp+2;
          state->pc=(opcode[2]<<8)|opcode[1];
        }
        break;
      case 0xdd://undefined
        break;
      case 0xde://SBI D8
        result=state->a-opcode[1]-state->cc.cy;
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xdf://RST 3 (18h)
        state->memory[state->sp-1]=state->pc>>8;
        state->memory[state->sp-2]=state->pc&0xff;
        state->sp=state->sp+2;
        state->pc=0x18;
        break;

      case 0xe0://RPO
        if(!state->cc.p) ret();
        break;
      case 0xe1://POP H
        state->l=state->memory[state->sp];
        state->h=state->memory[state->sp+1];
        state->sp=state->sp+2;
        break;
      case 0xe2://JPO ADR
        if(!state->cc.p) state->pc=(opcode[2]<<8)|opcode[1];
        break;
      case 0xe3://XTHL
        state->l=state->memory[state->sp];
        state->h=state->memory[state->sp+1];
        break;
      case 0xe4://CPO ADR
        if(!state->cc.p){
          //fcall 0
          state->memory[state->sp-1]=state->pc>>8;
          state->memory[state->sp-2]=state->pc&0xff;
          state->sp=state->sp+2;
          state->pc=(opcode[2]<<8)|opcode[1];
        }
        break;
      case 0xe5://PUSH H
        state->memory[state->sp-2]=state->l;
        state->memory[state->sp-1]=state->h;
        state->sp=state->sp-2;
        break;
      case 0xe6://ANI D8
        result=state->a&opcode[1];
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xe7://RST 4 (0x20)
        //function call:
        state->memory[state->sp-1]=state->pc>>8;
        state->memory[state->sp-2]=state->pc&0xff;
        state->sp=state->sp+2;
        state->pc=0x20;
        break;
      case 0xe8://RPE
        if(state->cc.p) ret();
        break;
      case 0xe9://PCHL
        state->pc=(state->h<<8)|(state->l);
        break;
      case 0xea://JPE ADR
        if(state->cc.p) state->pc=(opcode[2]<<8)|opcode[1];
        break;
      case 0xeb://XCHG exchanges H<->D and L<->E
        state->h=state->h+state->d;
        state->d=state->h-state->d;
        state->h=state->h-state->d;

        state->l=state->l+state->e;
        state->e=state->l-state->e;
        state->l=state->l-state->e;
        break;
      case 0xec://CPE ADR
        if(state->cc.p){
          //fcall
          state->memory[state->sp-1]=state->pc>>8;
          state->memory[state->sp-2]=state->pc&0xff;
          state->sp=state->sp+2;
          state->pc=(opcode[2]<<8)|opcode[1];
        }
        break;
      case 0xed://undefined
        break;
      case 0xee://XRI D8
        result=state->a^opcode[1];
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xef://RST 5 (28h)
        state->memory[state->sp-1]=state->pc>>8;
        state->memory[state->sp-2]=state->pc&0xff;
        state->sp=state->sp+2;
        state->pc=0x28;
        break;


      case 0xf0://RP P=positive
        if(!state->cc.s) ret();
        break;
      case 0xf1://POP PSW FIXME
        // state->l=state->memory[state->sp];
        // state->h=state->memory[state->sp+1];
        // state->sp=state->sp+2;
        break;
      case 0xf2://JP ADR
        if(!state->cc.s) state->pc=(opcode[2]<<8)|opcode[1];
        break;
      case 0xf3://DI NOTE SPECIAL
        break;
      case 0xf4://CP ADR
        if(!state->cc.s){
          //fcall 0
          state->memory[state->sp-1]=state->pc>>8;
          state->memory[state->sp-2]=state->pc&0xff;
          state->sp=state->sp+2;
          state->pc=(opcode[2]<<8)|opcode[1];
        }
        break;
      case 0xf5://PUSH PSW FIXME
        // state->memory[state->sp-2]=state->l;
        // state->memory[state->sp-1]=state->h;
        // state->sp=state->sp-2;
        break;
      case 0xf6://OEI D8
        result=state->a|opcode[1];
        state->a=(uint8_t)result;
        state->cc.z = ((state->a & 0xff) == 0);
        state->cc.s = ((state->a & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~(state->a);
        //state->cc.ac =FIXME
        break;
      case 0xf7://RST 6 (0x30)
        //function call:
        state->memory[state->sp-1]=state->pc>>8;
        state->memory[state->sp-2]=state->pc&0xff;
        state->sp=state->sp+2;
        state->pc=0x30;
        break;
      case 0xf8://RM
        if(state->cc.s) ret();
        break;
      case 0xf9://SPHL
        state->sp=(state->h<<8)|(state->l);
        break;
      case 0xfa://JM ADR
        if(state->cc.s) state->pc=(opcode[2]<<8)|opcode[1];
        break;
      case 0xfb://EI NOTE special
        state->h=state->h+state->d;
        state->d=state->h-state->d;
        state->h=state->h-state->d;

        state->l=state->l+state->e;
        state->e=state->l-state->e;
        state->l=state->l-state->e;
        break;
      case 0xfc://CM ADR
        if(state->cc.s){
          //fcall
          state->memory[state->sp-1]=state->pc>>8;
          state->memory[state->sp-2]=state->pc&0xff;
          state->sp=state->sp+2;
          state->pc=(opcode[2]<<8)|opcode[1];
        }
        break;
      case 0xfd://undefined
        break;
      case 0xfe://CPI D8 FIXME
        result=state->a-opcode[1];
        state->cc.z = (((uint8_t)result & 0xff) == 0);
        state->cc.s = (((uint8_t)result & 0x80) != 0);
        state->cc.cy = (result > 0xff);
        state->cc.p = ~((uint8_t)result);
        //state->cc.ac =FIXME
        break;
      case 0xff://RST 7 (38h)
        state->memory[state->sp-1]=state->pc>>8;
        state->memory[state->sp-2]=state->pc&0xff;
        state->sp=state->sp+2;
        state->pc=0x38;
        break;

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
