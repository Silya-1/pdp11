#include "pdp11.h"
#include <stdlib.h>
#include <stdio.h>
#include "pdp11_types.h"
//#include "commands.h"s
#include "pdp11_commands.h"
void do_add()
{
	char show_reg;
	w_write(dd.a, (signed short)dd.val + (signed short)ss.val, dd.show_reg);
	if ((((signed short)dd.val > 0) && ((signed short)ss.val > 0) && (((signed short)dd.val + (signed short)ss.val) < 0)) || (((signed short)dd.val < 0) && ((signed short)ss.val < 0) && (((signed short)dd.val + (signed short)ss.val) > 0)))
		V = 1;
	else 
		V = 0;
	if (((signed short)dd.val < 0) && ((signed short)ss.val > 0) && (((signed short)dd.val + (signed short)ss.val) > 0)) 
		C = 1;
	else
		C = 0;

}
void do_halt()
{
	reg_dump();
	printf("%c", mem[0177566]);
	//printf("%o %o\n", mem[0100], mem[0101]);
	exit(0);
}
void do_move()
{
	char show_reg;
	if(dd.a == 0177566)
	{
		printf("%c", ss.val);
	}
	if (byte_command == 1)
		b_write(dd.a, (signed char)ss.val, dd.show_reg);
	else
		w_write(dd.a, (signed short)ss.val, dd.show_reg);
}
void do_br()
{
	PC = PC + 2 * xx;
}
void do_sob()
{
	if(--reg[reg_num] == 0)
		return;
	PC = PC - 2 * nn;
}
void do_clr()
{
	char show_reg;
	if((dd.a & 07) == 07) 
		show_reg = 0;
	else 
	if (byte_command == 1)
		b_write(dd.a, 0, dd.show_reg);
	else
		w_write(dd.a, 0, dd.show_reg);
	Z = 1;
}
void do_inc()
{
	char show_reg;
	if((dd.a & 07) == 07) 
		show_reg = 0;
	else 
		show_reg = 1;
	if (byte_command == 1)
		b_write(dd.a, (signed char)dd.val + 1, dd.show_reg);
	else
		w_write(dd.a, (signed short)dd.val + 1, dd.show_reg);
	if (((signed short)dd.val > 0)  && (((signed short)dd.val + 1) == 0))
		V = 1;
	else 
		V = 0;

}//
void do_dec()
{
	char show_reg;
	if((dd.a & 07) == 07) 
		show_reg = 0;
	else 
		show_reg = 1;
	if (byte_command == 1)
		b_write(dd.a, (signed char)dd.val - 1, dd.show_reg);
	else
		w_write(dd.a, (signed short)dd.val - 1, dd.show_reg);
	if (((signed short)dd.val < 0)  && (((signed short)dd.val - 1) == 0))
		V = 1;
	else 
		V = 0;
}
void do_tst()
{
	if (byte_command == 1)
	{
		//b_write(dd.a, (signed char)dd.val - 1, dd.show_reg);
		if (dd.val == 0)
			Z = 1;
		else 
			Z = 0;
		if((signed char)dd.val <= 0)
			N = 1;
		else 
			N = 0;

	}
	else
	{
		//w_write(dd.a, (signed short)dd.val - 1, dd.show_reg);
		if (dd.val == 0)
			Z = 1;
		else
			Z = 0;
		if((signed short)dd.val <= 0)
			N = 1;
		else
			N = 0;

	}
}
void do_bpl()
{
	if (N == 0)
		do_br();
}
void do_bne()
{
	if (Z == 0)
		do_br();
}