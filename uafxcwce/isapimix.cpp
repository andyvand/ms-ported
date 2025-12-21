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
#include <afxisapi.h>

CHttpServerContext& CHttpServerContext::operator<<(const CLongBinary& blob)
{
	ISAPIASSERT(m_pStream != NULL);
	if (m_pStream != NULL) *m_pStream << blob;
		return *this;
}

CHttpServerContext& CHttpServerContext::operator<<(const CByteArray& array)
{
	ISAPIASSERT(m_pStream != NULL);
	if (m_pStream != NULL) *m_pStream << array;
		return *this;
}

CHtmlStream& CHtmlStream::operator<<(const CByteArray& array)
{
	int nSize = array.GetSize();
	if (nSize > 0)
	{
		const BYTE* pStart = array.GetData();
		if (pStart != NULL)
			Write(pStart, nSize);
	}

	return *this;
}

CHtmlStream& CHtmlStream::operator<<(const CLongBinary& blob)
{
	if (blob.m_dwDataLength > 0 && blob.m_hData != NULL)
	{
		LPVOID lpData = GlobalLock(blob.m_hData);
		if (lpData != NULL)
		{
			Write(lpData, blob.m_dwDataLength);
			GlobalUnlock(blob.m_hData);
		}
	}

	return *this;
}
