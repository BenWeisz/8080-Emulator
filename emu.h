#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ops.h"

#define SIGN   0x80
#define ZERO   0x40
#define AUX    0x10

#define PARITY 0x04
#define CARRY  0x01

typedef struct state {
	unsigned char A, B, C, D, E, H, L, M, F; 
	unsigned short PC, SP;
	unsigned char MEM[65536];
} STATE;

int emulate(STATE *state);
void flag(STATE *state, unsigned char op, unsigned char A, unsigned B);
