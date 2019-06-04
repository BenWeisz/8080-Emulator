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
#define SZAP (SIGN | ZERO | AUX | PARITY | CARRY)

typedef struct state {
	unsigned char A, B, C, D, E, H, L, F; 
	unsigned short PC, SP;
	unsigned char MEM[65536];
	unsigned char IN[255];
	unsigned char OUT[255];
	unsigned char INTE;
} STATE;

STATE *state init();
int emulate(STATE *state);
void flag(STATE *state, unsigned char flags, unsigned char val);
