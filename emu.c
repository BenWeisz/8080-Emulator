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
			state->A += state->B;
			break;
		case SUBB:
			state->A -= state->B;
			flag(state, SZAPC, state->A);
			break;
		case ANAB:
			state->A = state->A & state->B;
			flag(state, SZAPC, state->A);
			break;
		case ORAB:
			state->A = state->A | state->B;
			flag(state, SZAPC, state->A);
			break;
		case RNZ:
			if (!(state->F & ZERO)){
				unsigned short sp = state->SP;
				state->PC = ((state->MEM[sp + 1] << 8) | state->MEM[sp]);
				state->SP += 2;
			}
			break;
		case RNC:
			if (!(state->F & CARRY)){
				unsigned short sp = state->SP;
				state->PC = ((state->MEM[sp + 1] << 8) | state->MEM[sp]);
				state->SP += 2;
			}
			break;
		case RPO:
			if (!(state->F & PARITY)){
				unsigned short sp = state->SP;
				state->PC = ((state->MEM[sp + 1] << 8) | state->MEM[sp]);
				state->SP += 2;
			}
			break;
		case RP:
			if (!(state->F & SIGN)){
				unsigned short sp = state->SP;
				state->PC = ((state->MEM[sp + 1] << 8) | state->MEM[sp]);
				state->SP += 2;
			}
			break;
	}

	return 0;
}

void flag(STATE *state, unsigned char flags, unsigned char val){
	// Sign Bit
	if (val >> 7 && flags & SIGN)
		state->F |= SIGN;
	else if (flags & SIGN)
		state->F &= ~SIGN;

	// Zero Bit
	if (val == 0x00 && flags & ZERO)
		state->F |= ZERO;
	else if (flags & ZERO)
		state->F &= ~ZERO;

	// Aux Bit
	if ((val & 0x10) >> 4 && flags & AUX)
		state->F |= AUX;
	else if (flags & AUX)
		state->F &= ~AUX;

	// Parity Bit
	unsigned int p = val ^ (val >> 1);
	p ^= val >> 2;
	p ^= val >> 3;
	p ^= val >> 4;
	p ^= val >> 5;
	p ^= val >> 6;
	p ^= val >> 7;
	if (p && flags & PARITY)
		state->F |= PARITY;
	else if (flags & PARITY)
		state->F &= ~PARITY;

	// Carry Bit
	if ((val >> 8 > 0) && flags & CARRY)
		state->F |= CARRY;
	else if (flags & CARRY)
		state->F &= ~CARRY;
}
