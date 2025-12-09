TARGETNAME=Pegged
TARGETPATH=../../bin
TARGETTYPE=APPLICATION
CC = i686-pc-mingw32-gcc
AR = i686-pc-mingw32-ar
WINDRES = i686-pc-mingw32-windres -D_UNICODE=1 -DUNICODE=1
RANLIB = i686-pc-mingw32-ranlib
CFLAGS = -O2 -I. -Wall -Wpedantic -D_UNICODE=1 -DUNICODE=1
LDFLAGS = -static-libgcc -mwindows
LIBS = -lgdi32 -lhtmlhelp -lwinmm -lcomctl32
RM = rm -f
INSTALL = install -m 755
INSTALL_DIR = /usr

OBJS = \
	pegged.o \
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

