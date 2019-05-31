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
			state->H = 0x00;
			state->L = state->B;
			break;
		case ADDB:
			state->A += state->B;
			flag(state, SZAPC, state->A);
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
		case LXIB:
			state->B = state->MEM[state->PC + 2];
			state->C = state->MEM[state->PC + 1];
			return 3;
		case LXID:
			state->D = state->MEM[state->PC + 2];
			state->E = state->MEM[state->PC + 1];
			return 3;
		case LXIH:
			state->H = state->MEM[state->PC + 2];
			state->L = state->MEM[state->PC + 1];
			return 3;
		case LXISP:
			state->SP = state->MEM[state->PC + 2] << 8 | state->MEM[state->PC + 1];
			return 3;
		case MOVBC:
			state->B = state->C;
			break;
		case MOVDC:
			state->D = state->C;
			break;
		case MOVHC:
			state->H = state->C;
			break;
		case MOVMC:
			state->H = 0x00;
			state->L = state->C;
			break;
		case ADDC:
			state->A += state->C;
			flag(state, SZAPC, state->A);
			break;
		case SUBC:
			state->A -= state->C;
			flag(state, SZAPC, state->A);
			break;
		case ANAC:
			state->A = state->A & state->C;
			flag(state, SZAPC, state->A);
			break;
		case ORAC:
			state->A = state->A | state->C;
			flag(state, SZAPC, state->A);
			break;
		case POPB:
			state->B = state->MEM[state->SP + 1];
			state->C = state->MEM[state->SP];
			state->SP += 2;
			break;
		case POPD:
			state->D = state->MEM[state->SP + 1];
			state->E = state->MEM[state->SP];
			state->SP += 2;
			break;
		case POPH:
			state->H = state->MEM[state->SP + 1];
			state->L = state->MEM[state->SP];
			state->SP += 2;
			break;
		case POPPSW:
			state->F = state->MEM[state->SP]; // TODO --> POSSIBLE ERROR
			state->SP += 2;
			flag(state, SZAPC, state->A);
			break;
		case STAXB:
			state->MEM[(state->B << 8) | state->C] = state->A;
			break;
		case STAXD:
			state->MEM[(state->D << 8) | state->C] = state->A;
			break;
		case SHLD:
			unsigned char lo = state->MEM[state->PC + 1];
			unsigned char hi = state->MEM[state->PC + 2];
			state->MEM[(hi << 8) | low] = state->L;
			state->MEM[((hi << 8) | low) + 1] = state->H;
			return 3;
		case STA:
			unsigned char lo = state->MEM[state->PC + 1];
			unsigned char hi = state->MEM[state->PC + 2];
			state->MEM[(hi << 8) | lo] = state->A;
			return 3;
		case MOVBD:
			state->B = state->D;
			break;
		case MOVDD:
			state->D = state->D;
			break;
		case MOVHD:
			state->H = state->D;
			break;
		case MOVMD:
			state->H = 0x00;
			state->L = state->D;
			break;	
		case ADDD:
			state->A += state->D;
			flag(state, SZAPC, state->A);
			break;
		case SUBD:
			state->A -= state->D;
			flag(state, SZAPC, state->A);
			break;
		case ANAD:
			state-> = state->A & state->D;
			flag(state, SZAPC, state->A);		
			break;
		case ORAD:
			state->A = state->A | state->D;
			flag(state, SZAPC, state->A);
			break;
		case JNZ:
			if (state->F & ZERO == 0){
				unsigned char lo = state->MEM[state->PC + 1];
				unsigned char hi = state->MEM[state->PC + 2];
				state->PC = (hi << 8) | lo;
				return 0;						
			}
			return 3;
		case JNC:
			if (state->F & CARRY == 0){
				unsigned char lo = state->MEM[state->PC + 1];
				unsigned char hi = state->MEM[state->PC + 2];
				state->PC = (hi << 8) | low;
				return 0;			
			}
			return 3;
		case JPO:
			if (state->F & PARITY == 0){
				unsigned char lo = state->MEM[state->PC + 1];
				unsigned char hi = state->MEM[state->PC + 2];
				state->PC = (hi << 8) | lo;				
				return 0;
			}
			return 3;
		case JP:
			if (state->F & SIGN == 0){
				unsigned char lo = state->MEM[state->PC + 1];
				unsigned char hi = state->MEM[state->PC + 2];
				state->PC = (hi << 8) | lo;				
				return 0;			
			}
			return 3;
	}

	return 1;
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
