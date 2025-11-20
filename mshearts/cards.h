#ifndef CARDS_H
#define CARDS_H 1

#include <windows.h>

BOOL APIENTRY cdtInit(INT FAR *pdxCard, INT FAR *pdyCard);
BOOL APIENTRY cdtDrawExt(HDC hdc, INT x, INT y, INT dx, INT dy, INT cd, INT mode, DWORD rgbBgnd);
BOOL APIENTRY cdtDraw(HDC hdc, INT x, INT y, INT cd, INT mode, DWORD rgbBgnd);
BOOL APIENTRY cdtAnimate(HDC hdc, INT cd, INT x, INT y, INT ispr);
VOID APIENTRY cdtTerm();

#endif /* CARDS_H */
