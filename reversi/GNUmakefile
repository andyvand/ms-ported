TARGETNAME=Reversi
TARGETPATH=../../bin
TARGETTYPE=APPLICATION
CC = i686-pc-mingw32-gcc
AR = i686-pc-mingw32-ar
WINDRES = i686-pc-mingw32-windres
RANLIB = i686-pc-mingw32-ranlib
CFLAGS = -O2 -I. -Wall -Wpedantic
LDFLAGS = -static-libgcc
LIBS = -lgdi32 -lhtmlhelp
RM = rm -f
INSTALL = install -m 755
INSTALL_DIR = /usr

OBJS = \
    reversi.o \
    rev.o \
    res.o

%.o: %.rc
	$(WINDRES) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -DLIB_EXPORTS=1 -o $@ -c $<

all: $(TARGETNAME).exe

$(TARGETNAME).exe: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	$(RM) $(OBJS) $(DLL_OBJS) $(TARGETNAME).exe

install:
	$(INSTALL) $(TARGETNAME).exe $(INSTALL_DIR)/bin/$(TARGETNAME).exe

