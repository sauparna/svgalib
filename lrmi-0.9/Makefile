CFLAGS = -g -Wall

O   = 	bin

SRC = 	dosint.c 	\
	mode3.c 	\
	vbemodeinfo.c 	\
	vbetest.c 	\
	vga_reset.c

BIN = $(SRC:.c=)

all: $(BIN)

.c:
	mkdir -p $(O)
	$(CC) $(CFLAGS) -o $(O)/$* $*.c lrmi.c

install: $(O)/mode3 $(O)/vga_reset
	install $(O)/mode3 /sbin
	install $(O)/vga_reset /sbin

clean:
	rm -rf $(O) *.o core regs-out *.bak

distclean: clean
	rm -f .depend

depend: $(sources)
	-$(CC) -M $(CFLAGS) $^ >.depend

.PHONY: clean distclean depend
