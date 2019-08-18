#include "emu.h"

STATE *state;

int main(int argc, char **argv){
	state = init();

	state->MEM[0] = CMA;

	for (int i = 0; i < 1; i++){
		state->PC += emulate();
	}

	printf("Flags: %d, %d\n", state->F, state->A);

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

int emulate(){
	unsigned char *op = &(state->MEM[state->PC]);
	
	switch(*op){
		// 0x00 - 0x0F
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
			unsigned short v = (state->B << 8) | state->C;
			v++;
			state->B = (v >> 8);
			state->C = v & 0x00FF;	
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
			state->A = alu(CARRY, state->A, 0x00, RLC);
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
			v--;
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
			state->A = alu(CARRY, state->A, 0x00, RRC);
			break;
		}

		// 0x10 - 0x1F
		case NOP10: {
			break;
		}
		case LXID: {
			state->D = state->MEM[state->PC + 2];
			state->E = state->MEM[state->PC + 1];
			return 3;
		}

		case STAXD: {
			unsigned short adr = (state->D << 8) | state->E;
			state->MEM[adr] = state->A;
			break;
		}
		case INXD: {
			unsigned short v = (state->D << 8) | state->E;
			v++;
			state->D = v >> 8;
			state->E = v;
			break;  
		}
		case INRD: {
			state->D = alu(SZAP, state->D, 0x01, ADD);
			break;
		}
		case DCRD: {
			state->D = alu(SZAP, state->D, 0x01, SUB);
			break;
		}
		case MVID: {
			state->D = state->MEM[state->PC + 1];
			return 2;
		}
		case RAL: {
			state->A = alu(CARRY, state->A, 0x00, RAL);
			break;
		}
		case NOP18: {
			break;
		}
		case DADD: {
			unsigned short v = alu(CARRY, state->D, state->E, DAD);
			state->H = v >> 8;
			state->L = v;
			break;
		}
		case LDAXD: {
			unsigned short adr = (state->D << 8) | state->E;
			state->A = state->MEM[adr];
			break;
		}
		case DCXD: {
			unsigned short v = (state->D << 8) | state->E;
			v--;
			state->D = v >> 8;
			state->E = v;
			break;
		}
		case INRE: {
			state->E = alu(SZAP, state->E, 0x01, ADD);
			break;
		}
		case DCRE: {
			state->E = alu(SZAP, state->E, 0x01, SUB);
			break;
		}
		case MVIE: {
			state->E = state->MEM[state->PC + 1];
			return 2;
		}
		case RAR: {
			state->A = alu(CARRY, state->A, 0x00, RAR);
			break;
		}

		// 0x20 - 0x2F
		case NOP20: {
			break;
		}
		case LXIH: {
			state->H = state->MEM[state->PC + 2];
			state->L = state->MEM[state->PC + 1];
			return 3;
		}
		case SHLD: {
			unsigned short adr = (state->MEM[state->PC + 2] << 8) | state->MEM[state->PC + 1];
			state->MEM[adr] = state->L;
			state->MEM[adr + 1] = state->H;
			return 3;
		}
		case INXH: {
			unsigned short v = (state->H << 8) | state->L;
			v++;
			state->H = v >> 8;
			state->L = v;
			break;
		}
		case INRH: {
			state->H = alu(SZAP, state->H, 0x01, ADD);
			break;
		}
		case DCRH: {
			state->H = alu(SZAP, state->H, 0x01, SUB);
			break;
		}
		case MVIH: {
			state->H = state->MEM[state->PC + 1];
			return 2;
		}
		case DAA: {
			state->A = alu(SZAPC, state->A, 0x00, DAA);
			break;
		}
		case NOP28: {
			break;
		}
		case DADH: {
			unsigned short v = alu(CARRY, state->H, state->L, DAD);
			state->H = v >> 8;
			state->L = v;
			break;
		}
		case LHLD: {
			unsigned short adr = (state->MEM[state->PC + 2] << 8) | state->MEM[state->PC + 1];
			state->L = state->MEM[adr];
			state->H = state->MEM[adr + 1];
			return 3;
		}
		case DCXH: {
			unsigned short v = (state->H << 8) | state->L;
			v--;
			state->H = v >> 8;
			state->L = v;
			break;
		}
		case INRL: {
			state->L = alu(SZAP, state->L, 0x01, ADD);
			break;
		}
		case DCRL: {
			state->L = alu(SZAP, state->L, 0x01, SUB);
			break;
		}
		case MVIL: {
			state->L = state->MEM[state->PC + 1];
			return 2;
		}
		case CMA: {
			state->A = ~(state->A);
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
	else if (op == RAL){
		full = a + b;
		full |= (state->F & CARRY) << 8;
		full = full << 1;
		full |= (full >> 9);
	}
	else if (op == RAR){
		full = a + b;
		full |= (state->F & CARRY) << 8;
		full |= (full & 0x01) << 9;
		full = full >> 1;
	}
	else if (op == DAA){
		full = a + b;
		half = (a + b) & 0x0F;
		
		if (half > 0x09 || (flags & AUX)){
			full = full + 0x06;
			half = half + 0x06;
		}

		full &= ~0x0100;

		if ((full >> 4) > 0x09 || (flags & CARRY))
			full = full + 0x60;
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
