#include "emu.h"

STATE *state;

int main(int argc, char **argv){
	state = init();

	unsigned char b = alu(SZAPC, 0x18, 0xF7, SUB);
	printf("%d %d\n", state->F, b);

	return 0;
}

STATE *init(){
	STATE *state = malloc(sizeof(STATE));
	state->A = 0x00;
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

int emulate(STATE *state){
	unsigned char *op = &(state->MEM[state->PC]);
	
	switch(*op){

	}

	return 1;
}

unsigned char alu(unsigned char flags, unsigned char a, unsigned char b, unsigned char op){
	unsigned short full = 0x0000;
	unsigned char half = 0x00;
	
	if (op == ADD){
		full = a + b;
		half = (a & 0x0F) + (b & 0x0F);
	}
	else if (op == SUB){
		full = a + ((b ^ 0xFF) + 1);
		half = (a & 0x0F) + (((b ^ 0xFF) + 1) & 0x0F);
	}
	
	//Carry Bit
	if (flags & CARRY){
		if ((full & 0x0100) >> 8 == 0x01)
			state->F |= CARRY;
		else
			state->F &= ~CARRY;
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

	return full & 0xFF;
}
