#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include "pdp11.h"
//#include <error.h> 
#include "pdp11_commands.h"
#include "commands.h"
#include "pdp11_types.h"
byte mem[MEMSIZE];
short reg[REGCOUNT];
signed char xx;
Var ss;
Var dd;
word nn;
byte reg_num;
byte byte_command;
byte Z, N, C, V;
byte trace_lvl;
//extern Command commands[];
void trace(int lvl, int num, ...);
 Var get_mr(word cm)
{
	Var var;
	int r = cm & 7;
	int mode = (cm >> 3) & 7;
	adr a;
	word cm_next;
	char inf;
	switch(mode) {
		case 0:
			var.a = r;
			var.val = reg[r];
			trace(trace_lvl, 2, 0, r);
			//printf("R%o ", r);
			var.show_reg = 1;
			break;
		case 1:
			a = reg[r];
			var.a = a;
			if (byte_command == 1)
				var.val = (signed char)b_read(a);	
			else
				var.val = (signed short)w_read(a);
			trace(trace_lvl, 3, 1, r, var.val);
			break;
			var.show_reg = 0;
		case 2:
			a = reg[r];
			var.a = a;
			if (byte_command == 1)
				var.val = (signed char)b_read(a);	
			else
				var.val = (signed short)w_read(a);
			trace (trace_lvl, 3, 2, r, var.val);
			if ((byte_command == 1) && (r < 6))
				reg[r] += 1;
			else//
				reg[r] += 2; // +1 byte !R7 !R6
			var.show_reg = 0;
			break;
		case 3:
			a = reg[r];
			//printf(" %o ", r);
			if ((byte_command == 1)  && (r > 5))
			{
				var.a = w_read(a);	
				var.val = (signed char)b_read(var.a);	
			}
			else
			{
				var.a = w_read(a);
				var.val = (signed short)w_read(var.a);
			}
			trace(trace_lvl, 3, 3, 7, var.a);
			var.show_reg = 0;
			reg[r] += 2;
			break;
		case 4:
			if ((byte_command == 1) && (r < 6))
				reg[r] -= 1;
			else
				reg[r] -= 2;
			a = reg[r];
			var.a = a;
			if (byte_command == 1)
				var.val = (signed char)b_read(a);
			else
				var.val = (signed short)w_read(a);
			var.show_reg = 0;
			trace(trace_lvl, 2, 4, r);
			break;
		case 5:
			reg[r] -= 2;
			a = reg[r];
			var.a = a;
			var.a = w_read(var.a);
			if (byte_command == 1)
				var.val = (signed char)b_read(var.a);
			else
				var.val = (signed short)w_read(var.a);
			trace(trace_lvl, 2, 4, r);
			var.show_reg = 0;
			break;
		case 6:
			cm_next = w_read(PC);
			a = cm_next + reg[r];
			var.a = a;
			if (byte_command == 1)
			{
				var.val = (signed char)b_read(a);
			}
			else
				var.val = (signed short)w_read(a);
			var.show_reg = 0;
			trace(trace_lvl, 4, 6, r, var.val, cm_next);
			break;
		case 7:
			cm_next = (signed short )w_read(PC);
			//PC += 2;
			a = cm_next + reg[r];
			if (byte_command == 1)
			{
				a = b_read(a);	
				var.a = a;
				var.val = b_read(a);
			}
			else
			{
				a = w_read(a);
				var.a = a;
				var.val = w_read(a);
			}
			trace(trace_lvl, 4, 7, r, var.val, cm_next);
			break;
	}
	return var;
}
void findknown()
{
	int ch;
	word cm;
	PC = 01000;
	while(1)
	{
		cm = w_read(PC);
		//printf("%06o : %06o ", PC, cm);
		trace(trace_lvl, 2, PC, cm);
		PC += 2;
		for(ch = 0; ch < sizeof(commands) / sizeof(Command); ch++)
		{
			if ((cm & commands[ch].mask) == commands[ch].opcode)
			{
				byte_command = commands[ch].byte_cm;
				//printf("%s ",commands[ch].name);
				trace(trace_lvl, 1, commands[ch].name);
				//printf(" %o ", byte_command);
				if((commands[ch].param & XX) == XX)
				{
					xx = cm & (0377);
				}
				if((commands[ch].param & DD) == DD)
				{ 
					dd = get_mr(cm);
				}
				if((commands[ch].param & SS) == SS)
				{
					ss = get_mr(cm >> 6);
				}
				if((commands[ch].param & NN) == NN)
				{
					nn = cm & 077;
					reg_num = (cm & 0700) >> 6;
				}
				commands[ch].func();
				break;
			}
		}
		//printf("\n");
		trace(trace_lvl, 1, "\n");
	}
}
void trace(int lvl, int num, ...)
{
	if (lvl == 0)
		return;
	//printf("ggggg   %d ffffff", lvl);
	int mod, r, val; 
	va_list args;
	va_start (args, num);
	if (num == 1) 
	{
		printf("%s ", va_arg(args, char*));
		return;
	}
	mod = va_arg(args, int);
	if (mod < 01000)
	{
		switch  (mod)
		{
			case 0:
				r = va_arg(args, int);
				printf(" R%o ", r);
				break;
			case 1:
				r = va_arg(args, int);
				if (r==7)
					printf("#%o ", va_arg(args, int));
				else
					printf("(R%o) ", r);
				break;
			case 2:
				r = va_arg(args, int);
				if (r==7)
					printf("#%o ", va_arg(args, int));
				else//
					printf("(R%o)+ ", r);
				break;
			case 3:
				r = va_arg(args, int);
				if (r == 7)
					printf("@ #%o ", va_arg(args, int));
				else
					printf("@(R%o)+ ", r);
				break;
			case 4:
				r = va_arg(args, int);
				printf("-(R%o) ", r);
				break;
			case 5:
				r = va_arg(args, int);
				printf("@ -(R%o) ", r);
				break;
			case 6:
				r = va_arg(args, int);
				val = va_arg(args, int);
				if (r==7)
					printf("%o ", val);
				else
					printf("%o(R%o)",va_arg(args, int), r);
				break;
			case 7:
				r = va_arg(args, int);
				val = va_arg(args, int);
				if (r==7)
					printf("@ %o ", val);
				else
					printf("@ %o(R%o)+ ",va_arg(args, int),r);
				break;
		}
	}
	else
		printf("%06o : %06o ", mod,va_arg(args, int) );

}
int main(int argc, char* argv[])
{
	
	mem[0177564] = 0;
	FILE *src = NULL;
	if (strcmp(argv[1], "-t") == 0)
	{
		trace_lvl = 1;
	}
	else
	{
		trace_lvl = 0;
	}
	//printf(" %d  ", trace_lvl);
	if(argc == 1)
		src = stdin;
	else
		src = fopen(argv[argc - 1], "r");
	if (src == NULL) {
	    perror("in.txt");  
	    return 7;          
	}
    load_file(src);
	mem_dump(512, 16);
	printf("\n");
	fclose (src);
	findknown();
	return 0;
}
