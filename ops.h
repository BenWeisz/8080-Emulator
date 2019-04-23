#define NOP00  0x00
#define NOP10  0x10
#define NOP20  0x20
#define NOP30  0x30
#define MOVBB  0x40
#define MOVDB  0x50
#define MOVHB  0x60
#define MOVMB  0x70
#define ADDB   0x80
#define SUBB   0x90
#define ANAB   0xA0
#define ORAB   0xB0
#define RNZ    0xC0
#define RNC    0xD0
#define RPO    0xE0
#define RP     0xF0

#define LXIB   0x01
#define LXID   0x11
#define LXIH   0x21
#define LXISP  0x31
#define MOVBC  0x41
#define MOVDC  0x51
#define MOVHC  0x61
#define MOVMC  0x71
#define ADDC   0x81
#define SUBC   0x91
#define ANAC   0xA1
#define ORAC   0xB1
#define POPB   0xC1
#define POPD   0xD1
#define POPH   0xE1
#define POPPSW 0xF1

#define STAXB  0x02
#define STAXD  0x12
#define SHLD   0x22
#define STA    0x32
#define MOVBD  0x42
#define MOVDD  0x52
#define MOVHD  0x62
#define MOVMD  0x72
#define ADDD   0x82
#define SUBD   0x92
#define ANAD   0xA2
#define ORAD   0xB2
#define JNZ    0xC2
#define JNC    0xD2
#define JPO    0xE2
#define JP     0xF2

#define INXB   0x03
#define INXD   0x13
#define INXH   0x23
#define INXSP  0x33
#define MOVBE  0x43
#define MOVDE  0x53
#define MOVHE  0x63
#define MOVME  0x73
#define ADDE   0x83
#define SUBE   0x93
#define ANAE   0xA3
#define ORAE   0xB3
#define JMP    0xC3
#define OUT    0xD3
#define XTHL   0xE3
#define DI     0xF3

#define INRB   0x04
#define INRD   0x14
#define INRH   0x24
#define INRM   0x34
#define MOVBH  0x44
#define MOVDH  0x54
#define MOVHH  0x64
#define MOVMH  0x74
#define ADDH   0x84
#define SUBH   0x94
#define ANAH   0xA4
#define ORAH   0xB4
#define CNZ    0xC4
#define CNC    0xD4
#define CP0    0xE4
#define CP     0xF4

#define DCRB   0x05
#define DCRD   0x15
#define DCRH   0x25
#define DCRM   0x35
#define MOVBL  0x45
#define MOVDL  0x55
#define MOVHL  0x65
#define MOVML  0x75
#define ADDH   0x85
#define SUBH   0x95
#define ANAH   0xA5
#define ORAH   0xB5
#define CNZ    0xC5
#define CNC    0xD5
#define CPO    0xE5
#define CP     0xF5

#define MVIB   0x06
#define MVID   0x16
#define MVIH   0x26
#define MVIM   0x36
#define MOVBM  0x46
#define MOVDM  0x56
#define MOVHM  0x66
#define HLT    0x76
#define ADDM   0x86
#define SUBM   0x96
#define ANAM   0xA6
#define ORAM   0xB6
#define ADI    0xC6
#define SUI    0xD6
#define ANI    0xE6
#define ORI    0xF6
