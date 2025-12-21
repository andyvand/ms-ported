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

// Inlines for AFXCVIEW.H

#ifdef _AFXCVIEW_INLINE

// CListView
_AFXCVIEW_INLINE CListView::CListView() : CCtrlView(WC_LISTVIEW,
	AFX_WS_DEFAULT_VIEW)
	{ }
_AFXCVIEW_INLINE CListCtrl& CListView::GetListCtrl() const
	{ return *(CListCtrl*)this; }
_AFXCVIEW_INLINE CTreeView::CTreeView() : CCtrlView(WC_TREEVIEW,
	AFX_WS_DEFAULT_VIEW)
	{ }
_AFXCVIEW_INLINE CTreeCtrl& CTreeView::GetTreeCtrl() const
	{ return *(CTreeCtrl*)this; }

#endif //_AFXCVIEW_INLINE

/////////////////////////////////////////////////////////////////////////////
