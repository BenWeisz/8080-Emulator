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
	memset(state->IN_DEV, 0x00, 0xFF);
	memset(state->OUT_DEV, 0x00, 0xFF);

	state->INTE = 0x00;	

	return state;
}

int emulate(STATE *state){
	unsigned char *op = &(state->MEM[state->PC]);
	
	switch(*op){
		case NOP00: break;	
		case NOP10: break;	
		case NOP20: break;	
		case NOP30: break;
	 	case MOVBB: break;
		case MOVDB: {
			state->D = state->B;
			break;
		}
		case MOVHB: {
			state->H = state->B;
			break;
		}
		case MOVMB: {
			unsigned short adr = (state->H << 8) | state->L;
			state->MEM[adr] = state->B;
			break;
		}
		case ADDB: {
			state->A += state->B;
			flag(state, SZAPC, state->A);
			break;
		}
		case SUBB: {
			state->A -= state->B;
			flag(state, SZAPC, state->A);
			break;
		}
		case ANAB: {
			state->A = state->A & state->B;
			flag(state, SZAPC, state->A);
			break;
		}			
		case ORAB: {
			state->A = state->A | state->B;
			flag(state, SZAPC, state->A);
			break;
		}		
		case RNZ: {
			if (!(state->F & ZERO)){
				unsigned short sp = state->SP;
				state->PC = ((state->MEM[sp + 1] << 8) | state->MEM[sp]);
				state->SP += 2;
			}
			break;
		}		
		case RNC: {
			if (!(state->F & CARRY)){
				unsigned short sp = state->SP;
				state->PC = ((state->MEM[sp + 1] << 8) | state->MEM[sp]);
				state->SP += 2;
			}
			break;
		}		
		case RPO: {
			if (!(state->F & PARITY)){
				unsigned short sp = state->SP;
				state->PC = ((state->MEM[sp + 1] << 8) | state->MEM[sp]);
				state->SP += 2;
			}
			break;
		}		
		case RP: {
			if (!(state->F & SIGN)){
				unsigned short sp = state->SP;
				state->PC = ((state->MEM[sp + 1] << 8) | state->MEM[sp]);
				state->SP += 2;
			}
			break;
		}		
		case LXIB: {
			state->B = state->MEM[state->PC + 2];
			state->C = state->MEM[state->PC + 1];
			return 3;
		}		
		case LXID: {
			state->D = state->MEM[state->PC + 2];
			state->E = state->MEM[state->PC + 1];
			return 3;
		}		
		case LXIH: {
			state->H = state->MEM[state->PC + 2];
			state->L = state->MEM[state->PC + 1];
			return 3;
		}		
		case LXISP: {
			state->SP = state->MEM[state->PC + 2] << 8 | state->MEM[state->PC + 1];
			return 3;
		}		
		case MOVBC: {
			state->B = state->C;
			break;
		}		
		case MOVDC: {
			state->D = state->C;
			break;
		}		
		case MOVHC: {
			state->H = state->C;
			break;
		}		
		case MOVMC: {
			unsigned short adr = (state->H << 8) | state->L;
			state->MEM[adr] = state->C;
			break;
		}		
		case ADDC: {
			state->A += state->C;
			flag(state, SZAPC, state->A);
			break;
		}		
		case SUBC: {
			state->A -= state->C;
			flag(state, SZAPC, state->A);
			break;
		}			
		case ANAC: {
			state->A = state->A & state->C;
			flag(state, SZAPC, state->A);
			break;
		}
		case ORAC: {
			state->A = state->A | state->C;
			flag(state, SZAPC, state->A);
			break;
		}
		case POPB: {
			state->B = state->MEM[state->SP + 1];
			state->C = state->MEM[state->SP];
			state->SP += 2;
			break;
		}
		case POPD: {
			state->D = state->MEM[state->SP + 1];
			state->E = state->MEM[state->SP];
			state->SP += 2;
			break;
		}
		case POPH: {
			state->H = state->MEM[state->SP + 1];
			state->L = state->MEM[state->SP];
			state->SP += 2;
			break;
		}
		case POPPSW: {
			state->F = state->MEM[state->SP]; // TODO --> POSSIBLE ERROR
			state->SP += 2;
			flag(state, SZAPC, state->A);
			break;
		}
		case STAXB: {
			state->MEM[(state->B << 8) | state->C] = state->A;
			break;
		}
		case STAXD: {
			state->MEM[(state->D << 8) | state->C] = state->A;
			break;
		}
		case SHLD: {
			unsigned char lo = state->MEM[state->PC + 1];
			unsigned char hi = state->MEM[state->PC + 2];
			state->MEM[(hi << 8) | lo] = state->L;
			state->MEM[((hi << 8) | lo) + 1] = state->H;
			return 3;
		}
		case STA: {
			unsigned char lo = state->MEM[state->PC + 1];
			unsigned char hi = state->MEM[state->PC + 2];
			state->MEM[(hi << 8) | lo] = state->A;
			return 3;
		}
		case MOVBD: {
			state->B = state->D;
			break;
		}
		case MOVDD: {
			state->D = state->D;
			break;
		}
		case MOVHD: {
			state->H = state->D;
			break;
		}
		case MOVMD: {
			unsigned short adr = (state->H << 8) | state->L;
			state->MEM[adr] = state->D;
			break;	
		}
		case ADDD: {
			state->A += state->D;
			flag(state, SZAPC, state->A);
			break;
		}
		case SUBD: {
			state->A -= state->D;
			flag(state, SZAPC, state->A);
			break;
		}
		case ANAD: {
			state->A = state->A & state->D;
			flag(state, SZAPC, state->A);		
			break;
		}
		case ORAD: {
			state->A = state->A | state->D;
			flag(state, SZAPC, state->A);
			break;
		}
		case JNZ: {
			if ((state->F & ZERO) == 0){
				unsigned char lo = state->MEM[state->PC + 1];
				unsigned char hi = state->MEM[state->PC + 2];
				state->PC = (hi << 8) | lo;
				return 0;						
			}
			return 3;
		}
		case JNC: {
			if ((state->F & CARRY) == 0){
				unsigned char lo = state->MEM[state->PC + 1];
				unsigned char hi = state->MEM[state->PC + 2];
				state->PC = (hi << 8) | lo;
				return 0;			
			}
			return 3;
		}
		case JPO: {
			if ((state->F & PARITY) == 0){
				unsigned char lo = state->MEM[state->PC + 1];
				unsigned char hi = state->MEM[state->PC + 2];
				state->PC = (hi << 8) | lo;				
				return 0;
			}
			return 3;
		}
		case JP: {
			if ((state->F & SIGN) == 0){
				unsigned char lo = state->MEM[state->PC + 1];
				unsigned char hi = state->MEM[state->PC + 2];
				state->PC = (hi << 8) | lo;				
				return 0;			
			}
			return 3;
		}
		case INXB: {
			state->C = state->C + 1;
			if (state->C == 0x00)
				state->B = state->B + 1;
			break;
		}
		case INXD: {
			state->E = state->E + 1;
			if (state->E == 0x00)
				state->D = state->D + 1;
			break;
		}
		case INXH: {
			state->L = state->L + 1;
			if (state->L == 0x00)
				state->H = state->H + 1;
			break;
		}
		case INXSP: {
			state->SP = state->SP + 1;
			break;
		}
		case MOVBE: {
			state->B = state->E;
			break;
		}
		case MOVDE: {
			state->D = state->E;
			break;
		}
		case MOVHE: {
			state->H = state->E;
		}
		case MOVME: {
			unsigned short adr = (state->H << 8) | state->L;
			state->MEM[adr] = state->E;
			break;
		}
		case ADDE: {
			state->A = state->A + state->E;
			flag(state, SZAPC, state->A);
			break;
		}
		case SUBE: {
			state->A = state->A - state->E;
			flag(state, SZAPC, state->A);
			break;
		}
		case ANAE: {
			state->A = state->A & state->E;
			flag(state, SZAPC, state->A);
			break;
		}
		case ORAE: {
			state->A = state->A | state->E;
			flag(state, SZAPC, state->A);
			break;
		}
		case JMPC3: {
			unsigned char lo = state->MEM[state->PC + 1];
			unsigned char hi = state->MEM[state->PC + 2];
			state->PC = (hi << 8) | lo;
			return 0;
		}
		case OUT: {
			unsigned char dev = state->MEM[state->PC + 1];
			state->OUT_DEV[dev] = state->A;
			return 2;
		}
		case XTHL: {
			unsigned char t0 = state->L;
			unsigned char t1 = state->H;
			state->L = state->MEM[state->SP];
			state->H = state->MEM[state->SP + 1];
			state->MEM[state->SP] = t0;
			state->MEM[state->SP + 1] = t1;
			break;
		}
		case DI: {
			state->INTE = 0x00;
			break;
		}
		case INRB: {
			state->B = state->B + 1;
			flag(state, SZAP, state->B);
			break;
		}
		case INRD: {
			state->D = state->D + 1;
			flag(state, SZAP, state->D);
			break;
		}
		case INRH: {
			state->H = state->H + 1;
			flag(state, SZAP, state->H);
			break;
		}
		case INRM: {
			unsigned short adr = (state->H << 8) | state->L;
			state->MEM[adr] = state->MEM[adr] + 1;
			flag(state, SZAP, state->MEM[adr]);
			break;
		}
		case MOVBH: {
			state->B = state->H;
			break;
		}
		case MOVDH: {
			state->D = state->H;
			break;
		}
		case MOVHH: {
			state->H = state->H;
			break;
		}
		case MOVMH: {
			unsigned short adr = (state->H << 8) | state->L;
			state->MEM[adr] = state->H;
			break;
		}
		case ADDH: {
			state->A = state->A + state->H;
			flag(state, SZAPC, state->A);
			break;
		}
		case SUBH: {
			state->A = state->A - state->H;
			flag(state, SZAPC, state->A);
			break;
		}
		case ANAH: {
			state->A = state->A & state->H;
			flag(state, SZAPC, state->A);
			break;
		}
		case ORAH: {
			state->A = state->A | state->H;
			flag(state, SZAPC, state->A);
			break;
		}
		case CNZ: {
			if (state->F & ZERO){
				unsigned short ret_adr = state->PC + 3;
				state->MEM[state->SP - 1] = (ret_adr & 0xFF00) >> 8;
				state->MEM[state->SP] = ret_adr & 0x00FF;
				state->SP = state->SP - 2;

				unsigned char lo = state->MEM[state->PC + 1];
				unsigned char hi = state->MEM[state->PC + 2];

				unsigned short jmp_adr = (hi << 8) | lo;
				state->PC = jmp_adr;
				return 0;
			}
			return 3;
		}
		case CNC: {
			if (!(state->F & CARRY)){
				unsigned short ret_adr = state->PC + 3;
				state->MEM[state->SP - 1] = (ret_adr & 0xFF00) >> 8;
				state->MEM[state->SP] = ret_adr & 0x00FF;
				state->SP = state->SP - 2;

				unsigned char lo = state->MEM[state->PC + 1];
				unsigned char hi = state->MEM[state->PC + 2];

				unsigned short jmp_adr = (hi << 8) | lo;
				state->PC = jmp_adr;
				return 0;
			}
			return 3;
		}
		case CPO: {
			if (!(state->F & PARITY)){
				unsigned short ret_adr = state->PC + 3;
				state->MEM[state->SP - 1] = (ret_adr & 0xFF00) >> 8;
				state->MEM[state->SP] = ret_adr & 0x00FF;
				state->SP = state->SP - 2;

				unsigned char lo = state->MEM[state->PC + 1];
				unsigned char hi = state->MEM[state->PC + 2];

				unsigned short jmp_adr = (hi << 8) | lo;
				state->PC = jmp_adr;
				return 0;
			}
			return 3;
		}
		case CP: {
			if (!(state->F & SIGN)){
				unsigned short ret_adr = state->PC + 3;
				state->MEM[state->SP - 1] = (ret_adr & 0xFF00) >> 8;
				state->MEM[state->SP] = ret_adr & 0x00FF;
				state->SP = state->SP - 2;

				unsigned char lo = state->MEM[state->PC + 1];
				unsigned char hi = state->MEM[state->PC + 2];

				unsigned short jmp_adr = (hi << 8) | lo;
				state->PC = jmp_adr;
				return 0;
			}
			return 3;
		}
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
