#include "emu.h"

int main(int argc, char **argv){
	if (argc != 2){
		printf("Usage: ./emu <binary file>\n");	
		exit(1);
	}	

	char *ext = strstr(argv[1], ".b");
	if (ext == NULL){
		printf("The program arguement must be a binary file!\n");
		exit(1);	
	}

	return 0;
}

int emulate(STATE *state){
	unsigned char *op = &(state->MEM[state->PC]);
	
	switch(*op){
		case NOP00: break;	
		case NOP10: break;	
		case NOP20: break;	
		case NOP30: break;
	 	case MOVBB: break;
		case MOVDB:
			state->D = state->B;
			break;
		case MOVHB:
			state->H = state->B;
			break;
		case MOVMB:
			state->M = 0x0000 | state->B;
			break;
		case ADDB:
			flag(state, ADDB, state->A, state->B);
			state->A += state->B;
			break;
	}

	return 0;
}

void flag(STATE *state, unsigned char op, unsigned char A, unsigned B){
	// Operation
	unsigned int temp;
	if (op == ADDB)
		temp = (A + B);

	// Sign Bit
	if (temp >> 7)
		state->F |= SIGN;

	// Zero Bit
	if (temp == 0x00)
		state->F |= ZERO;

	// Aux Bit
	if ((temp & 0x10) >> 4)
		state->F |= AUX;

	// Parity Bit
	unsigned int p = temp ^ (temp >> 1);
	p ^= temp >> 2;
	p ^= temp >> 3;
	p ^= temp >> 4;
	p ^= temp >> 5;
	p ^= temp >> 6;
	p ^= temp >> 7;
	if (p)
		state->F |= PARITY;

	// Carry Bit
	if ((temp >> 8 > 0))
		state->F |= CARRY;
}
