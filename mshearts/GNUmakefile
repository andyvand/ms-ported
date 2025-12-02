TARGETNAME=mshearts
TARGETPATH=../../bin
TARGETTYPE=APPLICATION
CC = i686-pc-mingw32-gcc
CXX = i686-pc-mingw32-g++
AR = i686-pc-mingw32-ar
WINDRES = i686-pc-mingw32-windres
RANLIB = i686-pc-mingw32-ranlib
CFLAGS = -O2 -I. -Wall -Wpedantic -I../atlmfc/include -Wno-unused-result -Wno-delete-incomplete -Wno-return-type -Wno-int-to-pointer-cast -Wno-attributes -Wno-deprecated-declarations -Wno-invalid-offsetof -Wno-delete-non-virtual-dtor -Wno-unknown-pragmas -Wno-unused-value -Wno-class-memaccess -g3
LDFLAGS = -static-libgcc -mwindows -g3
LIBS = -L../atlmfc/mfc/x86 -lmfc_static -lgdi32 -lhtmlhelp -lgdi32 -ladvapi32 -lcomctl32 -lgdiplus -loleaut32 -loleacc -loledlg -lole32 -luuid -luxtheme -lhtmlhelp -lwinspool -lws2_32 -lurlmon -lwininet -limagehlp -lshlwapi -lodbc32 -limm32 -lcrypt32 -lpropsys -lmsimg32 -ldwmapi -lrpcrt4 -lwinmm -lcomdlg32 -Wl,-Bstatic,-lstdc++ -Wl,-Bstatic,-lsupc++
RM = rm -f
INSTALL = install -m 755
INSTALL_DIR = /usr

OBJS = \
    ../atlmfc/src/mfc/mfcintl_res.o \
    cards.o \
    card.o \
	comp2.o \
	computer.o \
	dde.o \
	ddecb.o \
	dlg.o \
	hearts.o \
	human.o \
	lead.o \
	main.o \
	main2.o \
	player.o \
	sound.o \
	welcome.o \
	cards_res.o \
	hearts_res.o

%_res.o: %.rc
	$(WINDRES) -o $@ $<

%.o: %.cpp
	$(CXX) $(CFLAGS) -DLIB_EXPORTS=1 -o $@ -c $<

%.o: %.c
	$(CC) $(CFLAGS) -DLIB_EXPORTS=1 -o $@ -c $<

all: $(TARGETNAME).exe

$(TARGETNAME).exe: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

clean:
	$(RM) $(OBJS) $(DLL_OBJS) $(TARGETNAME).exe

install:
	$(INSTALL) $(TARGETNAME).exe $(INSTALL_DIR)/bin/$(TARGETNAME).exe

