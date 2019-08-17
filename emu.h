#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ops.h"

#define SIGN   0x80
#define ZERO   0x40
#define AUX    0x10

#define PARITY 0x04
#define CARRY  0x01

#define SZAPC (SIGN | ZERO | AUX | PARITY | CARRY)
#define SZAP (SIGN | ZERO | AUX | PARITY)

// Operation Classes
#define ADD 0x80
#define SUB 0x90
#define DAD 0x09

typedef struct state {
	unsigned char A, B, C, D, E, H, L, F; 
	unsigned short PC, SP;
	unsigned char MEM[65536];
} STATE;

STATE *init();
int emulate();
unsigned short alu(unsigned char flags, unsigned char a, unsigned char b, unsigned char op);
