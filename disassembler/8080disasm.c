#include <stdio.h>
#include <inttypes.h>


//Based on http://www.pastraiser.com/cpu/i8080/i8080_opcodes.html

uint8_t disassemble(uint8_t *buffer, uint32_t pc){
  uint8_t instruction=buffer[pc];
  int inst_bytes=1;
  printf("%04x ", pc);
  printf("%02x ", buffer[pc]);
  switch (instruction) {
    case 0x00: printf("\tNOP"); break;
    case 0x01: printf("\tLXI B, #%02x%02x ",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0x02: printf("\tSTAX B"); break;
    case 0x03: printf("\tINX B"); break;
    case 0x04: printf("\tINR B"); break;
    case 0x05: printf("\tDCR B"); break;
    case 0x06: printf("\tMVI B, #%02x", buffer[pc+1]); inst_bytes=2; break;
    case 0x07: printf("\tRLC"); break;
    case 0x08: printf("\tNOP ;alternative"); break;
    case 0x09: printf("\tDAD B"); break;
    case 0x0A: printf("\tLDAX B"); break;
    case 0x0B: printf("\tDCX B"); break;
    case 0x0C: printf("\tINR C"); break;
    case 0x0D: printf("\tDCR C"); break;
    case 0x0E: printf("\tMVI C, #%02x",buffer[pc+1]); inst_bytes=2; break;
    case 0x0F: printf("\tRRC"); break;

    case 0x10: printf("\tNOP ;alternative"); break;
    case 0x11: printf("\tLXI D, #%02x%02x ",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0x12: printf("\tSTAX D"); break;
    case 0x13: printf("\tINX D"); break;
    case 0x14: printf("\tINR D"); break;
    case 0x15: printf("\tDCR D"); break;
    case 0x16: printf("\tMVI D, #%02x", buffer[pc+1]); inst_bytes=2; break;
    case 0x17: printf("\tRAL"); break;
    case 0x18: printf("\tNOP ;alternative"); break;
    case 0x19: printf("\tDAD D"); break;
    case 0x1A: printf("\tLDAX D"); break;
    case 0x1B: printf("\tDCX D"); break;
    case 0x1C: printf("\tINR E"); break;
    case 0x1D: printf("\tDCR E"); break;
    case 0x1E: printf("\tMVI E, #%02x",buffer[pc+1]); inst_bytes=2; break;
    case 0x1F: printf("\tRAR"); break;

    case 0x20: printf("\tNOP alternative"); break;
    case 0x21: printf("\tLXI H, #%02x%02x ",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0x22: printf("\tSHLP #%02x%02x ",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0x23: printf("\tINX H"); break;
    case 0x24: printf("\tINR H"); break;
    case 0x25: printf("\tDCR H"); break;
    case 0x26: printf("\tMVI H, #%02x", buffer[pc+1]); inst_bytes=2; break;
    case 0x27: printf("\tDAA"); break;
    case 0x28: printf("\tNOP ;alternative"); break;
    case 0x29: printf("\tDAD H"); break;
    case 0x2A: printf("\tLHLD , #%02x%02x ",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;///////////////////
    case 0x2B: printf("\tDCX H"); break;
    case 0x2C: printf("\tINR L"); break;
    case 0x2D: printf("\tDCR L"); break;
    case 0x2E: printf("\tMVI L, #%02x",buffer[pc+1]); inst_bytes=2; break;
    case 0x2F: printf("\tCMA"); break;

    case 0x30: printf("\tNOP ;alternative"); break;
    case 0x31: printf("\tLXI SP, #%02x%02x ",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0x32: printf("\tSTA #%02x%02x ",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0x33: printf("\tINX SP"); break;
    case 0x34: printf("\tINR M"); break;
    case 0x35: printf("\tDCR M"); break;
    case 0x36: printf("\tMVI M, #%02x", buffer[pc+1]); inst_bytes=2; break;
    case 0x37: printf("\tSTC"); break;
    case 0x38: printf("\tNOP ;alternative"); break;
    case 0x39: printf("\tDAD SP"); break;
    case 0x3A: printf("\tLDA #%02x%02x ",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0x3B: printf("\tDCX SP"); break;
    case 0x3C: printf("\tINR A"); break;
    case 0x3D: printf("\tDCR A"); break;
    case 0x3E: printf("\tMVI A, #%02x",buffer[pc+1]); inst_bytes=2; break;
    case 0x3F: printf("\tCMC"); break;

    case 0x40: printf("\tMOV B, B"); break;
    case 0x41: printf("\tMOV B, C"); break;
    case 0x42: printf("\tMOV B, D"); break;
    case 0x43: printf("\tMOV B, E"); break;
    case 0x44: printf("\tMOV B, H"); break;
    case 0x45: printf("\tMOV B, L"); break;
    case 0x46: printf("\tMOV B, M"); break;
    case 0x47: printf("\tMOV B, A"); break;
    case 0x48: printf("\tMOV C, B"); break;
    case 0x49: printf("\tMOV C, C"); break;
    case 0x4A: printf("\tMOV C, D"); break;
    case 0x4B: printf("\tMOV C, E"); break;
    case 0x4C: printf("\tMOV C, H"); break;
    case 0x4D: printf("\tMOV C, L"); break;
    case 0x4E: printf("\tMOV C, M"); break;
    case 0x4F: printf("\tMOV C, A"); break;

    case 0x50: printf("\tMOV D, B"); break;
    case 0x51: printf("\tMOV D, C"); break;
    case 0x52: printf("\tMOV D, D"); break;
    case 0x53: printf("\tMOV D, E"); break;
    case 0x54: printf("\tMOV D, H"); break;
    case 0x55: printf("\tMOV D, L"); break;
    case 0x56: printf("\tMOV D, M"); break;
    case 0x57: printf("\tMOV D, A"); break;
    case 0x58: printf("\tMOV E, B"); break;
    case 0x59: printf("\tMOV E, C"); break;
    case 0x5A: printf("\tMOV E, D"); break;
    case 0x5B: printf("\tMOV E, E"); break;
    case 0x5C: printf("\tMOV E, H"); break;
    case 0x5D: printf("\tMOV E, L"); break;
    case 0x5E: printf("\tMOV E, M"); break;
    case 0x5F: printf("\tMOV E, A"); break;

    case 0x60: printf("\tMOV H, B"); break;
    case 0x61: printf("\tMOV H, C"); break;
    case 0x62: printf("\tMOV H, D"); break;
    case 0x63: printf("\tMOV H, E"); break;
    case 0x64: printf("\tMOV H, H"); break;
    case 0x65: printf("\tMOV H, L"); break;
    case 0x66: printf("\tMOV H, M"); break;
    case 0x67: printf("\tMOV H, A"); break;
    case 0x68: printf("\tMOV L, B"); break;
    case 0x69: printf("\tMOV L, C"); break;
    case 0x6A: printf("\tMOV L, D"); break;
    case 0x6B: printf("\tMOV L, E"); break;
    case 0x6C: printf("\tMOV L, H"); break;
    case 0x6D: printf("\tMOV L, L"); break;
    case 0x6E: printf("\tMOV L, M"); break;
    case 0x6F: printf("\tMOV L, A"); break;

    case 0x70: printf("\tMOV M, B"); break;
    case 0x71: printf("\tMOV M, C"); break;
    case 0x72: printf("\tMOV M, D"); break;
    case 0x73: printf("\tMOV M, E"); break;
    case 0x74: printf("\tMOV M, H"); break;
    case 0x75: printf("\tMOV M, L"); break;
    case 0x76: printf("\tHLT"); break;
    case 0x77: printf("\tMOV M, A"); break;
    case 0x78: printf("\tMOV A, B"); break;
    case 0x79: printf("\tMOV A, C"); break;
    case 0x7A: printf("\tMOV A, D"); break;
    case 0x7B: printf("\tMOV A, E"); break;
    case 0x7C: printf("\tMOV A, H"); break;
    case 0x7D: printf("\tMOV A, L"); break;
    case 0x7E: printf("\tMOV A, M"); break;
    case 0x7F: printf("\tMOV A, A"); break;

    case 0x80: printf("\tADD B"); break;
    case 0x81: printf("\tADD C"); break;
    case 0x82: printf("\tADD D"); break;
    case 0x83: printf("\tADD E"); break;
    case 0x84: printf("\tADD H"); break;
    case 0x85: printf("\tADD L"); break;
    case 0x86: printf("\tADD M"); break;
    case 0x87: printf("\tADD A"); break;
    case 0x88: printf("\tADC B"); break;
    case 0x89: printf("\tADC C"); break;
    case 0x8A: printf("\tADC D"); break;
    case 0x8B: printf("\tADC E"); break;
    case 0x8C: printf("\tADC H"); break;
    case 0x8D: printf("\tADC L"); break;
    case 0x8E: printf("\tADC M"); break;
    case 0x8F: printf("\tADC A"); break;

    case 0x90: printf("\tSUB B"); break;
    case 0x91: printf("\tSUB C"); break;
    case 0x92: printf("\tSUB D"); break;
    case 0x93: printf("\tSUB E"); break;
    case 0x94: printf("\tSUB H"); break;
    case 0x95: printf("\tSUB L"); break;
    case 0x96: printf("\tSUB M"); break;
    case 0x97: printf("\tSUB A"); break;
    case 0x98: printf("\tSBB B"); break;
    case 0x99: printf("\tSBB C"); break;
    case 0x9A: printf("\tSBB D"); break;
    case 0x9B: printf("\tSBB E"); break;
    case 0x9C: printf("\tSBB H"); break;
    case 0x9D: printf("\tSBB L"); break;
    case 0x9E: printf("\tSBB M"); break;
    case 0x9F: printf("\tSBB A"); break;

    case 0xA0: printf("\tANA B"); break;
    case 0xA1: printf("\tANA C"); break;
    case 0xA2: printf("\tANA D"); break;
    case 0xA3: printf("\tANA E"); break;
    case 0xA4: printf("\tANA H"); break;
    case 0xA5: printf("\tANA L"); break;
    case 0xA6: printf("\tANA M"); break;
    case 0xA7: printf("\tANA A"); break;
    case 0xA8: printf("\tXRA B"); break;
    case 0xA9: printf("\tXRA C"); break;
    case 0xAA: printf("\tXRA D"); break;
    case 0xAB: printf("\tXRA E"); break;
    case 0xAC: printf("\tXRA H"); break;
    case 0xAD: printf("\tXRA L"); break;
    case 0xAE: printf("\tXRA M"); break;
    case 0xAF: printf("\tXRA A"); break;

    case 0xB0: printf("\tORA B"); break;
    case 0xB1: printf("\tORA C"); break;
    case 0xB2: printf("\tORA D"); break;
    case 0xB3: printf("\tORA E"); break;
    case 0xB4: printf("\tORA H"); break;
    case 0xB5: printf("\tORA L"); break;
    case 0xB6: printf("\tORA M"); break;
    case 0xB7: printf("\tORA A"); break;
    case 0xB8: printf("\tCMP B"); break;
    case 0xB9: printf("\tCMP C"); break;
    case 0xBA: printf("\tCMP D"); break;
    case 0xBB: printf("\tCMP E"); break;
    case 0xBC: printf("\tCMP H"); break;
    case 0xBD: printf("\tCMP L"); break;
    case 0xBE: printf("\tCMP M"); break;
    case 0xBF: printf("\tCMP A"); break;

    case 0xc0: printf("\tRNZ"); break;
    case 0xc1: printf("\tPOP B"); break;
    case 0xc2: printf("\tJNZ #%02x%02x",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xc3: printf("\tJP #%02x%02x",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xC4: printf("\tCNZ  #%02x%02x",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xC5: printf("\tPUSH B"); break;
    case 0xC6: printf("\tADI #%02x",buffer[pc+1]); inst_bytes=2; break;
    case 0xC7: printf("\tRST 0"); break;
    case 0xC8: printf("\tRZ"); break;
    case 0xC9: printf("\tRET"); break;
    case 0xCA: printf("\tJZ, #%02x%02x",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xCB: printf("\tJMP, #%02x%02x ;alternative",buffer[pc+2], buffer[pc+1]); inst_bytes=3;break;// BITS GO HERE
    case 0xCC: printf("\tCZ, #%02x%02x",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xCD: printf("\tCALL #%02x%02x",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xCE: printf("\tACI #%02x",buffer[pc+1]); inst_bytes=2; break;
    case 0xCF: printf("\tRST 1"); break;

    case 0xD0: printf("\tRNC"); break;
    case 0xD1: printf("\tPOP D"); break;
    case 0xD2: printf("\tJNC #%02x%02x",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xD3: printf("\tOUT #%02x",buffer[pc+1]); inst_bytes=2; break;
    case 0xD4: printf("\tCNC #%02x%02x",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xD5: printf("\tPUSH D"); break;
    case 0xD6: printf("\tSUI #%02x",buffer[pc+1]); inst_bytes=2; break;
    case 0xD7: printf("\tRST 2h"); break;
    case 0xD8: printf("\tRC"); break;
    case 0xD9: printf("\tRET ;alternative"); break;
    case 0xDA: printf("\tJC #%02x%02x",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xDB: printf("\tIN #%02x",buffer[pc+1]); inst_bytes=2; break;
    case 0xDC: printf("\tCC #%02x%02x",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xDD: printf("\tCALL #%02x%02x ;alternative",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xDE: printf("\tSBI #%02x",buffer[pc+1]); inst_bytes=2; break;
    case 0xDF: printf("\tRST 3"); break;

    case 0xE0: printf("\tRPO"); break;
    case 0xE1: printf("\tPOP H"); break;
    case 0xE2: printf("\tJPO #%02x%02x",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xE3: printf("\tXTHL"); break;
    case 0xE4: printf("\tCPO #%02x%02x",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xE5: printf("\tPUSH H"); break;
    case 0xE6: printf("\tANI #%02x",buffer[pc+1]); inst_bytes=2; break;
    case 0xE7: printf("\tRST 4"); break;
    case 0xE8: printf("\tRPE"); break;
    case 0xE9: printf("\tPCHL"); break;
    case 0xEA: printf("\tJPE #%02x%02x",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xEB: printf("\tXCHG"); break;
    case 0xEC: printf("\tCPE #%02x%02x",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xED: printf("\tCALL #%02x%02x ;alternative",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xEE: printf("\tXRI #%02x",buffer[pc+1]); inst_bytes=2; break;
    case 0xEF: printf("\tRST 5"); break;

    case 0xF0: printf("\tRP"); break;
    case 0xF1: printf("\tPOP PSW"); break;
    case 0xF2: printf("\tJP #%02x%02x",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xF3: printf("\tDI"); break;
    case 0xF4: printf("\tCP #%02x%02x",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xF5: printf("\tPUSH PSW"); break;
    case 0xF6: printf("\tORI #%02x",buffer[pc+1]); inst_bytes=2; break;
    case 0xF7: printf("\tRST 6"); break;
    case 0xF8: printf("\tRM"); break;
    case 0xF9: printf("\tSPHL"); break;
    case 0xFA: printf("\tJM #%02x%02x",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xFB: printf("\tEI");
    case 0xFC: printf("\tCM, #%02x%02x",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xFD: printf("\tCALL #%02x%02x ;alternative",buffer[pc+2], buffer[pc+1]); inst_bytes=3; break;
    case 0xFE: printf("\tCPI #%02x",buffer[pc+1]);inst_bytes=2; break;
    case 0xFF: printf("\tRST 7"); break;
  }
  printf("\n");
  return inst_bytes;

}

void hexdump(uint8_t* buffer, uint32_t buffersize){
  for (uint32_t i=0;i<buffersize;i++){
    if(i%8==0) printf("\n");
    printf("%02x ", buffer[i]);
  }
  printf("\n");
}
