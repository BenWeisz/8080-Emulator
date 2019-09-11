#include "emu.h"

STATE *state;

int main(int argc, char **argv){
	state = init();

	state->MEM[0] =	SBBL;

	for (int i = 0; i < 1; i++){
		state->PC += emulate();
	}

	printf("Flags: %xd %d\n", state->F, state->A);

	return 0;
}

STATE *init(){
	STATE *state = malloc(sizeof(STATE));
	state->A = 0x04;
	state->B = 0x00;
	state->C = 0x00;
	state->D = 0x00;
	state->E = 0x00;
	state->H = 0x00;
	state->L = 0x02;
	state->F = 0x01;

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
			state->B = alu(SZAP, state->B, 0x01, DCR);
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
			state->C = alu(SZAP, state->C, 0x01, DCR);
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
			state->D = alu(SZAP, state->D, 0x01, DCR);
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
			state->E = alu(SZAP, state->E, 0x01, DCR);
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
			state->H = alu(SZAP, state->H, 0x01, DCR);
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
			state->L = alu(SZAP, state->L, 0x01, DCR);
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

		// 0x30 - 0x3F
		case NOP30: {
			break;
		}
		case LXISP: {
			unsigned short v = (state->MEM[state->PC + 2] << 8) | state->MEM[state->PC + 1];
			state->SP = v;
			return 3;
		}
		case STA: {
			unsigned short adr = (state->MEM[state->PC + 2] << 8) | state->MEM[state->PC + 1];
			state->MEM[adr] = state->A;
			return 3;
		}
		case INXSP: {
			state->SP = state->SP + 1;
			break;
		}
		case INRM: {
			unsigned short adr = state->H << 8 | state->L;
			state->MEM[adr] = alu(SZAP, state->MEM[adr], 0x01, ADD);
			break;
		}
		case DCRM: {
			unsigned short adr = state->H << 8 | state->L;
			state->MEM[adr] = alu(SZAP, state->MEM[adr], 0x01, DCR);
			break;
		}
		case MVIM: {
			unsigned short adr = state->H << 8 | state->L;
			state->MEM[adr] = state->MEM[state->PC + 1];
			return 2;
		}
		case STC: {
			state->F |= CARRY;
			break;
		}
		case NOP38: {
			break;
		}
		case DADSP: {
			unsigned short v = alu(CARRY, state->SP >> 8, state->SP, DAD);
			state->H = v >> 8;
			state->L = v;
			break;
		}
		case LDA: {
			unsigned short adr = (state->MEM[state->PC + 2] << 8) | state->MEM[state->PC + 1];
			state->A = state->MEM[adr];
			return 3;
		}
		case DCXSP: {
			state->SP--;
			break;
		}
		case INRA: {
			state->A = alu(SZAP, state->A, 0x01, ADD);
			break;
		}
		case DCRA: {
			state->A = alu(SZAP, state->A, 0x01, DCR);
			break;
		}
		case MVIA: {
			state->A = state->MEM[state->PC + 1];
			return 2;
		}
		case CMC: {
			state->F ^= CARRY;
			break;
		}

		// 0x40 - 0x4F
		case MOVBB: {
			break;
		}
		case MOVBC: {
			state->B = state->C;
			break;
		}
		case MOVBD: {
			state->B = state->D;
			break;
		}
		case MOVBE: {
			state->B = state->E;
			break;
		}
		case MOVBH: {
			state->B = state->H;
			break;
		}
		case MOVBL: {
			state->B = state->L;
			break;
		}
		case MOVBM: {
			unsigned short adr = (state->H << 8) | state->L;
			state->B = state->MEM[adr];
			break;
		}
		case MOVBA: {
			state->B = state->A;
			break;
		}
		case MOVCB: {
			state->C = state->B;
			break;
		}
		case MOVCC: {
			break;
		}
		case MOVCD: {
			state->C = state->D;
			break;
		}
		case MOVCE: {
			state->C = state->E;
			break;
		}
		case MOVCH: {
			state->C = state->H;
			break;
		}
		case MOVCL: {
			state->C = state->L;
			break;
		}
		case MOVCM: {
			unsigned short adr = (state->H << 8) | state->L;
			state->C = state->MEM[adr];
			break;
		}
		case MOVCA: {
			state->C = state->A;
			break;
		}

		// 0x50 - 0x5F
		case MOVDB: {
			state->D = state->B;
			break;
		}
		case MOVDC: {
			state->D = state->C;
			break;
		}
		case MOVDD: {
			break;
		}
		case MOVDE: {
			state->D = state->E;
			break;
		}
		case MOVDH: {
			state->D = state->H;
			break;
		}
		case MOVDL: {
			state->D = state->L;
			break;
		}
		case MOVDM: {
			unsigned short adr = (state->H << 8) | state->L;
			state->D = state->MEM[adr];
			break;
		}
		case MOVDA: {
			state->D = state->A;
			break;
		}
		case MOVEB: {
			state->E = state->B;
			break;
		}
		case MOVEC: {
			state->E = state->C;
			break;
		}
		case MOVED: {
			state->E = state->D;
			break;
		}
		case MOVEE: {
			break;
		}
		case MOVEH: {
			state->E = state->H;
			break;
		}
		case MOVEL: {
			state->E = state->L;
			break;
		}
		case MOVEM: {
			unsigned short adr = (state->H << 8) | state->L;
			state->E = state->MEM[adr];
			break;
		}
		case MOVEA: {
			state->E = state->A;
			break;
		}

		// 0x60 - 0x6F
		case MOVHB: {
			state->H = state->B;
			break;
		}
		case MOVHC: {
			state->H = state->C;
			break;
		}
		case MOVHD: {
			state->H = state->D;
			break;
		}
		case MOVHE: {
			state->H = state->E;
			break;
		}
		case MOVHH: {
			break;
		}
		case MOVHL: {
			state->H = state->L;
			break;
		}
		case MOVHM: {
			unsigned short adr = (state->H << 8) | state->L;
			state->H = state->MEM[adr];
			break;
		}
		case MOVHA: {
			state->H = state->A;
			break;
		}
		case MOVLB: {
			state->L = state->B;
			break;
		}
		case MOVLC: {
			state->L = state->C;
			break;
		}
		case MOVLD: {
			state->L = state->D;
			break;
		}
		case MOVLE: {
			state->L = state->E;
			break;;
		}
		case MOVLH: {
			state->L = state->H;
			break;
		}
		case MOVLL: {
				break;
		}
		case MOVLM: {
			unsigned short adr = (state->H << 8) | state->L;
			state->L = state->MEM[adr];
			break;
		}
		case MOVLA: {
			state->L = state->A;
			break;
		}

		// 0x70 - 0x7F
		case MOVMB: {
			unsigned short adr = (state->H << 8) | state->L;
			state->MEM[adr] = state->B;
			break;
		}
		case MOVMC: {
			unsigned short adr = (state->H << 8) | state->L;
			state->MEM[adr] = state->C;
			break;
		}
		case MOVMD: {
			unsigned short adr = (state->H << 8) | state->L;
			state->MEM[adr] = state->D;
			break;
		}
		case MOVME: {
			unsigned short adr = (state->H << 8) | state->L;
			state->MEM[adr] = state->E;
			break;
		}
		case MOVMH: {
			unsigned short adr = (state->H << 8) | state->L;
			state->MEM[adr] = state->H;
			break;
		}
		case MOVML: {
			unsigned short adr = (state->H << 8) | state->L;
			state->MEM[adr] = state->L;
			break;
		}
		case HLT: {
			return -1;
		}
		case MOVAB: {
			state->A = state->B;
			break;
		}
		case MOVAC: {
			state->A = state->C;
			break;
		}
		case MOVAD: {
			state->A = state->D;
			break;
		}
		case MOVAE: {
			state->A = state->E;
			break;
		}
		case MOVAH: {
			state->A = state->H;
			break;
		}
		case MOVAL: {
			state->A = state->L;
			break;
		}
		case MOVAM: {
			unsigned short adr = (state->H << 8) | state->L;
			state->A = state->MEM[adr];
			break;
		}
		case MOVAA: {
			break;
		}

		// 0x80 - 0x8F
		case ADDB: {
			state->A = alu(SZAPC, state->A, state->B, ADD);
			break;
		}
		case ADDC: {
			state->A = alu(SZAPC, state->A, state->C, ADD);
			break;
		}
		case ADDD: {
			state->A = alu(SZAPC, state->A, state->D, ADD);
			break;
		}
		case ADDE: {
			state->A = alu(SZAPC, state->A, state->E, ADD);
			break;
		}
		case ADDH: {
			state->A = alu(SZAPC, state->A, state->H, ADD);
			break;
		}
		case ADDL: {
			state->A = alu(SZAPC, state->A, state->L, ADD);
			break;
		}
		case ADDM: {
			unsigned short adr = (state->H << 8) | state->L;
			state->A = alu(SZAPC, state->A, state->MEM[adr], ADD);
			break;
		}
		case ADDA: {
			state->A = alu(SZAPC, state->A, state->A, ADD);
			break;
		}
		case ADCB: {
			state->A = alu(SZAPC, state->A, state->B + (state->F & CARRY), ADD);
			break;
		}
		case ADCC: {
			state->A = alu(SZAPC, state->A, state->C + (state->F & CARRY), ADD);
			break;
		}
		case ADCD: {
			state->A = alu(SZAPC, state->A, state->D + (state->F & CARRY), ADD);
			break;
		}
		case ADCE: {
			state->A = alu(SZAPC, state->A, state->E + (state->F & CARRY), ADD);
			break;
		}
		case ADCH: {
			state->A = alu(SZAPC, state->A, state->H + (state->F & CARRY), ADD);
			break;
		}
		case ADCL: {
			state->A = alu(SZAPC, state->A, state->L + (state->F & CARRY), ADD);
			break;
		}
		case ADCM: {
			unsigned short adr = (state->H << 8) | state->L;
			state->A = alu(SZAPC, state->A, state->MEM[adr] + (state->F & CARRY), ADD);
			break;
		}
		case ADCA: {
			state->A = alu(SZAPC, state->A, state->A + (state->F & CARRY), ADD);
			break;
		}

		// 0x90 - 0x9F
		case SUBB: {
			state->A = alu(SZAPC, state->A, state->B, SUB);
			break;
		}
		case SUBC: {
			state->A = alu(SZAPC, state->A, state->C, SUB);
			break;
		}
		case SUBD: {
			state->A = alu(SZAPC, state->A, state->D, SUB);
			break;
		}
		case SUBE: {
			state->A = alu(SZAPC, state->A, state->E, SUB);
			break;
		}
		case SUBH: {
			state->A = alu(SZAPC, state->A, state->H, SUB);
			break;
		}
		case SUBL: {
			state->A = alu(SZAPC, state->A, state->L, SUB);
			break;
		}
		case SUBM: {
			unsigned short adr = (state->H << 8) | state->L;
			state->A = alu(SZAPC, state->A, state->MEM[adr], SUB);
			break;
		}
		case SUBA: {
			state->A = alu(SZAPC, state->A, state->A, SUB);
			break;
		}
		case SBBB: {
			state->A = alu(SZAPC, state->A, state->B + (state->F & CARRY), SUB);
			break;
		}
		case SBBC: {
			state->A = alu(SZAPC, state->A, state->C + (state->F & CARRY), SUB);
			break;
		}
		case SBBD: {
			state->A = alu(SZAPC, state->A, state->D + (state->F & CARRY), SUB);
			break;
		}
		case SBBE: {
			state->A = alu(SZAPC, state->A, state->E + (state->F & CARRY), SUB);
			break;
		}
		case SBBH: {
			state->A = alu(SZAPC, state->A, state->H + (state->F & CARRY), SUB);
			break;
		}
		case SBBL: {
			state->A = alu(SZAPC, state->A, state->L + (state->F & CARRY), SUB);
			break;
		}
		case SBBM: {
			unsigned short adr = (state->H << 8) | state->L;
			state->A = alu(SZAPC, state->A, state->MEM[adr] + (state->F & CARRY), SUB);
			break;
		}
		case SBBA: {
			state->A = alu(SZAPC, state->A, state->A + (state->F & CARRY), SUB);
			break;
		}

		// 0xA0 - 0xAF
		case ANAB: {
			state->A = alu(SZAPC, state->A, state->B, AND);
			break;
		}
		case ANAC: {
			state->A = alu(SZAPC, state->A, state->C, AND);
			break;
		}
		case ANAD: {
			state->A = alu(SZAPC, state->A, state->D, AND);
			break;		
		}
		case ANAE: {
			state->A = alu(SZAPC, state->A, state->E, AND);
			break;
		}
		case ANAH: {
			state->A = alu(SZAPC, state->A, state->H, AND);
			break;		
		}
		case ANAL: {
			state->A = alu(SZAPC, state->A, state->L, AND);
			break;
		}
		case ANAM: {
			unsigned short adr = (state->H << 8) | state->L;		
			state->A = alu(SZAPC, state->A, state->MEM[adr], AND);
			break;
		}
		case ANAA: {
			state->A = alu(SZAPC, state->A, state->A, AND);
			break;
		}
		case XRAB: {
			state->A = alu(SZAPC, state->A, state->B, XRA);
			break;	
		}
		case XRAC: {
			state->A = alu(SZAPC, state->A, state->C, XRA);
			break;		
		}
		case XRAD: {
			state->A = alu(SZAPC, state->A, state->D, XRA);
			break;
		}
		case XRAE: {
			state->A = alu(SZAPC, state->A, state->E, XRA);
			break;
		}
		case XRAH: {
			state->A = alu(SZAPC, state->A, state->H, XRA);
			break;
		}
		case XRAL: {
			state->A = alu(SZAPC, state->A, state->L, XRA);
			break;
		}
		case XRAM: {
			unsigned short adr = (state->H << 8) | state->L;
			state->A = alu(SZAPC, state->A, state->MEM[adr], XRA);
			break;
		}
		case XRAA: {
			state->A = alu(SZAPC, state->A, state->A, XRA);
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
	else if (op == DCR || op == SUB){
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
	else if (op == AND){
		full = a & b;
		half = a & b & 0x0F;
	}
	else if (op == XRA){
		full = a ^ b;
		half = (a & 0x0F) ^ (a & 0x0F);
	}

	//Carry Bit
	if (flags & CARRY){
		if (op != DAD){
			if (op != SUB){
				if ((full & 0x0100) >> 8 == 0x01)
					state->F |= CARRY;
				else
					state->F &= ~CARRY;
			}
			else {
				if ((full & 0x0100) >> 8 == 0x00)
					state->F |= CARRY;
				else
					state->F &= ~CARRY;
			}
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
