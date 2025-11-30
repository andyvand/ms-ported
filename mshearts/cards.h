#ifndef CARDS_H
#define CARDS_H 1

#include <windows.h>

#ifdef __cplusplus
extern "C"
{
#endif
    extern BOOL fKlondWinner;
    extern BOOL APIENTRY cdtInit(INT FAR *pdxCard, INT FAR *pdyCard);
    extern BOOL APIENTRY cdtDrawExt(HDC hdc, INT x, INT y, INT dx, INT dy, INT cd, INT mode, DWORD rgbBgnd);
    extern BOOL APIENTRY cdtDraw(HDC hdc, INT x, INT y, INT cd, INT mode, DWORD rgbBgnd);
    extern BOOL APIENTRY cdtAnimate(HDC hdc, INT cd, INT x, INT y, INT ispr);
    extern VOID APIENTRY cdtTerm();
#ifdef __cplusplus
}
#endif

#endif /* CARDS_H */
