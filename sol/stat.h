#ifndef __STAT_H__
#define __STAT_H__

/* PUBLICs */
BOOL FRegisterStat(BOOL fFirstInst);
BOOL FDestroyStat( VOID );
VOID StatRender( VOID );
VOID StatUpdate( VOID );
VOID StatMove( VOID );
VOID StatString(INT ids);
VOID StatStringSz(TCHAR *sz);

#endif /* __STAT_H__ */
