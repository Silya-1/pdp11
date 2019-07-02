#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <error.h>     
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned short adr;
byte mem[65536];
adr pc = 01000;
struct cmd
{
	word opcode;
	word mask;
	char name[20];
	void(*func)(void);

};
void do_add()
{
	printf("not completed yet");
}
void do_halt()
{
	exit(0);
}
void do_move()
{
	printf("not completed yet");
}
struct cmd comand[] = 
{
	{0000000,~0000000, "halt",do_halt},
	{0010000,~0007777, "move",do_move},
	{0060000,~0007777, "add", do_add}
};
byte b_read  (adr a)     
{
	return mem[a];
}
void b_write (adr a, byte val) 
{
	mem[a] = val;
}
word w_read  (adr a)
{
	word l = (((word)mem[a]) | ((word)mem[a+1] << 8));
	return l; 
}
void w_write (adr a, word val) 
{
	word ch = 0xff00;
	b_write(a, (byte)val&(~ch));
	b_write(a + 1, (byte)((val & ch) >> 8));
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
			b_write(adress, inf);
			adress++;
		}
	}
}
1void findknown()
{
	int num;
	int ch;
	word cm;
	for(pc; ; pc += 2)
	{
		cm = w_read(pc);
		printf("%06o : %o ", pc, cm);
		for(ch = 0; ch < sizeof(comand)/sizeof( struct cmd); ch++)
		{
			if ((cm & comand[ch].mask) == comand[ch].opcode)
			{
				printf("%s ",comand[ch].name);
				comand[ch].func();
				break;
			}
		}
		printf("\n");
	}
}
int main(int argc, char* argv[])
{
	FILE *src = NULL;
	if(argc == 1)
		src = stdin;
	else
		src = fopen(argv[argc - 1], "r");
	if (src == NULL) {
	    perror("in.txt");  
	    return 7;          
	}
    load_file(src);
	//mem_dump(64, 12);
	printf("\n");
	fclose (src);
	findknown();
	return 0;
}
