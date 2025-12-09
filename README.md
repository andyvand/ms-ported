# Various Microsoft applications and games ported to Visual Studio 2026.
# Various Microsoft applications and games ported to MinGW (i386, x86_64 and aarch64).

- Solitaire
- Reversi
- Hearts
- Winmine
- Freecell
- Pegged
- Winver
- version.dll

It requires for a MinGW build of mshearts the MFC library ported to MinGW.
- To build it go to atlmfc/src/mfc in the Terminal
- make -f GNUmakefile (i386)
- make -f GNUmakefile.MinGW64 (x86_64)
- make -f GNUmakefile.MinGWARM64 (arm64)

Enjoy ;-)
