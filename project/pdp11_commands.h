#ifndef _PDP11_COMMANDS_h_
#define _PDP11_COMMANDS_h_
#include "pdp11_types.h"
byte b_read  (adr a);
void b_write (adr a, byte val, byte show_reg);   
word w_read  (adr a);
void w_write (adr a, word val, byte show_reg);
void mem_dump(adr start, word n);
void load_file(FILE *  src);
void reg_dump();
void do_sob();
void do_clr();
void do_inc();
void do_dec();
void do_tst();
void do_bpl();
void do_bne();
extern signed char xx;
#endif