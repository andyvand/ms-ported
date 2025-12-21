//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
// This is a part of the Microsoft Foundation Classes C++ library.
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

// Inlines for AFXCMN.H (via WINCTRL6.CPP)

#ifdef _AFXCMN_INLINE

#if !defined(_WIN32_WCE_NO_CONTROLBARS)
/////////////////////////////////////////////////////////////////////////////

_AFXCMN_INLINE BOOL CStatusBarCtrl::IsSimple() const
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, SB_ISSIMPLE, 0, 0); }
WCE_DEL _AFXCMN_INLINE void CStatusBarCtrl::SetTipText(int nPane, LPCTSTR pszTipText)
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, SB_SETTIPTEXT, nPane, (LPARAM)pszTipText); }
WCE_DEL _AFXCMN_INLINE COLORREF CStatusBarCtrl::SetBkColor(COLORREF cr)
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, SB_SETBKCOLOR, 0, (LPARAM)cr); }
WCE_DEL _AFXCMN_INLINE BOOL CStatusBarCtrl::SetIcon(int nPane, HICON hIcon)
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, SB_SETICON, nPane, (LPARAM)hIcon); }

/////////////////////////////////////////////////////////////////////////////

_AFXCMN_INLINE void CSpinButtonCtrl::SetRange32(int nLower, int nUpper)
	{ ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, UDM_SETRANGE32, (WPARAM) nLower, (LPARAM) nUpper); }
_AFXCMN_INLINE void CSpinButtonCtrl::GetRange32(int& nLower, int& nUpper) const
	{ ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, UDM_GETRANGE32, (WPARAM) &nLower, (LPARAM) &nUpper); }

/////////////////////////////////////////////////////////////////////////////

_AFXCMN_INLINE DWORD CToolBarCtrl::GetButtonSize() const
    { ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TB_GETBUTTONSIZE, 0, 0L); }
_AFXCMN_INLINE CImageList* CToolBarCtrl::GetDisabledImageList() const
    { ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, TB_GETDISABLEDIMAGELIST, 0, 0)); }
WCE_DEL _AFXCMN_INLINE CImageList* CToolBarCtrl::GetHotImageList() const
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, TB_GETHOTIMAGELIST, 0, 0)); }
_AFXCMN_INLINE CImageList* CToolBarCtrl::GetImageList() const
	{ ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, TB_GETIMAGELIST, 0, 0)); }
_AFXCMN_INLINE DWORD CToolBarCtrl::GetStyle() const
    { ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TB_GETSTYLE, 0, 0L); }
_AFXCMN_INLINE INT CToolBarCtrl::GetMaxTextRows() const
    { ASSERT(::IsWindow(m_hWnd)); return (INT) ::SendMessage(m_hWnd, TB_GETTEXTROWS, 0, 0L); }
_AFXCMN_INLINE BOOL CToolBarCtrl::GetRect(int nID, LPRECT lpRect) const
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_GETRECT, nID, (LPARAM)lpRect); }
_AFXCMN_INLINE BOOL CToolBarCtrl::IsButtonHighlighted(int nID) const
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_ISBUTTONHIGHLIGHTED, nID, 0); }
_AFXCMN_INLINE void CToolBarCtrl::LoadImages(int iBitmapID, HINSTANCE hinst)
    { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, TB_LOADIMAGES, iBitmapID, (LPARAM)hinst); }
_AFXCMN_INLINE BOOL CToolBarCtrl::SetButtonWidth(int cxMin, int cxMax)
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_SETBUTTONWIDTH, 0, MAKELPARAM(cxMin, cxMax)); }
_AFXCMN_INLINE CImageList* CToolBarCtrl::SetDisabledImageList(CImageList* pImageList)
    { ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, TB_SETDISABLEDIMAGELIST, 0, (LPARAM)pImageList->GetSafeHandle())); }
WCE_DEL _AFXCMN_INLINE CImageList* CToolBarCtrl::SetHotImageList(CImageList* pImageList)
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, TB_SETHOTIMAGELIST, 0, (LPARAM)pImageList->GetSafeHandle())); }
_AFXCMN_INLINE CImageList* CToolBarCtrl::SetImageList(CImageList* pImageList)
    { ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, TB_SETIMAGELIST, 0, (LPARAM)pImageList->GetSafeHandle())); }
_AFXCMN_INLINE BOOL CToolBarCtrl::SetIndent(int iIndent)
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_SETINDENT, iIndent, 0L); }
_AFXCMN_INLINE BOOL CToolBarCtrl::SetMaxTextRows(int iMaxRows)
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, iMaxRows, 0L); }
_AFXCMN_INLINE void CToolBarCtrl::SetStyle(DWORD dwStyle)
    { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, TB_SETSTYLE, 0, dwStyle); }
_AFXCMN_INLINE BOOL CToolBarCtrl::GetButtonInfo(int nID, TBBUTTONINFO* ptbbi) const
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_GETBUTTONINFO, nID, (LPARAM)ptbbi); }
_AFXCMN_INLINE BOOL CToolBarCtrl::SetButtonInfo(int nID, TBBUTTONINFO* ptbbi)
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_SETBUTTONINFO, nID, (LPARAM)ptbbi); }
_AFXCMN_INLINE DWORD CToolBarCtrl::SetDrawTextFlags(DWORD dwMask, DWORD dwDTFlags)
    { ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TB_SETDRAWTEXTFLAGS, dwMask, dwDTFlags); }
WCE_DEL _AFXCMN_INLINE BOOL CToolBarCtrl::GetAnchorHighlight() const
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_GETANCHORHIGHLIGHT, 0, 0); }
WCE_DEL _AFXCMN_INLINE BOOL CToolBarCtrl::SetAnchorHighlight(BOOL fAnchor)
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_SETANCHORHIGHLIGHT, fAnchor, 0); }
WCE_DEL _AFXCMN_INLINE int CToolBarCtrl::GetHotItem() const
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, TB_GETHOTITEM, 0, 0); }
WCE_DEL _AFXCMN_INLINE int CToolBarCtrl::SetHotItem(int nHot)
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, TB_SETHOTITEM, nHot, 0); }
WCE_DEL _AFXCMN_INLINE void CToolBarCtrl::GetInsertMark(TBINSERTMARK* ptbim) const
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, TB_GETINSERTMARK, 0, (LPARAM)ptbim); }
WCE_DEL _AFXCMN_INLINE void CToolBarCtrl::SetInsertMark(TBINSERTMARK* ptbim)
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, TB_SETINSERTMARK, 0, (LPARAM)ptbim); }
WCE_DEL _AFXCMN_INLINE BOOL CToolBarCtrl::GetMaxSize(LPSIZE pSize) const
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_GETMAXSIZE, 0, (LPARAM)pSize); }
WCE_DEL _AFXCMN_INLINE BOOL CToolBarCtrl::InsertMarkHitTest(LPPOINT ppt, LPTBINSERTMARK ptbim) const
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_INSERTMARKHITTEST, (WPARAM)ppt, (LPARAM)ptbim); }
WCE_DEL _AFXCMN_INLINE BOOL CToolBarCtrl::MapAccelerator(TCHAR chAccel, UINT* pIDBtn)
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_MAPACCELERATOR, (WPARAM)chAccel, (LPARAM)pIDBtn); }
WCE_DEL _AFXCMN_INLINE BOOL CToolBarCtrl::MarkButton(int nID, BOOL bHighlight)
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_MARKBUTTON, nID, MAKELPARAM(bHighlight, 0)); }
WCE_DEL _AFXCMN_INLINE BOOL CToolBarCtrl::MoveButton(UINT nOldPos, UINT nNewPos)
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_MOVEBUTTON, nOldPos, nNewPos); }
#endif // _WIN32_WCE_NO_CONTROLBARS

/////////////////////////////////////////////////////////////////////////////

_AFXCMN_INLINE DWORD CListCtrl::SetExtendedStyle(DWORD dwNewStyle)
    { ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM) dwNewStyle); }
WCE_DEL _AFXCMN_INLINE HCURSOR CListCtrl::SetHotCursor(HCURSOR hc)
WCE_DEL { ASSERT(::IsWindow(m_hWnd) && hc != NULL); return (HCURSOR) ::SendMessage(m_hWnd, LVM_SETHOTCURSOR, 0, (LPARAM) hc); }
WCE_DEL _AFXCMN_INLINE int CListCtrl::SetHotItem(int iIndex)
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_SETHOTITEM, (WPARAM) iIndex, 0); }
WCE_DEL _AFXCMN_INLINE void CListCtrl::SetWorkAreas(int nWorkAreas, LPRECT lpRect)
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, LVM_SETWORKAREAS, nWorkAreas, (LPARAM) lpRect); }
_AFXCMN_INLINE int CListCtrl::SubItemHitTest(LPLVHITTESTINFO pInfo)
    { ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_SUBITEMHITTEST, 0, (LPARAM) pInfo); }
WCE_DEL _AFXCMN_INLINE HCURSOR CListCtrl::GetHotCursor()
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (HCURSOR) ::SendMessage(m_hWnd, LVM_GETHOTCURSOR, 0, 0); }
WCE_DEL _AFXCMN_INLINE int CListCtrl::GetHotItem()
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_GETHOTITEM, 0, 0); }
_AFXCMN_INLINE DWORD CListCtrl::GetExtendedStyle()
    { ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0); }
_AFXCMN_INLINE CSize CListCtrl::ApproximateViewRect(CSize sz, int iCount) const
    { ASSERT(::IsWindow(m_hWnd)); return CSize((DWORD) ::SendMessage(m_hWnd, LVM_APPROXIMATEVIEWRECT, iCount, MAKELPARAM(sz.cx, sz.cy))); }
WCE_DEL _AFXCMN_INLINE BOOL CListCtrl::GetBkImage(LVBKIMAGE* plvbkImage) const
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, LVM_GETBKIMAGE, 0, (LPARAM)plvbkImage); }
WCE_DEL _AFXCMN_INLINE DWORD CListCtrl::GetHoverTime() const
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, LVM_GETHOVERTIME, 0, 0); }
WCE_DEL _AFXCMN_INLINE void CListCtrl::GetWorkAreas(int nWorkAreas, LPRECT prc) const
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, LVM_GETWORKAREAS, nWorkAreas, (LPARAM)prc); }
WCE_DEL _AFXCMN_INLINE BOOL CListCtrl::SetBkImage(LVBKIMAGE* plvbkImage)
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, LVM_SETBKIMAGE, 0, (LPARAM)plvbkImage); }
WCE_DEL _AFXCMN_INLINE DWORD CListCtrl::SetHoverTime(DWORD dwHoverTime)
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (DWORD)::SendMessage(m_hWnd, LVM_SETHOVERTIME, 0, dwHoverTime); }
WCE_DEL _AFXCMN_INLINE UINT CListCtrl::GetNumberOfWorkAreas() const
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); UINT nWorkAreas; ::SendMessage(m_hWnd, LVM_GETNUMBEROFWORKAREAS, 0, (LPARAM)&nWorkAreas); return nWorkAreas; }
_AFXCMN_INLINE int CListCtrl::SetSelectionMark(int iIndex)
	{ ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_SETSELECTIONMARK, 0, (LPARAM) iIndex); }
_AFXCMN_INLINE int CListCtrl::GetSelectionMark()
	{ ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, LVM_GETSELECTIONMARK, 0, 0); }

/////////////////////////////////////////////////////////////////////////////

_AFXCMN_INLINE BOOL CHeaderCtrl::GetItemRect(int nIndex, LPRECT lpRect) const
    { ASSERT(::IsWindow(m_hWnd)); ASSERT(lpRect != NULL); return (BOOL)::SendMessage(m_hWnd, HDM_GETITEMRECT, nIndex, (LPARAM)lpRect); }
_AFXCMN_INLINE int CHeaderCtrl::OrderToIndex(int nOrder) const
    { ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, HDM_ORDERTOINDEX, nOrder, 0); }
_AFXCMN_INLINE int CHeaderCtrl::SetHotDivider(CPoint pt)
    { ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, HDM_SETHOTDIVIDER, TRUE, MAKELPARAM(pt.x, pt.y)); }
_AFXCMN_INLINE int CHeaderCtrl::SetHotDivider(int nIndex)
    { ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, HDM_SETHOTDIVIDER, FALSE, nIndex); }
_AFXCMN_INLINE CImageList* CHeaderCtrl::GetImageList() const
    { ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, HDM_GETIMAGELIST, 0, 0L)); }
_AFXCMN_INLINE CImageList* CHeaderCtrl::SetImageList(CImageList* pImageList)
    { ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, HDM_SETIMAGELIST, 0, (LPARAM)pImageList->GetSafeHandle())); }
_AFXCMN_INLINE CImageList* CHeaderCtrl::CreateDragImage(int nIndex)
    { ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, HDM_CREATEDRAGIMAGE, nIndex, 0L)); }

/////////////////////////////////////////////////////////////////////////////

#if !defined(_WIN32_WCE_NO_CONTROLBARS)
_AFXCMN_INLINE CReBarCtrl::CReBarCtrl()
	{ }
_AFXCMN_INLINE UINT CReBarCtrl::GetBandCount() const
    { ASSERT(::IsWindow(m_hWnd)); return (UINT) ::SendMessage(m_hWnd, RB_GETBANDCOUNT, 0, 0L); }
_AFXCMN_INLINE BOOL CReBarCtrl::GetBandInfo(UINT uBand, REBARBANDINFO* prbbi) const
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_GETBANDINFO, uBand, (LPARAM)prbbi); }
_AFXCMN_INLINE UINT CReBarCtrl::GetBarHeight() const
    { ASSERT(::IsWindow(m_hWnd)); return (UINT) ::SendMessage(m_hWnd, RB_GETBARHEIGHT, 0, 0L); }
_AFXCMN_INLINE BOOL CReBarCtrl::GetBarInfo(REBARINFO* prbi) const
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_GETBARINFO, 0, (LPARAM)prbi); }
_AFXCMN_INLINE COLORREF CReBarCtrl::GetBkColor() const
    { ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, RB_GETBKCOLOR, 0, 0L); }
WCE_DEL _AFXCMN_INLINE IDropTarget* CReBarCtrl::GetDropTarget() const
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); IDropTarget* pdt; ::SendMessage(m_hWnd, RB_GETDROPTARGET, 0, (LPARAM)&pdt); return pdt; }
_AFXCMN_INLINE BOOL CReBarCtrl::GetRect(UINT uBand, LPRECT prc) const
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_GETRECT, uBand, (LPARAM)prc); }
_AFXCMN_INLINE UINT CReBarCtrl::GetRowCount() const
    { ASSERT(::IsWindow(m_hWnd)); return (UINT) ::SendMessage(m_hWnd, RB_GETROWCOUNT, 0, 0L); }
_AFXCMN_INLINE UINT CReBarCtrl::GetRowHeight(UINT uRow) const
    { ASSERT(::IsWindow(m_hWnd)); return (UINT) ::SendMessage(m_hWnd, RB_GETROWHEIGHT, uRow, 0L); }
_AFXCMN_INLINE COLORREF CReBarCtrl::GetTextColor() const
    { ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, RB_GETTEXTCOLOR, 0, 0L); }
#if !defined(_WIN32_WCE_NO_TOOLTIPS)
_AFXCMN_INLINE CToolTipCtrl* CReBarCtrl::GetToolTips() const
    { ASSERT(::IsWindow(m_hWnd)); return (CToolTipCtrl*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, RB_GETTOOLTIPS, 0, 0L)); }
#endif // _WIN32_WCE_NO_TOOLTIPS
_AFXCMN_INLINE int CReBarCtrl::IDToIndex(UINT uBandID) const
    { ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, RB_IDTOINDEX, uBandID, 0L); }
_AFXCMN_INLINE BOOL CReBarCtrl::SetBandInfo(UINT uBand, REBARBANDINFO* prbbi)
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_SETBANDINFO, uBand, (LPARAM)prbbi); }
_AFXCMN_INLINE BOOL CReBarCtrl::SetBarInfo(REBARINFO* prbi)
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_SETBARINFO, 0, (LPARAM)prbi); }
_AFXCMN_INLINE COLORREF CReBarCtrl::SetBkColor(COLORREF clr)
    { ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, RB_SETBKCOLOR, 0, (LPARAM)clr); }
_AFXCMN_INLINE CWnd* CReBarCtrl::SetOwner(CWnd* pWnd)
    { ASSERT(::IsWindow(m_hWnd)); return CWnd::FromHandle((HWND)::SendMessage(m_hWnd, RB_SETPARENT, (WPARAM)pWnd->m_hWnd, 0L)); }
_AFXCMN_INLINE COLORREF CReBarCtrl::SetTextColor(COLORREF clr)
    { ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, RB_SETTEXTCOLOR, 0, (LPARAM)clr); }
#if !defined(_WIN32_WCE_NO_TOOLTIPS)
_AFXCMN_INLINE void CReBarCtrl::SetToolTips(CToolTipCtrl* pTip)
    { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, RB_SETTOOLTIPS, (WPARAM)pTip->m_hWnd, 0L); }
#endif // _WIN32_WCE_NO_TOOLTIPS
_AFXCMN_INLINE void CReBarCtrl::BeginDrag(UINT uBand, DWORD dwPos)
    { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, RB_BEGINDRAG, uBand, dwPos); }
_AFXCMN_INLINE BOOL CReBarCtrl::DeleteBand(UINT uBand)
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_DELETEBAND, uBand, 0L); }
_AFXCMN_INLINE void CReBarCtrl::DragMove(DWORD dwPos)
    { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, RB_DRAGMOVE, 0, dwPos); }
_AFXCMN_INLINE void CReBarCtrl::EndDrag()
    { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, RB_ENDDRAG, 0, 0L); }
_AFXCMN_INLINE int CReBarCtrl::HitTest(RBHITTESTINFO* prbht)
    { ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, RB_HITTEST, 0, (LPARAM)prbht); }
_AFXCMN_INLINE BOOL CReBarCtrl::InsertBand(UINT uIndex, REBARBANDINFO* prbbi)
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_INSERTBAND, uIndex, (LPARAM)prbbi); }
_AFXCMN_INLINE void CReBarCtrl::MaximizeBand(UINT uBand)
    { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, RB_MAXIMIZEBAND, uBand, 0L); }
_AFXCMN_INLINE void CReBarCtrl::MinimizeBand(UINT uBand)
    { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, RB_MINIMIZEBAND, uBand, 0L); }
_AFXCMN_INLINE BOOL CReBarCtrl::SizeToRect(CRect& rect)
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_SIZETORECT, 0, (LPARAM)&rect); }
_AFXCMN_INLINE BOOL CReBarCtrl::ShowBand(UINT uBand, BOOL fShow /*= TRUE*/)
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_SHOWBAND, uBand, fShow); }
_AFXCMN_INLINE void CReBarCtrl::GetBandBorders(UINT uBand, LPRECT prc) const
    { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, RB_GETBANDBORDERS, uBand, (LPARAM)prc); }
WCE_DEL _AFXCMN_INLINE CPalette* CReBarCtrl::GetPalette() const
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return CPalette::FromHandle((HPALETTE)::SendMessage(m_hWnd, RB_GETPALETTE, 0, 0L)); }
WCE_DEL _AFXCMN_INLINE CPalette* CReBarCtrl::SetPalette(HPALETTE hPal)
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return CPalette::FromHandle((HPALETTE)::SendMessage(m_hWnd, RB_SETPALETTE, 0, (LPARAM)hPal)); }
WCE_DEL _AFXCMN_INLINE BOOL CReBarCtrl::MoveBand(UINT uFrom, UINT uTo)
WCE_DEL { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, RB_MOVEBAND, uFrom, uTo); }
#endif // _WIN32_WCE_NO_CONTROLBARS

/////////////////////////////////////////////////////////////////////////////

#if !defined(_WIN32_WCE_NO_TOOLTIPS)
_AFXCMN_INLINE void CToolTipCtrl::SetDelayTime(UINT nDelay)
    { ASSERT(::IsWindow(m_hWnd));  ::SendMessage(m_hWnd, TTM_SETDELAYTIME, 0, nDelay); }
_AFXCMN_INLINE int CToolTipCtrl::GetDelayTime(DWORD dwDuration) const
    { ASSERT(::IsWindow(m_hWnd));  return (int) ::SendMessage(m_hWnd, TTM_GETDELAYTIME, dwDuration, 0L); }
_AFXCMN_INLINE void CToolTipCtrl::GetMargin(LPRECT lprc) const
    { ASSERT(::IsWindow(m_hWnd));  ::SendMessage(m_hWnd, TTM_GETMARGIN, 0, (LPARAM)lprc); }
_AFXCMN_INLINE int CToolTipCtrl::GetMaxTipWidth() const
    { ASSERT(::IsWindow(m_hWnd));  return (int) ::SendMessage(m_hWnd, TTM_GETMAXTIPWIDTH, 0, 0L); }
_AFXCMN_INLINE COLORREF CToolTipCtrl::GetTipBkColor() const
    { ASSERT(::IsWindow(m_hWnd));  return (COLORREF) ::SendMessage(m_hWnd, TTM_GETTIPBKCOLOR, 0, 0L); }
_AFXCMN_INLINE COLORREF CToolTipCtrl::GetTipTextColor() const
    { ASSERT(::IsWindow(m_hWnd));  return (COLORREF) ::SendMessage(m_hWnd, TTM_GETTIPTEXTCOLOR, 0, 0L); }
_AFXCMN_INLINE void CToolTipCtrl::Pop()
    { ASSERT(::IsWindow(m_hWnd));  ::SendMessage(m_hWnd, TTM_POP, 0, 0L); }
_AFXCMN_INLINE void CToolTipCtrl::SetDelayTime(DWORD dwDuration, int iTime)
    { ASSERT(::IsWindow(m_hWnd));  ::SendMessage(m_hWnd, TTM_SETDELAYTIME, dwDuration, MAKELPARAM(iTime, 0)); }
_AFXCMN_INLINE void CToolTipCtrl::SetMargin(LPRECT lprc)
    { ASSERT(::IsWindow(m_hWnd));  ::SendMessage(m_hWnd, TTM_SETMARGIN, 0, (LPARAM)lprc); }
_AFXCMN_INLINE int CToolTipCtrl::SetMaxTipWidth(int iWidth)
    { ASSERT(::IsWindow(m_hWnd));  return (int) ::SendMessage(m_hWnd, TTM_SETMAXTIPWIDTH, 0, iWidth); }
_AFXCMN_INLINE void CToolTipCtrl::SetTipBkColor(COLORREF clr)
    { ASSERT(::IsWindow(m_hWnd));  ::SendMessage(m_hWnd, TTM_SETTIPBKCOLOR, clr, 0L); }
_AFXCMN_INLINE void CToolTipCtrl::SetTipTextColor(COLORREF clr)
    { ASSERT(::IsWindow(m_hWnd));  ::SendMessage(m_hWnd, TTM_SETTIPTEXTCOLOR, clr, 0L); }
_AFXCMN_INLINE void CToolTipCtrl::Update()
    { ASSERT(::IsWindow(m_hWnd));  ::SendMessage(m_hWnd, TTM_UPDATE, 0, 0L); }
#endif // _WIN32_WCE_NO_TOOLTIPS

/////////////////////////////////////////////////////////////////////////////
#if !defined(_WIN32_WCE)
_AFXCMN_INLINE CComboBoxEx::CComboBoxEx()
    { }
_AFXCMN_INLINE DWORD CComboBoxEx::GetExtendedStyle() const
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, CBEM_GETEXTENDEDSTYLE, 0, 0); }
_AFXCMN_INLINE DWORD CComboBoxEx::SetExtendedStyle(DWORD dwExMask, DWORD dwExStyles)
    { ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, CBEM_SETEXTENDEDSTYLE, (DWORD) dwExMask, (LPARAM) dwExStyles); }
_AFXCMN_INLINE BOOL CComboBoxEx::HasEditChanged()
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, CBEM_HASEDITCHANGED, 0, 0); }
_AFXCMN_INLINE CEdit* CComboBoxEx::GetEditCtrl()
    { ASSERT(::IsWindow(m_hWnd)); return (CEdit*) CEdit::FromHandle((HWND) ::SendMessage(m_hWnd, CBEM_GETEDITCONTROL, 0, 0)); }
_AFXCMN_INLINE CComboBox* CComboBoxEx::GetComboBoxCtrl()
    { ASSERT(::IsWindow(m_hWnd)); return (CComboBox*) CComboBox::FromHandle((HWND) ::SendMessage(m_hWnd, CBEM_GETCOMBOCONTROL, 0, 0)); }
_AFXCMN_INLINE CImageList* CComboBoxEx::SetImageList(CImageList* pImageList)
    { ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, CBEM_SETIMAGELIST, 0, (LPARAM)pImageList->GetSafeHandle())); }
_AFXCMN_INLINE CImageList* CComboBoxEx::GetImageList() const
    { ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, CBEM_GETIMAGELIST, 0, 0)); }

// While CComboBoxEx derives from CComboBox, there are some
// CB_messages the underlying ComboBoxEx control doesn't support.

_AFXCMN_INLINE int CComboBoxEx::Dir(UINT attr, LPCTSTR lpszWildCard)
	{ UNUSED_ALWAYS(attr); UNUSED_ALWAYS(lpszWildCard);
		ASSERT(FALSE); return CB_ERR; }
_AFXCMN_INLINE int CComboBoxEx::FindString(int nIndexStart, LPCTSTR lpszFind) const
	{ UNUSED_ALWAYS(nIndexStart); UNUSED_ALWAYS(lpszFind);
		ASSERT(FALSE); return CB_ERR; }
_AFXCMN_INLINE int CComboBoxEx::AddString(LPCTSTR lpszString)
	{ UNUSED_ALWAYS(lpszString); ASSERT(FALSE); return CB_ERR;}
_AFXCMN_INLINE BOOL CComboBoxEx::SetEditSel(int nStartChar, int nEndChar)
	{ UNUSED_ALWAYS(nStartChar); UNUSED_ALWAYS(nEndChar);
		ASSERT(FALSE); return FALSE; }
_AFXCMN_INLINE int CComboBoxEx::InsertString(int nIndex, LPCTSTR lpszString)
	{ UNUSED_ALWAYS(nIndex); UNUSED_ALWAYS(lpszString);
		ASSERT(FALSE); return CB_ERR; }

/////////////////////////////////////////////////////////////////////////////
#endif // _WIN32_WCE


_AFXCMN_INLINE int CProgressCtrl::SetPos(int nPos)
    { ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, PBM_SETPOS, nPos, 0L); }

/////////////////////////////////////////////////////////////////////////////

#if !defined(_WIN32_WCE)
_AFXCMN_INLINE CIPAddressCtrl::CIPAddressCtrl()
    { }
_AFXCMN_INLINE void CIPAddressCtrl::ClearAddress()
    { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, IPM_CLEARADDRESS, 0, 0L); }
_AFXCMN_INLINE BOOL CIPAddressCtrl::IsBlank() const
    { ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, IPM_ISBLANK, 0, 0L); }
_AFXCMN_INLINE int CIPAddressCtrl::GetAddress(DWORD& dwAddress)
    { ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, IPM_GETADDRESS, 0, (LPARAM) &dwAddress); }
_AFXCMN_INLINE void CIPAddressCtrl::SetAddress(DWORD dwAddress)
    { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, IPM_SETADDRESS, 0, (LPARAM) dwAddress); }
_AFXCMN_INLINE void CIPAddressCtrl::SetAddress(BYTE nField0, BYTE nField1, BYTE nField2, BYTE nField3)
    { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, IPM_SETADDRESS, 0, (LPARAM) MAKEIPADDRESS(nField0, nField1, nField2, nField3)); }
_AFXCMN_INLINE void CIPAddressCtrl::SetFieldFocus(WORD nField)
    { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, IPM_SETFOCUS, (WPARAM) nField, 0); }
_AFXCMN_INLINE void CIPAddressCtrl::SetFieldRange(int nField, BYTE nLower, BYTE nUpper)
    { ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, IPM_SETRANGE, (WPARAM) nField, MAKEIPRANGE(nLower, nUpper)); }
#endif // _WIN32_WCE

/////////////////////////////////////////////////////////////////////////////

_AFXCMN_INLINE BOOL CImageList::SetImageCount(UINT uNewCount)
	{ ASSERT(m_hImageList != NULL); return ImageList_SetImageCount(m_hImageList, uNewCount); }
_AFXCMN_INLINE BOOL CImageList::Copy(int iDst, int iSrc, UINT uFlags /* = ILCF_MOVE */)
	{ ASSERT(m_hImageList != NULL); return ImageList_Copy(m_hImageList, iDst, m_hImageList, iSrc, uFlags); }
_AFXCMN_INLINE BOOL CImageList::Copy(int iDst, CImageList* pSrc, int iSrc, UINT uFlags /* = ILCF_MOVE */)
	{ ASSERT(m_hImageList != NULL); ASSERT(pSrc != NULL && (HIMAGELIST)*pSrc != NULL); return ImageList_Copy(m_hImageList, iDst, *pSrc, iSrc, uFlags); }

/////////////////////////////////////////////////////////////////////////////

#endif //_AFXCMN_INLINE

/////////////////////////////////////////////////////////////////////////////


