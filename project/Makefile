all: pdp
pdp:main.o commands.o workercommands.c
	gcc  -o  $@ $^
%.o:%/c
	gcc -c -MD -Wall -O2  -o$@ $<
-include $(*d)
clean:
	rm -rf *.o *.d
