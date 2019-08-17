#include "emu.h"

STATE *state;

int main(int argc, char **argv){
	state = init();

	state->MEM[0] = RRC;

	for (int i = 0; i < 1; i++){
		state->PC += emulate();
	}

	printf("Flags: %d, %d\n", state->F, state->A);

	return 0;
}

STATE *init(){
	STATE *state = malloc(sizeof(STATE));
	state->A = 0x01;
	state->B = 0x00;
	state->C = 0x00;
	state->D = 0x00;
	state->E = 0x00;
	state->H = 0x00;
	state->L = 0x00;
	state->F = 0x00;

	state->PC = 0x0000;
	state->SP = 0x0000;

	memset(state->MEM, 0x00, 0xFFFF);

	return state;
}

int emulate(){
	unsigned char *op = &(state->MEM[state->PC]);
	
	switch(*op){
		case NOP00: {
			break;
		} 
		case LXIB: {
			state->B = state->MEM[state->PC + 2];
			state->C = state->MEM[state->PC + 1];
			return 3;	
		}
		case STAXB: {
			unsigned short adr = (state->B << 8) | state->C;
			state->MEM[adr] = state->A;
			break;
		}
		case INXB: {
			unsigned short comb = (state->B << 8) | state->C;
			comb++;
			state->B = (comb >> 8);
			state->C = comb & 0x00FF;	
			break;
		}
		case INRB: {
			state->B = alu(SZAP, state->B, 0x01, ADD);
			break;
		}
		case DCRB: {
			state->B = alu(SZAP, state->B, 0x01, SUB);
			break;
		}
		case MVIB: {
			state->B = state->MEM[state->PC + 1];
			return 2;
		}
		case RLC: {
			state->A = alu(CARRY, state->A, 0, RLC);
			break;
		}
		case NOP08: {
			break;
		}
		case DADB: {
			unsigned short v = alu(CARRY, state->B, state->C, DAD);
			state->H = v >> 8;
			state->L = v;
			break;
		}
		case LDAXB: {
			unsigned short adr = (state->B << 8) | state->C;
			state->A = state->MEM[adr];
			break;
		}
		case DCXB: {
			unsigned short v = (state->B << 8) | state->C;
			v = v - 0x0001;
			state->B = v >> 8;
			state->C = v;
			break; 
		}
		case INRC: {
			state->C = alu(SZAP, state->C, 0x01, ADD);
			break;
		}
		case DCRC: {
			state->C = alu(SZAP, state->C, 0x01, SUB);
			break;
		}
		case MVIC: {
			state->C = state->MEM[state->PC + 1];
			return 2;
		}
		case RRC: {
			state->A = alu(CARRY, state->A, 0, RRC);
			break;
		}
	}

	return 1;
}

unsigned short alu(unsigned char flags, unsigned char a, unsigned char b, unsigned char op){
	unsigned int full = 0x00000000;
	unsigned char half = 0x00;
	
	if (op == ADD){
		full = a + b;
		half = (a & 0x0F) + (b & 0x0F);
	}
	else if (op == SUB){
		full = a + ((b ^ 0xFF) + 1);
		half = (a & 0x0F) + (((b ^ 0xFF) + 1) & 0x0F);
	}
	else if (op == RLC){
		full = a + b;
		full = full << 1;
		full |= (full & 0x0100) >> 8;
	}
	else if (op == DAD){
		unsigned short ap = (a << 8) | b;
		unsigned short bp = (state->H << 8) | state->L;
		full = ap + bp;
	}
	else if (op == RRC){
		full = a + b;
		full |= (full & 0x01) << 8;
		full |= (full & 0x01) << 9;
		full = full >> 1;
	}
	
	//Carry Bit
	if (flags & CARRY){
		if (op != DAD){
			if ((full & 0x0100) >> 8 == 0x01)
				state->F |= CARRY;
			else
				state->F &= ~CARRY;
		}
		else {
			if ((full & 0x00010000) >> 16 == 0x01)
				state->F |= CARRY;
			else
				state->F &= ~CARRY;
		}
	}

	//Parity Bit
	if (flags & PARITY){
		unsigned char parity = 0x01;
		parity ^= full & 0x01;
		parity ^= (full & 0x02) >> 1;
		parity ^= (full & 0x04) >> 2;
		parity ^= (full & 0x08) >> 3;
		parity ^= (full & 0x10) >> 4;
		parity ^= (full & 0x20) >> 5;
		parity ^= (full & 0x40) >> 6;
		parity ^= (full & 0x80) >> 7;
		if (parity == 0x01)
			state->F |= PARITY;
		else
			state->F &= ~PARITY;
	}

	//Auxiliary Bit
	if (flags & AUX){
		if (((half & AUX) >> 4) == 0x01)
			state->F |= AUX;
		else 
			state->F &= ~AUX;
	}

	//Zero Bit
	if (flags & ZERO){
		if ((full & 0x00FF) == 0x00)
			state->F |= ZERO;
		else
			state->F &= ~ZERO;
	}

	//Sign Bit
	if (flags & SIGN){
		if (((full & SIGN) >> 7) == 0x01)
			state->F |= SIGN;
		else
			state->F &= ~SIGN;
	}

	if (op != DAD)
		return full & 0xFF;
	else
		return full & 0xFFFF;
}
