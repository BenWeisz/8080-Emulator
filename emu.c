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
			state->D = state->B
			break;
		case MOVHB:
			state->H = state->B;
			break;
		case MOVMB:
			state->M = 0xFF00 | state->B;
			break;
		case ADDB:
			unsigned int c = (state->A + state->B >> 8 > 0);
			if (c)	state->F |= CARRY;
			break;
		
	}

	return 0;
}
