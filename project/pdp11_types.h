#ifndef _PDP11_TYPES_H_
#define _PDP11_TYPES_H_
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned short adr;
typedef struct _Var
{
	adr a;
	signed short val;
	word show_reg;
} Var;
#endif