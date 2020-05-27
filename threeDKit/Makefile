include ../Makefile.cfg

ROOT	= ..
VPATH	= $(ROOT)/src
SRCDIR	= .
O	= bin
OBJDIR  = obj
LIBDIR  = lib

SRC 	= 3dinit.c 	\
	  3dkit.c  	\
	  quickmath.c 	\
	  swtriangle.c 	\
	  triangl.c 	\
	  triangle.c 	\
	  wrapsurf.c 	\
	  wtriangle.c

INC    += -I$(ROOT)/src
OBJ     = $(SRC:.c=.o)
LDFLAGS	= -lvgagl -lvga -lm

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

all: $(LIBDIR)/lib3dkit.a $(LIBDIR)/lib3dkit.so.$(VERSION) plane wrap

$(LIBDIR)/lib3dkit.so.$(VERSION): $(OBJ)
	mkdir -p $(LIBDIR)
	$(CC) -shared -Wl,-soname,lib3dkit.so.$(MAJOR_VER) \
	  -o $(LIBDIR)/lib3dkit.so.$(VERSION) $(OBJ) -lm

$(sharedlibdir)/lib3dkit.so.$(VERSION): $(LIBDIR)/lib3dkit.so.$(VERSION)
	$(INSTALL_SHLIB) $< $(sharedlibdir)/lib3dkit.so.$(VERSION)
	(cd $(sharedlibdir); ln -sf lib3dkit.so.$(VERSION) `echo lib3dkit.so.$(VERSION) | sed 's/\.so\..*/.so/'` )
	-ldconfig

install: $(sharedlibdir)/lib3dkit.so.$(VERSION) installheaders

installheaders:
	@cp 3dkit.h $(includedir)/3dkit.h
	@chmod a+r $(includedir)/3dkit.h
	@cp triangle.h $(includedir)/triangle.h
	@chmod a+r $(includedir)/triangle.h

$(LIBDIR)/lib3dkit.a: $(ALLOBJS)
	mkdir -p $(LIBDIR)
	$(AR) rcs $(LIBDIR)/lib3dkit.a $(ALLOBJS)

plane: planukit.o planinit.o $(OBJ)
	mkdir -p $(O)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(O)/plane planukit.o planinit.o $(OBJ)

wrap: wrapdemo.o $(OBJ)
	mkdir -p $(O)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(O)/wrap wrapdemo.o $(OBJ)

clean:
	rm -rf $(O) $(LIBDIR) *.o *.bak core 

.PHONY:	all clean install installheaders
