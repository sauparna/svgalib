include ../Makefile.cfg

svgalibroot = ..
VPATH       = $(svgalibroot)/demos.x
O           = bin
CFLAGS      = $(WARN) $(OPTIMIZE)
INCLUDE     = -I$(svgalibroot)/include -I$(svgalibroot)/gl
ifeq (a.out, $(TARGET_FORMAT))
  CFLAGS   += -DSVGA_AOUT
endif
ifdef INSTALLSHAREDLIB
  LIB       = -lvgagl -lvga
endif
ifndef LIB
  LIB       = $(svgalibroot)/staticlib/libvgagl.a $(svgalibroot)/staticlib/libvga.a
  LVGADEP   = $(LIB)
endif

SRC         = 	accel.c 	\
		addmodetest.c 	\
		bankspeed.c 	\
		bg_test.c 	\
		cursor.c 	\
		eventtest.c 	\
		forktest.c 	\
		fun.c 		\
		joytest.c 	\
		keytest.c 	\
		linearfork.c 	\
		linearspeed.c 	\
		lineart.c 	\
		mjoytest.c 	\
		mousetest.c 	\
		printftest.c 	\
		scrolltest.c 	\
		speedtest.c 	\
		spin.c 		\
		svidtune.c 	\
		testaccel.c 	\
		testgl.c 	\
		testlinear.c 	\
		vgatest.c 	\
		vgatweak.c

BIN = $(SRC:.c=)

all:	$(BIN) buildcsr

.c:
	mkdir -p $(O)
	$(CC) $(CFLAGS) $(INCLUDE) -o $(O)/$* $*.c $(LDFLAGS) $(LIB)
   
buildcsr: mkcur.o
	$(CC) $(CFLAGS) $(INCLUDE) -o $(O)/buildcsr mkcur.o $(LDFLAGS) $(LIB)

clean:
	rm -rf $(O) .depend *.o *~ *.bak

.PHONY: all clean
