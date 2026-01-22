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
//  1. custom3 sm3p0: Implements the P0 transformation function as used in the SM3 hash function
//     .insn r opcode, func3, func7, rd, rs1, rs2
//  2. custom3 sm3p1: Implements the P1 transformation function as used in the SM3 hash function
//     .insn r opcode, func3, func7, rd, rs1, rs2
////////////////////////////////////////////////////////////

// custom3 sm3p0
unsigned int custom3_sm3p0(unsigned int X)
{
    Uns32 code = 0x7b710000;
    Uns32 rs1  = X;
    Uns32 rs2  = 0x00000000;
    Uns32 P0;
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
    P0 = readReg32(addr_y_out_0, mode);

    return P0;
}

// custom3 sm3p1
unsigned int custom3_sm3p1(unsigned int X)
{
    Uns32 code = 0x7b720000;
    Uns32 rs1  = X;
    Uns32 rs2  = 0x00000000;
    Uns32 P1;
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
    P1 = readReg32(addr_y_out_0, mode);

    return P1;
}

#define SM3_Rotl32(buf, n) (((buf)<<n)|((buf)>>(32-n)))

unsigned int IV[8] = {
    0x7380166F,
    0x4914B2B9,
    0x172442D7,
    0xDA8A0600,
    0xA96F30BC,
    0x163138AA,
    0xE38DEE4D,
    0xB0FB0E4E
};

unsigned int T[64] = { 0 };
void T_init() {
    int j;

    for(j = 0; j < 16; j++)
        T[j] = 0x79CC4519;

    for(j = 16; j < 64; j++)
        T[j] = 0x7A879D8A;

    return;
}

unsigned int FF(unsigned int X, unsigned int Y, unsigned int Z, unsigned int j) {
    if(j>=0 && j<=15)
        return (X ^ Y ^ Z);
    else if(j>=16 && j<=63)
        return ((X & Y) | (X & Z) | (Y & Z));

    return 0;
}

unsigned int GG(unsigned int X, unsigned int Y, unsigned int Z, unsigned int j) {
    if(j>=0 && j<=15)
        return (X ^ Y ^ Z);
    else if(j>=16 && j<=63)
        return ((X & Y) | ((~X) & Z));

    return 0;
}

unsigned int message_ext_nice(unsigned char* message_buf, unsigned int W68[], unsigned int W64[]) {
    int j = 0;

    for(j = 0; j < 16; j++) {
        W68[j] = ((unsigned int)message_buf[j * 4 + 0] << 24) & 0xFF000000
               | ((unsigned int)message_buf[j * 4 + 1] << 16) & 0x00FF0000
               | ((unsigned int)message_buf[j * 4 + 2] <<  8) & 0x0000FF00
               | ((unsigned int)message_buf[j * 4 + 3] <<  0) & 0x000000FF;
    }

    for(j = 16; j < 68; j++) {
        unsigned int W68_buf;
        unsigned int W68_temp;

        W68_buf = W68[j - 16] ^ W68[j - 9] ^ (SM3_Rotl32(W68[j - 3], 15));
        W68_temp = custom3_sm3p1(W68_buf);

        W68[j] = W68_temp ^ (SM3_Rotl32(W68[j - 13], 7)) ^ W68[j - 6];
    }

    for(j = 0; j < 64; j++) {
        W64[j] = W68[j] ^ W68[j + 4];
    }

    return 0;
}

typedef enum {
    A=0,
    B=1,
    C=2,
    D=3,
    E=4,
    F=5,
    G=6,
    H=7
} type;

unsigned int CF_nice(unsigned int V[], unsigned int W68[], unsigned int W64[]) {
    int j = 0;
    unsigned int SS1 = 0, SS2 = 0, TT1 = 0, TT2 = 0;
    unsigned int V_buf[8] = { 0 };

    for(j = 0; j < 8; j++) {
        V_buf[j] = V[j];
    }

    for(j = 0; j < 64; j++) {
        SS1 = SM3_Rotl32((SM3_Rotl32(V_buf[A], 12) + V_buf[E] + SM3_Rotl32(T[j], j % 32)), 7);
        SS2 = SS1 ^ (SM3_Rotl32(V_buf[A], 12));
        TT1 = FF(V_buf[A], V_buf[B], V_buf[C], j) + V_buf[D] + SS2 + W64[j];
        TT2 = GG(V_buf[E], V_buf[F], V_buf[G], j) + V_buf[H] + SS1 + W68[j];
        V_buf[D] = V_buf[C];
        V_buf[C] = SM3_Rotl32(V_buf[B], 9);
        V_buf[B] = V_buf[A];
        V_buf[A] = TT1;
        V_buf[H] = V_buf[G];
        V_buf[G] = SM3_Rotl32(V_buf[F], 19);
        V_buf[F] = V_buf[E];
        V_buf[E] = custom3_sm3p0(TT2);
    }

    for(j = 0; j < 8; j++) {
        V[j] = V_buf[j] ^ V[j];
    }

    return 0;
}

unsigned int SM3_nice(unsigned char* message, unsigned int message_len, unsigned int V[]) {
    int i = 0;
    unsigned int num = (message_len + 1 + 8 + 64) / 64;
    unsigned char* message_buf = (unsigned char*)malloc(num * 64);
    memset(message_buf, 0, num * 64);
    memcpy(message_buf, message, message_len);
    message_buf[message_len] = 0x80;

    for(i = 0; i < 8; i++) {
        message_buf[num * 64 - i - 1] = ((unsigned long long)(message_len * 8) >> (i * 8)) & 0xFF;
    }

    unsigned int W68[68] = { 0 };
    unsigned int W64[64] = { 0 };

    T_init();

    for(int i = 0; i < 8; i++) {
        V[i] = IV[i];
    }

    for(i = 0; i < num; i++){
        message_ext_nice(&message_buf[i * 64], W68, W64);
        CF_nice(V, W68, W64);
    }

    return 0;
}

unsigned int sm3_nice(unsigned char* message, unsigned int message_len) {
    unsigned int V[8] = { 0 };

    SM3_nice(message, message_len, V);

    // printf("SM3 complete! \n");

    // printf("Message:\n");
    // printf("%s\n", message);

    // printf("Hash:\n");
    // for(int i = 0; i < 8; i++) {
    //     printf("%08x ",V[i]);
    // }
    // printf("\n");

    return 0;
}