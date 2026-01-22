#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char  Uns8;
typedef unsigned short Uns16;
typedef unsigned int   Uns32;

//Mode
#define MOD_POLL    0x000000ff
#define MOD_READ    0x00000000

//Address
#define addr_x_in_0 0x50004000
#define addr_x_in_1 0x50004004
#define addr_x_in_2 0x50004008
#define addr_x_in_3 0x5000400c
#define addr_x_in_4 0x50004010

#define addr_y_out_0      0x50004014

#define addr_state        0x50004018

#define LE2BE(X) (((X) & 0x000000ff) << 24) | (((X) & 0x0000ff00) << 8) | (((X) & 0x00ff0000) >> 8) | (((X) & 0xff000000) >> 24)

//LE:little-endian BE:big-endian
typedef enum {LE=0, BE} Endian;

//Determine whether the CPU storage mode is big-endian or little-endian
Endian get_endian(){
  Uns16 data = 0x1122;
  Uns8 *p = (Uns8 *)&data;

  return (*p < *(p+1)) ? BE : LE;
}

void writeReg32(Uns32 addr, Uns32 data, Endian mode){
  if(mode == LE)
      *(volatile Uns32*)addr = data;
  else if(mode == BE)
      *(volatile Uns32*)addr = LE2BE(data);
  else
      return;
}

Uns32 readReg32(Uns32 addr, Endian mode){
  if(mode == LE)
      return *(volatile Uns32*)addr;
  else if(mode == BE)
      return LE2BE(*(volatile Uns32*)addr);
  else
      return 0;
}

////////////////////////////////////////////////////////////
// custom3:
// Supported format: only R type here
// Supported instr:
//  1. custom3 sm4ks: Accelerates the Key Schedule operation of the SM4 block cipher
//     .insn r opcode, func3, func7, rd, rs1, rs2
//  2. custom3 sm4ed: Accelerates the block encrypt/decrypt operation of the SM4 block cipher
//     .insn r opcode, func3, func7, rd, rs1, rs2
////////////////////////////////////////////////////////////

// custom3 sm4ks
unsigned int custom3_sm4ks(unsigned int K, unsigned int TK)
{
  Uns32 code = 0x7b730000;
  Uns32 rs1  = K;
  Uns32 rs2  = TK;
  Uns32 rk;
  Uns32 rdata;

  Endian mode = get_endian();
  writeReg32(addr_x_in_0, code, mode);
  while(rdata != 1) {
      rdata = readReg32(addr_state, mode);
  }
  writeReg32(addr_x_in_1, rs1, mode);
  while(rdata != 2) {
      rdata = readReg32(addr_state, mode);
  }
  writeReg32(addr_x_in_2, rs2, mode);
  while(rdata != 3) {
      rdata = readReg32(addr_state, mode);
  }
  writeReg32(addr_x_in_3, MOD_POLL, mode);
  while(rdata != 4) {
      rdata = readReg32(addr_state, mode);
  }
  writeReg32(addr_x_in_4, MOD_READ, mode);
  while(rdata != 5) {
      rdata = readReg32(addr_state, mode);
  }
  rk = readReg32(addr_y_out_0, mode);

  return rk;
}

// custom3 sm4ed
unsigned int custom3_sm4ed(unsigned int X, unsigned int TX)
{
  Uns32 code = 0x7b740000;
  Uns32 rs1  = X;
  Uns32 rs2  = TX;
  Uns32 Y;
  Uns32 rdata;

  Endian mode = get_endian();
  writeReg32(addr_x_in_0, code, mode);
  while(rdata != 1) {
      rdata = readReg32(addr_state, mode);
  }
  writeReg32(addr_x_in_1, rs1, mode);
  while(rdata != 2) {
      rdata = readReg32(addr_state, mode);
  }
  writeReg32(addr_x_in_2, rs2, mode);
  while(rdata != 3) {
      rdata = readReg32(addr_state, mode);
  }
  writeReg32(addr_x_in_3, MOD_POLL, mode);
  while(rdata != 4) {
      rdata = readReg32(addr_state, mode);
  }
  writeReg32(addr_x_in_4, MOD_READ, mode);
  while(rdata != 5) {
      rdata = readReg32(addr_state, mode);
  }
  Y = readReg32(addr_y_out_0, mode);

  return Y;
}

unsigned int SM4_FK[4] = {
  0xA3B1BAC6,
  0x56AA3350,
  0x677D9197,
  0xB27022DC
};

unsigned int SM4_CK[32] = {
  0x00070e15, 0x1c232a31, 0x383f464d, 0x545b6269,
  0x70777e85, 0x8c939aa1, 0xa8afb6bd, 0xc4cbd2d9,
  0xe0e7eef5, 0xfc030a11, 0x181f262d, 0x343b4249,
  0x50575e65, 0x6c737a81, 0x888f969d, 0xa4abb2b9,
  0xc0c7ced5, 0xdce3eaf1, 0xf8ff060d, 0x141b2229,
  0x30373e45, 0x4c535a61, 0x686f767d, 0x848b9299,
  0xa0a7aeb5, 0xbcc3cad1, 0xd8dfe6ed, 0xf4fb0209,
  0x10171e25, 0x2c333a41, 0x484f565d, 0x646b7279
};

unsigned int sm4_nice(unsigned int key[4], unsigned int cipher[4])
{
  volatile unsigned int i=0;
  volatile unsigned int K_temp;
  volatile unsigned int K[36]={0};
  volatile unsigned int X_temp;
  volatile unsigned int X[36]={0};
  volatile unsigned int Y_temp;
  volatile unsigned int Y[36]={0};
  volatile unsigned int rk[32]={0};

  for(i = 0; i < 4; i++) {
    K[i] = key[i] ^ SM4_FK[i];
  }

  for(i = 0; i < 32; i++) {
    K_temp = K[i + 1] ^ K[i + 2] ^ K[i + 3] ^ SM4_CK[i];
    K[i + 4] = custom3_sm4ks(K[i], K_temp);
    rk[i] = K[i + 4];
  }

  // printf("key extension complete! \n");

  for(i = 0; i < 4; i++) {
    X[i] = cipher[i];
  }

  for(i = 0; i < 32; i++) {
    X_temp = X[i + 1] ^ X[i + 2] ^ X[i + 3] ^ rk[i];
    X[i + 4] = custom3_sm4ed(X[i], X_temp);
  }

  // printf("encrypt complete! result: \n");

  // for(i = 0; i < 4; i++) {
  //   printf("%08x ", X[35-i]);
  //   if(i == 3)
  //     printf("\n");
  // }

  for(i = 0; i < 4; i++) {
    Y[i] = X[35-i];
  }

  for(i = 0; i < 32; i++) {
    Y_temp = Y[i + 1] ^ Y[i + 2] ^ Y[i + 3] ^ rk[31-i];
    Y[i + 4] = custom3_sm4ed(Y[i], Y_temp);
  }

  // printf("decrypt complete! result: \n");

  // for(i = 0; i < 4; i++) {
  //   printf("%08x ", Y[35-i]);
  //   if(i == 3)
  //     printf("\n");
  // }

  return 0;
}