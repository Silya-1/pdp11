#include "pdp11.h"
#include "pdp11_commands.h"
byte b_read  (adr a)     
{
	return mem[a];
}
void b_write (adr a, byte val, byte show_reg) 
{
	if (show_reg == 1)
	{
		reg[a] = (signed char)val;
		(signed char)val < 0 ?  V = 1 : 0;
		return;
	}
	mem[a] = val;
}
word w_read  (adr a)
{
	word l = (((word)mem[a]) | ((word)mem[a+1] << 8));
	return l; 
}
void w_write (adr a, word val, byte show_reg) 
{
	if(show_reg == 1) 
	{
		reg[a] = (signed short)val;
		return;
	}
	word ch = 0xff00;
	b_write(a, (byte)val&(~ch), 0);
	b_write(a + 1, (byte)((val & ch) >> 8), 0);
}
void mem_dump(adr start, word n)
{
	for(; n > 0; n -= 2)
	{
		printf("%06o", start);
		printf(" : ");
		printf("%06o\n", w_read(start));
		start += 2;
	}
}
void load_file(FILE *  src)
{

	unsigned int adress;
	unsigned int inf;
	unsigned int num;
	while((fscanf(src,"%x%x", &adress, &num)) == 2)
	{
		for(; num != 0; num--) 
		{
			fscanf(src,"%x", &inf);
			b_write(adress, inf, 0);
			adress++;
		}
	}
}
void reg_dump()
{
	int i;
	printf("\n");
	for(i = 0; i < REGCOUNT; i++)
		printf("%o ", reg[i]);
	printf("\n");
}