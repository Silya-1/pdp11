#ifndef _COMMANDS_h_
#define _COMMANDS_h_
#include "pdp11.h"
#include "pdp11_types.h"
typedef struct _commmand {
	unsigned int opcode;
	unsigned int mask;
	char name[20];
	void(*func)(void);
	byte param;
	word byte_cm;
} Command;
/* operation prototypes */
void do_add();
void do_move();
void do_halt();
void do_br();
#define NO 0
#define SS 1
#define DD 2
#define XX 4
#define NN 8
/* end of operation prototypes */
extern Command commands[];
Command commands[] =
{
	{0000000,~0000000, "halt",do_halt, NO, 0},
	{0110000,~0007777, "moveb",do_move, SS | DD, 1},
	{0010000,~0007777, "move",do_move, SS | DD, 0},
	{0060000,~0007777, "add", do_add,  SS | DD, 0},
	{0000400,~0000377, "br", do_br, XX, 0},
	{0005000,~0000077, "clr", do_clr, DD, 0},
	{0105000,~0000077, "clrb", do_clr, DD, 1},
	{0077000,~0000777, "sob", do_sob, NN, 0},
	{0005200,~0000077, "inc", do_inc, DD, 0},
	{0105200,~0000077, "inc", do_inc, DD, 1},
	{0005300,~0000077, "dec", do_dec, DD, 0},
	{0105300,~0000077, "dec", do_dec, DD, 1},
	{0005700,~0000077, "tst", do_tst, DD, 0},
	{0105700,~0000077, "tstb", do_tst, DD, 1},
	{0100000,~0000377, "bpl", do_bpl, XX,0},
	{0001000,~0000377, "bne", do_bne, XX, 0}
};	
#endif