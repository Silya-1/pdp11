#ifndef _PDP11_h_
#define _PDP11_h_
#define MEMSIZE 65536
#define REGCOUNT 8
#include <stdio.h>
#include "pdp11_types.h"
extern  byte byte_command;
extern byte mem[MEMSIZE];
extern short reg[REGCOUNT];
extern signed char xx;
#define PC reg[7]
extern Var ss;
extern Var dd;
extern byte Z, N, C, V; 
extern byte reg_num;
extern word nn;
#endif