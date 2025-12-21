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

#include "stdafx.h"

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)

#ifdef AFX_CORE2_SEG
#pragma code_seg(AFX_CORE2_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW
/////////////////////////////////////////////////////////////////////////////
// CCeDocListDocTemplate construction/destruction

CCeDocListDocTemplate::CCeDocListDocTemplate(UINT nIDResource,
	CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass,
	CRuntimeClass* pViewClass, CString& strFilterList, CString& strFolder)
		: CSingleDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass),
		  m_strFilterList(strFilterList),
		  m_strFolder(strFolder)
{
	m_pWndDocList = new CCeDocList(this);

	//always are going to be below my documents, so copy it here
	m_crtFolder.LoadString(AFXCE_IDS_INTL_DIRMYDOCUMENTS);

	CString sAllFolders;
	sAllFolders.LoadString(AFXCE_IDS_INTL_ALLFOLDERS);
	if(m_strFolder != sAllFolders)
		m_crtFolder += CString("\\") + m_strFolder;
}

CCeDocListDocTemplate::~CCeDocListDocTemplate()
{
	if(m_pWndDocList != NULL)
		delete m_pWndDocList;
}

/////////////////////////////////////////////////////////////////////////////
// CCeDocListDocTemplate document management (a list of currently open documents)

// We want the ability to create a frame window without a document.  
// For example, this allows a PSPC 2.0 doc list to be shown first.

CFrameWnd* CCeDocListDocTemplate::CreateNewFrame
	(CDocument* pDoc /*= NULL*/, CFrameWnd* pOther /*= NULL*/)
{
	ASSERT(pDoc == NULL && pOther == NULL);
	
	CDocument* pTempDoc = CreateNewDocument();
	if(pTempDoc == NULL)
		return NULL;

	pTempDoc->m_bTempDoc = TRUE; // We want a frame, but no view.

	CFrameWnd* pFrame = CSingleDocTemplate::CreateNewFrame(pTempDoc, NULL);
	ASSERT(pTempDoc->GetFirstViewPosition() == NULL);

	CWinThread* pThread = AfxGetThread();
	if (pThread->m_pMainWnd == NULL)
	{
		// set as main frame (InitialUpdateFrame will show the window)
		pThread->m_pMainWnd = pFrame;
	}
	InitialUpdateFrame(pFrame, pTempDoc, TRUE);

	// Note: RemoveDocument causes OnEmptyFrame to be called (a useful side-effect)
	RemoveDocument(pTempDoc);
	delete pTempDoc;

	return pFrame;
}

void CCeDocListDocTemplate::RemoveDocument(CDocument* pDoc)
{
	// Note: this function is also called from CCeDocListDocTemplate::CreateNewFrame
	ASSERT(m_pOnlyDoc == pDoc);     // must be this one
	ASSERT_VALID(pDoc);

	CDocTemplate::RemoveDocument(pDoc);
	m_pOnlyDoc = NULL;

	// This is the last document to be removed (because it's an SDI app),
	// so let the frame know it is empty again.
	if((AfxGetThreadState()->m_lastSentMsg.message != WM_CLOSE) && 
		!pDoc->m_bTempDoc)
	{
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		if(pFrame != NULL)
			ShowDocList();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCeDocListDocTemplate commands

CDocument* CCeDocListDocTemplate::OpenDocumentFile(LPCTSTR lpszPathName,
	BOOL bMakeVisible)
	// if lpszPathName == NULL => create new file of this type
{
	CDocument* pDocument = NULL;
	CFrameWnd* pFrame = NULL;
	BOOL bCreated = FALSE;      // => doc and frame created
	BOOL bWasModified = FALSE;
	BOOL bCreateView = FALSE;

	if (m_pOnlyDoc != NULL)
	{
		// already have a document - reinit it
		pDocument = m_pOnlyDoc;
		if (!pDocument->SaveModified())
			return NULL;        // leave the original one

		pFrame = (CFrameWnd*)AfxGetMainWnd();
		ASSERT(pFrame != NULL);
		ASSERT_KINDOF(CFrameWnd, pFrame);
		ASSERT_VALID(pFrame);
	}
	else
	{
		// create a new document
		pDocument = CreateNewDocument();
		pFrame = (CFrameWnd*)AfxGetMainWnd();
		if(pFrame != NULL && pFrame->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
			bCreateView = TRUE;
		else
			pFrame = NULL;
		bCreated = TRUE;
	}

	if (pDocument == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return NULL;
	}
	ASSERT(pDocument == m_pOnlyDoc);

	if (pFrame == NULL)
	{
		ASSERT(bCreated);

		// create frame - set as main document frame
		BOOL bAutoDelete = pDocument->m_bAutoDelete;
		pDocument->m_bAutoDelete = FALSE;
					// don't destroy if something goes wrong
		pFrame = CSingleDocTemplate::CreateNewFrame(pDocument, NULL);
		pDocument->m_bAutoDelete = bAutoDelete;
		if (pFrame == NULL)
		{
			AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
			delete pDocument;       // explicit delete on error
			return NULL;
		}
	}
	else if(bCreateView) // We already have an empty frame, so create the view.
	{
		ASSERT(pDocument->GetFirstViewPosition() == NULL);
		ASSERT_VALID(pFrame);
		CCreateContext context;
		context.m_pCurrentFrame = NULL;
		context.m_pCurrentDoc = pDocument;
		context.m_pNewViewClass = m_pViewClass;
		context.m_pNewDocTemplate = this;
		if (pFrame->CreateView(&context, AFX_IDW_PANE_FIRST) == NULL)
			return NULL;
		pFrame->RecalcLayout();
	}

	if (lpszPathName == NULL)
	{
		// create a new document
		SetDefaultTitle(pDocument);

		// avoid creating temporary compound file when starting up invisible
		if (!bMakeVisible)
			pDocument->m_bEmbedded = TRUE;

		if (!pDocument->OnNewDocument())
		{
			// user has been alerted to what failed in OnNewDocument
			TRACE0("CDocument::OnNewDocument returned FALSE.\n");
			return NULL;
		}
	}
	else
	{
		CWaitCursor wait;

		// open an existing document
		bWasModified = pDocument->IsModified();
		pDocument->SetModifiedFlag(FALSE);  // not dirty for open

		if (!pDocument->OnOpenDocument(lpszPathName))
		{
			// user has been alerted to what failed in OnOpenDocument
			TRACE0("CDocument::OnOpenDocument returned FALSE.\n");
			if (!pDocument->IsModified())
			{
				// original document is untouched
				pDocument->SetModifiedFlag(bWasModified);
			}
			else
			{
				// we corrupted the original document
				pDocument->m_bAutoDelete = TRUE;
				pDocument->m_bTempDoc = TRUE; // treat as a temporary doc; don't change app state
				pDocument->OnCloseDocument();

				// We don't want the frame in an empty state, especially if we consider that
				// we tried to open a document in a non-empty state.
				if(m_pWndDocList->m_hWnd == NULL)
					ShowDocList();
			}
			return NULL;        // open failed
		}
		pDocument->SetPathName(lpszPathName);
	}

	CWinThread* pThread = AfxGetThread();
	if (bCreated && pThread->m_pMainWnd == NULL)
	{
		// set as main frame (InitialUpdateFrame will show the window)
		pThread->m_pMainWnd = pFrame;
	}

	InitialUpdateFrame(pFrame, pDocument, bMakeVisible);

	return pDocument;
}

void CCeDocListDocTemplate::ShowDocList()
{
	CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
	if(pFrame == NULL)
	{
		pFrame = CreateNewFrame();
		ASSERT(pFrame != NULL);
	}

	ASSERT_KINDOF(CFrameWnd, pFrame);
	ASSERT(m_pWndDocList != NULL);
	ASSERT(m_pWndDocList->m_hWnd == NULL);

	m_pWndDocList->Create(pFrame, m_strFilterList, m_strFolder);
	m_pWndDocList->SetFilterIndex(1); // one-based index to select the first filter from m_strFilterList
	m_pWndDocList->Refresh();
}

void CCeDocListDocTemplate::CloseAllDocuments(BOOL bEndSession)
{
	CSingleDocTemplate::CloseAllDocuments(bEndSession);
	if (m_pWndDocList != NULL && ::IsWindow(m_pWndDocList->m_hWnd))
		m_pWndDocList->OnClose();
}
#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

IMPLEMENT_DYNAMIC(CCeDocListDocTemplate, CSingleDocTemplate)

/////////////////////////////////////////////////////////////////////////////
#endif // _WIN32_WCE_PSPC
