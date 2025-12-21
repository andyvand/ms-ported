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
#include <math.h>

static FONTDESC s_fdDefault =
{
    sizeof(FONTDESC),
    OLESTR("Tahoma"),
    {8 * 10000, 0},
    400,
    DEFAULT_CHARSET,
    FALSE,
    FALSE,
    FALSE
};

HRESULT CharSetOfFace(HDC hdc, OLECHAR *szFaceName, WORD *pwCharset);
double RoundFloatDiv(double numer, double denom);
long RoundDiv(long numer, long denom);


int  CCeFont::s_cFcr  = 0;		// Count of records in the font cache
FCR *CCeFont::s_rgfcr = NULL;	// Array of font cache records (the font cache)
BOOL CCeFont::s_fInit = FALSE;	// Has font cache been initialized?
HDC  CCeFont::m_hdc   = NULL;

CCeFont::CCeFont(void)
    :CUnknown(NULL)
{
    m_hfont = NULL;
    m_nFcr  = -1;
    m_fr.cySize.Lo  = 80000; // Default to 8 point
    m_fr.cySize.Hi  = 0;
    m_fr.cyLogical  = 1;
    m_fr.cyHimetric = 1;
    m_fr.sCharset   = DEFAULT_CHARSET;
    m_fr.sWeight    = FW_DONTCARE;
    m_fr.bFlags     = 0;
    m_fr.rgchFace[0] = 0;
    m_fNameDirty    = FALSE;
    m_fCharSetDirty = FALSE;
	m_bModified     = FALSE;
}

CCeFont::~CCeFont(void)
{
    if (m_nFcr != -1) 
    {
        ASSERT(s_rgfcr[m_nFcr].cRefs > 0);
        s_rgfcr[m_nFcr].cRefs--;
    }
}

//
// Return the font name
//
STDMETHODIMP CCeFont::get_Name(BSTR* pbstrName)
{
    if (!pbstrName)
        {
        return E_INVALIDARG;
        }

    HRESULT hr = NOERROR;

    hr = UpdateFont();
    if (FAILED(hr)) 
        {
        return hr;
        }
    
    Lock();

    *pbstrName = SysAllocString(m_fr.rgchFace);
    if (!*pbstrName)
        {
        hr = E_OUTOFMEMORY;
        goto LReturn;
        }

LReturn:
    Unlock();

    return hr;
}

//
// Answer if the given string is a valid name
//
HRESULT CCeFont::ChkName(LPOLESTR szName)
{
    if (!szName || (::lstrlenW(szName) >= LF_FACESIZE))
        {
        return CTL_E_INVALIDPROPERTYVALUE;
        }

    return NOERROR;
}

//
// Set the font name
//
STDMETHODIMP CCeFont::put_Name(BSTR bstrName)
{
    HRESULT hr;

    hr = ChkName(bstrName);
    if (FAILED(hr))
        {
        return hr;
        }

    if (::lstrcmpW(m_fr.rgchFace, bstrName)) 
        {
        ::wcsncpy(m_fr.rgchFace, bstrName, LF_FACESIZE);
        m_fNameDirty = TRUE;
		DiscardFont(TRUE, DISPID_FONT_NAME);
		UpdateFont();
        }

    return NOERROR;
}

STDMETHODIMP CCeFont::get_Size(CY *pcySize)
{
    if (!pcySize)
        {
        return E_INVALIDARG;
        }

    HRESULT hr;

    hr = UpdateFont();
    if (FAILED(hr)) 
        {
        return hr;
        }
    
    Lock();

    pcySize->Hi = m_fr.cySize.Hi;
    pcySize->Lo = m_fr.cySize.Lo;
    
    Unlock();

    return NOERROR;
}

STDMETHODIMP CCeFont::put_Size(CY cySize)
{
    unsigned long ulHeight;

    if (cySize.Hi < 0 || (cySize.Lo == 0 && cySize.Hi == 0))
        {
        return CTL_E_INVALIDPROPERTYVALUE;
        }
    //
    // currently only accept values from 1 to 65535 (what VB used to accept)
    //
    if (cySize.Hi != 0 || cySize.Lo > 655350000)
        {
        return CTL_E_OVERFLOW;
        }
    //
    // have to scale cySize.Lo back down to avoid overflow when
    // multiplying by GetDeviceCaps()
    //
    ulHeight = ((cySize.Lo / 10000) * GetDeviceCaps(Hdc(), LOGPIXELSY)) / 72;
    if (ulHeight > 32767)
        {
        return ResultFromScode(CTL_E_OVERFLOW);
        }
    
    Lock();
    if (cySize.Lo == m_fr.cySize.Lo) 
        {
        goto LReturn;
        }
    
    m_fr.cySize = cySize;
	DiscardFont(TRUE, DISPID_FONT_SIZE);
	UpdateFont();

LReturn:
    Unlock();

    return NOERROR;
}

STDMETHODIMP CCeFont::get_Bold(BOOL *pfBold)
{
    if (!pfBold)
        {
        return E_INVALIDARG;
        }

    HRESULT hr;

    hr = UpdateFont();
    if (FAILED(hr)) 
        {
        return hr;
        }
    
    Lock();

    *pfBold = (m_fr.sWeight > ((FW_MEDIUM + FW_SEMIBOLD) / 2)) ? VARIANT_TRUE : VARIANT_FALSE;

    Unlock();

    return NOERROR;
}

STDMETHODIMP CCeFont::put_Bold(BOOL bold)
{
    return put_Weight((short)(bold ? FW_BOLD : FW_NORMAL));
}

STDMETHODIMP CCeFont::get_Italic(BOOL *pfItalic)
{
    if (!pfItalic)
        {
        return E_INVALIDARG;
        }

    HRESULT hr;

    hr = UpdateFont();
    if (FAILED(hr)) 
        {
        return hr;
        }
    
    Lock();

    *pfItalic = (m_fr.fItalic != 0) ? VARIANT_TRUE : VARIANT_FALSE;

    Unlock();

    return NOERROR;
}

STDMETHODIMP CCeFont::put_Italic(BOOL fItalic)
{
    Lock();

    if (m_fr.fItalic != fItalic) 
        {
        m_fr.fItalic = fItalic;
		DiscardFont(TRUE, DISPID_FONT_ITALIC);
		UpdateFont();
        }
    
    Unlock();

    return NOERROR;
}

STDMETHODIMP CCeFont::get_Underline(BOOL *pfUnderline)
{
    if (!pfUnderline)
        {
        return E_INVALIDARG;
        } 

    HRESULT hr;

    hr = UpdateFont();
    if (FAILED(hr)) 
        {
        return hr;
        }
    
    Lock();

    *pfUnderline = (m_fr.fUnderline != 0) ? VARIANT_TRUE : VARIANT_FALSE;
    
    Unlock();

    return NOERROR;
}

STDMETHODIMP CCeFont::put_Underline(BOOL fUnderline)
{
    Lock();

    if (m_fr.fUnderline != fUnderline) 
        {
        m_fr.fUnderline = fUnderline;
		DiscardFont(TRUE, DISPID_FONT_UNDER);
		UpdateFont();
        }

    Unlock();

    return NOERROR;
}

STDMETHODIMP CCeFont::get_Strikethrough(BOOL* pfStrikethrough)
{
    if (!pfStrikethrough)
        {
        return E_INVALIDARG;
        }

    HRESULT hr;
  
    hr = UpdateFont();
    if (FAILED(hr)) 
        {
        return hr;
        }
    
    Lock();

    *pfStrikethrough = (m_fr.fStrikethrough != 0) ? VARIANT_TRUE : VARIANT_FALSE;

    Unlock();

    return NOERROR;
}

STDMETHODIMP CCeFont::put_Strikethrough(BOOL fStrikethrough)
{
    Lock();

    if (m_fr.fStrikethrough != fStrikethrough) 
        {
        m_fr.fStrikethrough = fStrikethrough;
		DiscardFont(TRUE, DISPID_FONT_STRIKE);
		UpdateFont();
        } 

    Unlock();

    return NOERROR;
}

STDMETHODIMP CCeFont::get_Weight(short* psWeight)
{
    if (!psWeight)
        {
        return E_INVALIDARG;
        }

    HRESULT hr;
      
    hr = UpdateFont();
    if (FAILED(hr)) 
        {
        return hr;
        }
    
    Lock();

    *psWeight = m_fr.sWeight;

    Unlock();

    return NOERROR;
}

STDMETHODIMP CCeFont::put_Weight(short sWeight)
{
    if (sWeight < 0 || sWeight > 1000)
      return CTL_E_INVALIDPROPERTYVALUE;
    
    Lock();

    if (m_fr.sWeight != sWeight) 
        {
        m_fr.sWeight = sWeight;
		DiscardFont(TRUE, DISPID_FONT_WEIGHT);
		UpdateFont();
        }

    Unlock();

    return NOERROR;
}

STDMETHODIMP CCeFont::get_Charset(short *psCharset)
{
    if (!psCharset)
        {
        return E_INVALIDARG;
        }

    HRESULT hr;
      
    hr = UpdateFont();
    if (FAILED(hr)) 
        {
        return hr;
        }
    
    Lock();

    *psCharset = m_fr.sCharset;

    Unlock();

    return NOERROR;
}

STDMETHODIMP CCeFont::put_Charset(short sCharset)
{
    if (sCharset > 255 || sCharset < 0)
        {
        return CTL_E_INVALIDPROPERTYVALUE;
        }
    
    Lock();

    if (m_fr.sCharset != sCharset) 
        {
        m_fr.sCharset = sCharset;
        m_fCharSetDirty = TRUE;
		DiscardFont(TRUE, DISPID_FONT_CHARSET);
		UpdateFont();
        }
    
    Unlock();

    return NOERROR;
}

STDMETHODIMP CCeFont::get_hFont(HFONT *phfont)
{
    if (!phfont)
        {
        return E_INVALIDARG;
        }
      
    HRESULT hr;

    hr = UpdateFont();
    if (FAILED(hr)) 
        {
        return hr;
        }
    
    Lock();

    *phfont = m_hfont;

    Unlock();

    return NOERROR;
}

STDMETHODIMP CCeFont::Clone(IFont **ppfont)
{
    if (!ppfont)
        {
        return E_INVALIDARG;
        }

    HRESULT hr;

    hr = UpdateFont();
    if (FAILED(hr)) 
        {
        return hr;
        }

    CCeFont *pfontClone;
    
    pfontClone = new CCeFont();
    if (!pfontClone) 
        {
        return E_OUTOFMEMORY;
        }
    
    Lock();

    pfontClone->m_hfont = m_hfont;
    WCE_FCTN(memcpy)(&pfontClone->m_fr, &m_fr, sizeof(FR));

    if ((pfontClone->m_nFcr = m_nFcr) != -1)
        {
        s_rgfcr[m_nFcr].cRefs++;
        }
    
    Unlock();

    hr = pfontClone->QueryInterface(IID_IFont, (PVOID *) ppfont);
    if (FAILED(hr))
        {
        delete pfontClone;
        }
 
    return hr;
}

STDMETHODIMP CCeFont::IsEqual(IFont *pfontOther)
{
    return E_NOTIMPL;
}

STDMETHODIMP CCeFont::SetRatio(long cyLogical, long cyHimetric)
{
    if (cyLogical == 0 || cyHimetric == 0)
        {
        return E_FAIL;
        }
    
    Lock();

    if (cyLogical != m_fr.cyLogical || cyHimetric != m_fr.cyHimetric) 
        {
        m_fr.cyLogical = cyLogical;
        m_fr.cyHimetric = cyHimetric;
		DiscardFont(FALSE, DISPID_UNKNOWN);
		UpdateFont();
        }
    
    Unlock();

    return NOERROR;
}

STDMETHODIMP CCeFont::QueryTextMetrics(LPTEXTMETRICOLE ptm)
{
    if (!ptm)
        {
        return E_INVALIDARG;
        }

    HRESULT hr;


    hr = UpdateFont();
    if (FAILED(hr))
        {
        return hr;
        }
    
    Lock();

    WCE_FCTN(memcpy)(ptm, &s_rgfcr[m_nFcr].tm, sizeof(TEXTMETRICOLE));

    Unlock();

    return NOERROR;
}

//
// Find the font in the font cache and up its ref count
//
STDMETHODIMP CCeFont::AddRefHfont(HFONT hfont)
{
    if (!hfont)
        {
        return E_INVALIDARG;
        }

    HRESULT hr;
    FCR *pfcr, *pfcrEnd;

    hr = S_FALSE;

    Lock();

    pfcrEnd = &s_rgfcr[s_cFcr];
    // 
    // Find the font in the cache
    //
    for (pfcr = s_rgfcr; pfcr < pfcrEnd; ++pfcr) 
        {
        if (pfcr->hfont == hfont) 
            {
	        pfcr->cRefs++;
	        hr = NOERROR;
	        break;
            }
        }

    Unlock();

    return hr;
}

//
// Reduce the ref count on the given hfont by one.
//
// Note, we don't actually delete the font when the ref count goes
// to 0.  This will happen if the cache record is reused by another
// font, or when the last font object is released.
//
STDMETHODIMP CCeFont::ReleaseHfont(HFONT hfont)
{
    if (!hfont)
        {
        return E_INVALIDARG;
        }

    HRESULT hr;
    FCR *pfcr, *pfcrEnd;

    hr = S_FALSE;

    Lock();

    pfcrEnd = &s_rgfcr[s_cFcr];
    //
    // Find the font by looping through the cache
    //
    for (pfcr = s_rgfcr; pfcr < pfcrEnd; ++pfcr) 
        {
        if (pfcr->hfont == hfont) 
            {
	        if (pfcr->cRefs > 1) 
                {
	            pfcr->cRefs--;
	            hr = NOERROR;
	            } 
            else
                {
	            hr = S_FALSE;	// We always have 1 reference by the font obj
                }
	        break;
            }
        }

    Unlock();

    return hr;
}

STDMETHODIMP CCeFont::SetHdc(HDC hdc)
{
    return E_NOTIMPL;
}

//
// Return system information about the given font.
//
HRESULT CCeFont::QueryFontInfo(HFONT hfont, TEXTMETRICOLE *ptm, OLECHAR rgchFace[LF_FACESIZE])
{
    HRESULT hr = E_FAIL;
    HDC hdc = Hdc();
    HFONT hfontPrev = (HFONT) ::SelectObject(hdc, (HGDIOBJ) hfont);

    if (!FontGetTextMetrics(hdc, ptm))
        {
        goto LReturn;
        }

    if (!FontGetTextFace(hdc, LF_FACESIZE, rgchFace))
        {
        goto LReturn;
        }
    
    hr = NOERROR;

LReturn:
    ::SelectObject(hdc, (HGDIOBJ) hfontPrev);

    return hr;
}

//
// Make sure that we have ahold of an HFONT that reflects corresponds
// to the current font settings.
//
HRESULT CCeFont::UpdateFont()
{
    FCR *pfcr;
    HRESULT hr;
    
    Lock();

    if (m_nFcr != -1)
        {
        hr = NOERROR; // got one!
        goto LReturn;
        }
    //
    // Look for a match in the font cache
    //
    hr = FindMatchingFont(&m_nFcr);
    ASSERT(SUCCEEDED(hr)); // Should return S_OK or S_FALSE

    if (hr == NOERROR) 
        {
        //
        // Found a match in the cache
        //
        pfcr = &s_rgfcr[m_nFcr];
        pfcr->cRefs++;
        m_hfont = pfcr->hfont;
        //
        // Update our state to reflect the cache entry we matched
        //
        WCE_FCTN(memcpy)(&m_fr, &pfcr->frActual, sizeof(FR));
        goto LDone;
        }

    HFONT hfont;
    TEXTMETRICOLE tm;
    OLECHAR rgchFace[LF_FACESIZE];
    //
    // Realize a new font, and stuff it into the cache
    //
    hr = NewFcr(&m_nFcr); // allocate a new cache slot
    if (FAILED(hr))
        {
        goto LReturn;
        }
    //
    // Create a font that matches our FR
    //
    hr = CreateMatchingFont(&hfont);
    //
    // If this failed, try to keep going with the old hfont.
    //
    if (FAILED(hr)) 
        {
	    if (!m_hfont) 
            {
	        m_nFcr = -1;
	        goto LReturn;
	        }
	    
        hfont = m_hfont;
	    hr = NOERROR;
        }
    //
    // The create succeeded, make sure we like what we got...
    //
    QueryFontInfo(hfont, &tm, rgchFace);
    //
    // If the font name we get out is different than the one we requested
    // then we may have had a problem with the charset. So enumerate all
    // fonts to see if there is a font that matches in name, but has a
    // different charset, and try to use it instead.
    //
    // NOTE, This is a very expensive operation, so we avoid it in the
    // mainline case. We also do work to avoid unless we know it was a
    // name or charset change that has caused us to realize a new font.
    //
    // NOTE: We should never execute this when realizing a font after
    // loading from binary.
    //
    WORD wCharset;
    HFONT hfontWithCharset;

    if (m_fNameDirty || m_fCharSetDirty) 
        {
	    if (::lstrcmpiW(m_fr.rgchFace, rgchFace)) 
            {
	        if (SUCCEEDED(CharSetOfFace(Hdc(), m_fr.rgchFace, &wCharset))) 
                {
	            if (m_fr.sCharset != wCharset) 
                    {
	                m_fr.sCharset = wCharset;
	                if (SUCCEEDED(CreateMatchingFont(&hfontWithCharset))) 
                        {
                        //
		                // Replace with the better match
                        //
		                ::DeleteObject((HGDIOBJ) hfont);
		                hfont = hfontWithCharset;
		                QueryFontInfo(hfont, &tm, rgchFace);
	                    }
	                }
	            }
	        }
        }
    //
    // Add it to the font cache
    //
    pfcr = &s_rgfcr[m_nFcr];
    pfcr->cRefs++;
    pfcr->hfont = hfont;
    //
    // Record the font we requested
    //
    WCE_FCTN(memcpy)(&pfcr->frRequest, &m_fr, sizeof(FR));
    //
    // Update our state based on the font we actually mapped to
    //
    UpdateFr(hfont, &tm, rgchFace);
    WCE_FCTN(memcpy)(&pfcr->frActual, &m_fr, sizeof(FR));

LDone:
    m_fNameDirty = FALSE;
    m_fCharSetDirty = FALSE;

    hr = NOERROR;

LReturn:
    Unlock();

    return hr;
}

//
// Refresh our current FR based on the given HFONT.
//
HRESULT CCeFont::UpdateFr(HFONT hfont, TEXTMETRICOLE *ptm, OLECHAR rgchFace[LF_FACESIZE])
{
    double n, d;

    ASSERT(m_nFcr != -1);
    
    Lock();

    m_hfont = hfont;
    //
    // Cache the metrics in the FCR
    //
    WCE_FCTN(memcpy)(&s_rgfcr[m_nFcr].tm, ptm, sizeof(*ptm));

    m_fr.fItalic        = ptm->tmItalic;
    m_fr.fUnderline     = ptm->tmUnderlined;
    m_fr.fStrikethrough = ptm->tmStruckOut;
    m_fr.sWeight        = (SHORT)ptm->tmWeight;
    m_fr.sCharset       = (SHORT)ptm->tmCharSet;
    //
    // Convert font height in logical pixels back to our
    // internal representation, which is a currency rep of points.
    //
    n = (double)(ptm->tmHeight - ptm->tmInternalLeading);

    if (m_fr.cyLogical == 1 && m_fr.cyHimetric == 1) 
        {
        d = (double)GetDeviceCaps(Hdc(), LOGPIXELSY);
        m_fr.cySize.Lo = (ULONG)RoundFloatDiv(n * 720000.0, d);
        } 
    else 
        {
        n = RoundFloatDiv(n * 72000.0, 254.0) * (double)m_fr.cyHimetric;
        m_fr.cySize.Lo = (ULONG)RoundFloatDiv(n, (double)m_fr.cyLogical);
        m_fr.cySize.Lo = RoundDiv(m_fr.cySize.Lo, 100) * 100;
        }

    ASSERT(m_fr.cySize.Lo > 0);
    m_fr.cySize.Hi = 0;

    ::lstrcpyW(m_fr.rgchFace, rgchFace);
    
    Unlock();

    return NOERROR;
}

//
// Discard our current font handle, reducing the ref count by one.
//
// Note: this routine doesn't actually delete the HFONT if the ref
// count goes to zero.  This will happen if the font cache record
// is reused by another object, or when the cache itself is released.
//
HRESULT CCeFont::DiscardFont(BOOL fNotify, DISPID dispidChanged)
{
    Lock();

    if (m_nFcr != -1) 
        {
        ASSERT(s_rgfcr[m_nFcr].cRefs > 0);
        ASSERT(s_rgfcr[m_nFcr].hfont == m_hfont);
        s_rgfcr[m_nFcr].cRefs--;
        m_nFcr = -1;
        }

    Unlock();

    if (fNotify) 
        {
        //
        // Notification not currently supported!
        //
        }

    return NOERROR;
}

//
// Answers TRUE if the 2 given FR structs match.
//
inline BOOL FrEq(FR *pfr1, FR *pfr2)
{
    //
    // NOTE: these should be orderd beginning with fields most likely to differ
    //
    // CONSIDER: chould keep a CRC-type field at the beginning of
    // CONSIDER: the struct to speed up detection of non-matches...
    //
    if (pfr1->cySize.Lo != pfr2->cySize.Lo)
        {
        return FALSE;
        }
    if (::lstrcmpiW(pfr1->rgchFace, pfr2->rgchFace))
        {
        return FALSE;
        }
    if (pfr1->bFlags != pfr2->bFlags)
        {
        return FALSE;
        }
    if (pfr1->sWeight != pfr2->sWeight)
        {
        return FALSE;
        }
    if (pfr1->sCharset != pfr2->sCharset)
        {
        return FALSE;
        }
    if (pfr1->cyLogical != pfr2->cyLogical)
        {
        return FALSE;
        }
    if (pfr1->cyHimetric != pfr2->cyHimetric)
        {
        return FALSE;
        }
    if (pfr1->cySize.Hi != pfr2->cySize.Hi)
        {
        return FALSE;
        }

    return TRUE;
}

//
// Try to locate a matching font in the font cache.
//
// NOTE: access synchronized by the caller!
//
HRESULT CCeFont::FindMatchingFont(int *pnFcr)
{
    int i;
    FCR *pfcr;

    for (i = 0, pfcr = s_rgfcr; i < s_cFcr; ++i, ++pfcr) 
        {
        if (!pfcr->hfont)
            {
	        continue; // skip empty entry
            }
        if (FrEq(&pfcr->frRequest, &m_fr) || FrEq(&pfcr->frActual, &m_fr)) 
            {
	        *pnFcr = i;
	        return NOERROR; // Found one!
            }
        }

    return S_FALSE;
}

//
// Realize a font that matches our current FR description.
//
// NOTE: access synchronized by the caller!
//
HRESULT CCeFont::CreateMatchingFont(HFONT *phfont)
{
    double n;
    LOGFONT lf;
    HFONT hfont;
    //
    //  Font properties we ignore...
    //
    lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    lf.lfEscapement = 0;
    lf.lfOrientation = 0;
    lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfQuality = DEFAULT_QUALITY;
    lf.lfWidth = 0;
    //
    // properties that we dont ignore...
    //
    ::lstrcpyW(lf.lfFaceName, m_fr.rgchFace);
    
    lf.lfWeight    = m_fr.sWeight;
    lf.lfCharSet   = (BYTE)m_fr.sCharset;
    lf.lfItalic    = m_fr.fItalic;
    lf.lfUnderline = m_fr.fUnderline;
    lf.lfStrikeOut = m_fr.fStrikethrough;

	if (m_fr.cyLogical == 1 && m_fr.cyHimetric == 1) 
	{
		n = (double)m_fr.cySize.Lo * (double)GetDeviceCaps(Hdc(), LOGPIXELSY);
		lf.lfHeight = (int)RoundFloatDiv(n, 720000.0);
	} 
	else 
	{
		n = RoundFloatDiv((double)m_fr.cySize.Lo * 254.0, 72000.0);
		lf.lfHeight = (int)RoundFloatDiv(n * (double)m_fr.cyLogical,
			(double)m_fr.cyHimetric);
	}

	if (lf.lfHeight <= 0) 
	{
		ASSERT(lf.lfHeight == 0);
		lf.lfHeight = 1;
	}

    lf.lfHeight = -lf.lfHeight; //  Use character height

    hfont = ::CreateFontIndirect(&lf);
    if (!hfont)
        {
        return HRESULT_FROM_WIN32(GetLastError());
        }

    *phfont = hfont;

    return NOERROR;
}

//
// Return an unused font cache record.  This may require growing
// the font cache.
//
// NOTE: access synchronized by the caller!
//
HRESULT CCeFont::NewFcr(int *pnFcr)
{
    int i;
    FCR *pfcr;
    //
    // Look for an unused cache slot...
    //
    for (i = 0, pfcr = s_rgfcr; i < s_cFcr; ++i, ++pfcr) 
        {
        if (pfcr->cRefs == 0) 
            {
	        if (pfcr->hfont)
                {
	            ::DeleteObject((HGDIOBJ) pfcr->hfont);
				memset(pfcr, 0, sizeof(FCR));
	            *pnFcr = i;
	            return NOERROR;
                }
    // Grow the cache by a few
            }
        }
    //
    //
    pfcr = (FCR*)wce_CoTaskMemRealloc(s_rgfcr, sizeof(FCR) * s_cFcr,
		                              sizeof(FCR) * (s_cFcr+FONT_CACHE_GROWBY));
	if(pfcr == NULL)
        {
        return E_OUTOFMEMORY;
        }
    //
    // Zero out thew new nodes
    //
	memset(&pfcr[s_cFcr], 0, sizeof(FCR) * FONT_CACHE_GROWBY);
    //
    // Take the next available slot
    //
    *pnFcr = s_cFcr;

    s_cFcr += FONT_CACHE_GROWBY;
    s_rgfcr = pfcr;

    return NOERROR;
}

//
// CUnknown
//
STDMETHODIMP CCeFont::InternalQueryInterface(REFIID riid, PVOID *ppv)
{
	if ((IID_IFont == riid) || (IID_IUnknown == riid))
	{
		return GetInterface((IFont *) this, ppv);		
	}

	if (IID_IDispatch == riid)
	{
		return GetInterface((IFontDisp *) this, ppv);
	}

	if(IID_IFontDisp == riid)
	{
		return GetInterface((IFontDisp *) this, ppv);
	}

	if (IID_IPersistStream == riid)
	{
		return GetInterface((IPersistStream *) this, ppv);
	}

	if (IID_IPersistStreamInit == riid)
	{
		return GetInterface((IPersistStreamInit *) this, ppv);
	}

	if (IID_IPersistPropertyBag == riid)
	{
		return GetInterface((IPersistPropertyBag *) this, ppv);
	}

	return CUnknown::InternalQueryInterface(riid, ppv);
}

const int NUM_FONTPROPS=8;

struct tag_fontprop
{
	OLECHAR* szName;
	OLECHAR* szPersistName;
	DISPID dispid;
} szFontProps[NUM_FONTPROPS] = 
{
	{OLESTR("Name"),          OLESTR("OLECE_Name"),          DISPID_FONT_NAME},
	{OLESTR("Size"),          OLESTR("OLECE_Size"),          DISPID_FONT_SIZE},  
	{OLESTR("Bold"),          OLESTR("OLECE_Bold"),          DISPID_FONT_BOLD},
	{OLESTR("Italic"),        OLESTR("OLECE_Italic"),        DISPID_FONT_ITALIC},
	{OLESTR("Underline"),     OLESTR("OLECE_Underline"),     DISPID_FONT_UNDER},
	{OLESTR("Strikethrough"), OLESTR("OLECE_Strikethrough"), DISPID_FONT_STRIKE},
	{OLESTR("Weight"),        OLESTR("OLECE_Weight"),        DISPID_FONT_WEIGHT},
	{OLESTR("Charset"),       OLESTR("OLECE_Charset"),       DISPID_FONT_CHARSET}
};


HRESULT CCeFont::GetTypeInfoCount(UINT *pctInfo)			
{													
	return E_NOTIMPL;									
}													

HRESULT CCeFont::GetTypeInfo(UINT itinfo, LCID lcid,		
						ITypeInfo **pptinfo)			
{													
	return E_NOTIMPL;								
}													

HRESULT CCeFont::GetIDsOfNames(REFIID riid,				
						OLECHAR **rgszNames,		
						UINT	cNames,				
						LCID	lcid,				
						DISPID	*rgdispids)							
{			
	// check arguments
	if (riid != IID_NULL)
		return DISP_E_UNKNOWNINTERFACE;

	// find a match for the property name (assume that no method names will be requested)
	rgdispids[0] = DISPID_UNKNOWN;
	for(UINT n=0; n < NUM_FONTPROPS; n++)
	{
		if(lstrcmpi(*rgszNames, szFontProps[n].szName) == 0)
		{
			rgdispids[0] = szFontProps[n].dispid;
			break;
		}
	}
	if(rgdispids[0] == DISPID_UNKNOWN)
		return DISP_E_UNKNOWNNAME;

	// argument names are always DISPID_UNKNOWN (for this implementation)
	for (n=1; n < cNames; n++)
		rgdispids[n] = DISPID_UNKNOWN;

	return S_OK;									
}	

HRESULT CCeFont::Invoke(DISPID dispid,						
					REFIID riid, LCID, WORD wFlags,
					DISPPARAMS *pdispparams,			
					VARIANT *pVarResult, EXCEPINFO *,
					UINT *puArgErr)						
{	
	if (riid != IID_NULL)
		return DISP_E_UNKNOWNINTERFACE;

	if(wFlags & (DISPATCH_PROPERTYGET | DISPATCH_METHOD))
	{
		if(pVarResult == NULL)
			return DISP_E_PARAMNOTOPTIONAL;

		AfxVariantInit(pVarResult);	
	
		switch(dispid)
		{
			case DISPID_FONT_NAME:
				pVarResult->vt = VT_BSTR;
				return get_Name(&pVarResult->bstrVal);
			break;
			case DISPID_FONT_SIZE:
				pVarResult->vt = VT_CY;
				return get_Size(&pVarResult->cyVal);
			break;
			case DISPID_FONT_BOLD:
				pVarResult->vt = VT_BOOL;
				return get_Bold((BOOL*)&pVarResult->boolVal);
			break;
			case DISPID_FONT_ITALIC:
				pVarResult->vt = VT_BOOL;
				return get_Italic((BOOL*)&pVarResult->boolVal);
			break;
			case DISPID_FONT_UNDER:
				pVarResult->vt = VT_BOOL;
				return get_Underline((BOOL*)&pVarResult->boolVal);
			break;
			case DISPID_FONT_STRIKE:
				pVarResult->vt = VT_BOOL;
				return get_Strikethrough((BOOL*)&pVarResult->boolVal);
			break;
			case DISPID_FONT_WEIGHT:
				pVarResult->vt = VT_I2;
				return get_Weight(&pVarResult->iVal);
			break;
			case DISPID_FONT_CHARSET:
				pVarResult->vt = VT_I2;
				return get_Charset(&pVarResult->iVal);
			break;
			default:
				return DISP_E_MEMBERNOTFOUND;
			break;
		}
	}
	else if(wFlags & DISPATCH_PROPERTYPUT)
	{
		// Crack/validate pdispparams
		if(pdispparams->cArgs != 1) // exactly one argument required
			return DISP_E_BADPARAMCOUNT;
	
		if(pdispparams->rgvarg == NULL)
			return DISP_E_PARAMNOTOPTIONAL;

		if(puArgErr != NULL)
			*puArgErr = 0; // if rgvarg->vt (the first type) is the wrong type, then set the index to 0

		VARIANTARG* rgvarg = pdispparams->rgvarg;

		switch(dispid)
		{
			case DISPID_FONT_NAME:
				if(rgvarg->vt != VT_BSTR)
					return DISP_E_TYPEMISMATCH;
				return put_Name(rgvarg->bstrVal);
			break;
			case DISPID_FONT_SIZE:
				if(rgvarg->vt == VT_CY)
				{
					return put_Size(rgvarg->cyVal);
				}
				else if(rgvarg->vt == VT_I2)
				{
					CY cy = {rgvarg->iVal * 10000, 0};
					return put_Size(cy);
				}
				else if(rgvarg->vt == VT_I4)
				{
					CY cy = {rgvarg->lVal * 10000, 0};
					return put_Size(cy);
				}
				else if(rgvarg->vt == VT_R4)
				{
					CY cy = {(unsigned long)(rgvarg->fltVal * 10000), 0};
					return put_Size(cy);
				}
				else if(rgvarg->vt == VT_R8)
				{
					CY cy = {(unsigned long)(rgvarg->dblVal * 10000), 0};
					return put_Size(cy);
				}
				else
				{
					return DISP_E_TYPEMISMATCH;
				}
			break;
			case DISPID_FONT_BOLD:
				if(rgvarg->vt != VT_BOOL)
					return DISP_E_TYPEMISMATCH;
				return put_Bold((BOOL)rgvarg->boolVal);
			break;
			case DISPID_FONT_ITALIC:
				if(rgvarg->vt != VT_BOOL)
					return DISP_E_TYPEMISMATCH;
				return put_Italic((BOOL)rgvarg->boolVal);
			break;
			case DISPID_FONT_UNDER:
				if(rgvarg->vt != VT_BOOL)
					return DISP_E_TYPEMISMATCH;
				return put_Underline((BOOL)rgvarg->boolVal);
			break;
			case DISPID_FONT_STRIKE:
				if(rgvarg->vt != VT_BOOL)
					return DISP_E_TYPEMISMATCH;
				return put_Strikethrough((BOOL)rgvarg->boolVal);
			break;
			case DISPID_FONT_WEIGHT:
				if(rgvarg->vt != VT_I2)
					return DISP_E_TYPEMISMATCH;
				return put_Weight(rgvarg->iVal);
			break;
			case DISPID_FONT_CHARSET:
				if(rgvarg->vt != VT_I2)
					return DISP_E_TYPEMISMATCH;
				return put_Charset(rgvarg->iVal);
			break;
			default:
				return DISP_E_MEMBERNOTFOUND;
			break;
		}
	}

	return E_NOTIMPL;									
}	

//
// Initialize the font cache
//
// NOTE: access synchronized by the caller!
//
HRESULT CCeFont::InitFontObject()
{
    // Allocate the font cache, start it out with a few slots
    CCeFont::s_rgfcr = (FCR*)::CoTaskMemAlloc(sizeof(FCR) * FONT_CACHE_GROWBY);
    if (CCeFont::s_rgfcr == NULL)
        {
        return E_OUTOFMEMORY;
        }

    memset(s_rgfcr, 0, sizeof(FCR) * FONT_CACHE_GROWBY);

    CCeFont::s_cFcr = FONT_CACHE_GROWBY;

    return NOERROR;
}

//
// Un-initialize the font cache
//
// NOTE: access synchronized by the caller!
//
HRESULT CCeFont::UninitFontObject()
{
    FCR *pfcr, *pfcrEnd;
    //
    // Delete any fonts remaining in the cache
    pfcrEnd = &s_rgfcr[s_cFcr];
    for (pfcr = s_rgfcr; pfcr < pfcrEnd; ++pfcr) 
        {
        if (pfcr->hfont != NULL) 
            {
	        ::DeleteObject((HGDIOBJ) pfcr->hfont);
            }
        }
    //
    // Free the font cache
    //
    s_cFcr = 0;
    ::CoTaskMemFree(s_rgfcr);

    CCeFont::s_fInit = FALSE;

    return NOERROR;
}

//
// Routine used by the class object
//
HRESULT CCeFont::CreateFont(LPFONTDESC pfd, REFIID riid, PVOID *ppv)
{
	if (!ppv)
	{
		return E_INVALIDARG;
	}
	
	CCeFont *pfont;
	HRESULT hr;
	
	if (!m_hdc)
	{
		BOOL bCreated = FALSE;
		HWND hWnd = ::GetForegroundWindow(); // any window will do
		if(hWnd == NULL)
			hWnd = ::GetActiveWindow();
		if(hWnd == NULL)
		{
			hWnd = ::CreateWindowEx(0,_T("static"),_T(""),0,0,0,10,10,NULL,NULL,AfxGetInstanceHandle(),0);
			bCreated = (hWnd != NULL);
		}
		ASSERT(hWnd != NULL);
		HDC hdc = ::GetDC(hWnd);
		ASSERT(hdc != NULL);
		if (hdc != NULL)
		{
			m_hdc = ::CreateCompatibleDC(hdc);
			::ReleaseDC(hWnd,hdc);
		}
		if (bCreated)
		{
			::DestroyWindow(hWnd);
		}
	}
	
	if (!m_hdc)
	{
		ASSERT(m_hdc);
		return E_FAIL;
	}
	
	pfont = new CCeFont();
	if (!pfont)
	{
		return E_OUTOFMEMORY;
	}
	//
	// If no font description provided, create a default font
	//
	if (!pfd)
	{
		pfd = &s_fdDefault;
	}
	//
	// Set up the font record
	//
	pfont->m_fr.sWeight     = pfd->sWeight;
	pfont->m_fr.sCharset    = pfd->sCharset;
	pfont->m_fr.fItalic     = pfd->fItalic;
	pfont->m_fr.fUnderline  = pfd->fUnderline;
	pfont->m_fr.fStrikethrough = pfd->fStrikethrough;
	pfont->m_fr.cySize.Hi   = pfd->cySize.Hi;
	pfont->m_fr.cySize.Lo   = pfd->cySize.Lo;
	::wcsncpy(pfont->m_fr.rgchFace, pfd->lpstrName, LF_FACESIZE);
	//
	// Get the requested pointer..
	//
	hr = pfont->QueryInterface(riid, ppv);
	
	if (FAILED(hr))
	{
		delete pfont;
	}
	
	return hr;
}

//
// RoundFloatDiv -- rounded floating-point division
//
double CCeFont::RoundFloatDiv(double numer, double denom)
{
    double quotient = numer / denom;

    double absrem = fabs(fmod(numer, denom));

    if (fabs(denom) - absrem <= absrem)
        {
        if (quotient >= 0)
            {
            quotient++;
            }
          else
            {
            quotient--;
            }
        }
    return quotient;
}

// Rounded long division
long RoundDiv(long numer, long denom)
{
    long quotient;

    if (numer < 0) {
      if (denom < 0) {
        // neg / neg = pos
        quotient = (numer - ((-denom)>>1)) / denom;
      }
      else {
        // neg / pos = neg
        quotient = (numer - (denom>>1)) / denom;
      }
    }
    else {
      if (denom < 0) {
        // pos / neg = neg
        quotient = (numer + ((-denom)>>1)) / denom;
      }
      else {
        // pos / pos = pos
        quotient = (numer + (denom>>1)) / denom;
      }
    }
    return quotient;
}


//
// Call-back function to find the char set of a face name.
//
int CALLBACK CharSetOfFaceCallBack(ENUMLOGFONT *pelfx, TEXTMETRIC *ptm, int FontType,
                                LPARAM  lp)
{
    UNUSED(pelfx);
    UNUSED(FontType);

    *(DWORD*)lp = (DWORD)ptm->tmCharSet;

    return 0;
}

//
// Lookup the CharSet that the given font face name belongs to.
//
HRESULT CharSetOfFace(HDC hdc, OLECHAR *szFaceName, WORD *pwCharset)
{
    TCHAR  lfFaceName[LF_FACESIZE];
    DWORD dwCharset = (DWORD)(-1);

    ::lstrcpyW(lfFaceName, szFaceName);
    ::EnumFontFamilies(hdc, lfFaceName, (FONTENUMPROC)CharSetOfFaceCallBack,
                      (LPARAM)&dwCharset);

    if (dwCharset == (DWORD)-1)
        {
        return E_FAIL;
        }

    *pwCharset = (WORD)dwCharset;

    return NOERROR;
}




//
// Statics
//
PFNADDREF CUnknown::m_pfnAddRef = NULL;
PFNRELEASE CUnknown::m_pfnRelease = NULL;
CRITICAL_SECTION CUnknown::m_csUnkLock;

//
// Constructor
// 1. Initialize Ref count to 0.
// 2. Save aggregating unknown, if any is provided. Else set the aggregating unknown to be
//  this object itself (keeps code simple.. so there is ALWAYS an aggregating unknown)
//
// Parameters:
//  pUnk: aggregating unknown, if any
//
CUnknown::CUnknown(IUnknown *pUnk)
{
	m_cRef = 0;	// should always be zero
	if (pUnk)
		{
		//
		// This object is being aggregated
		//
		m_pUnkOwner = pUnk;
		}
	else
		{
		//
		// the object owns itself! This greatly simplifies the code. This cast from 
		// IInternalUnknown to IUnknown is a dirty trick, but really makes life easier, and
		// gets around the compiler. It exploits the fact that IUnknown and IInternalUnknown
		// are identical interfaces, with identical V-tables and identical type information.
		//
		//
		m_pUnkOwner = (IUnknown *) (IInternalUnknown *) this;	
		}
}

//
// Virtual Destructor
//
CUnknown::~CUnknown(void)
{
	ASSERT(0 == m_cRef);
}

//
// A generic implementation of a QueryInterface for IUnknown
//
// Parameters:
//  riid:   interface requrested
//  ppv: receives the interface
//
// Returns:
//  E_INVALIDARG, E_NOINTERFACE
//  NOERROR
//
STDMETHODIMP CUnknown::InternalQueryInterface(REFIID riid, PVOID *ppv)
{
	if (!ppv)
		{
		ASSERT(ppv);
		return E_INVALIDARG;
		}

	if (riid == IID_IUnknown)
		{
		//
		// An explicit request for the internal IUnknown query interface..
		//
		*ppv = (PVOID) (IUnknown *) (IInternalUnknown *) this;
		}

	return E_NOINTERFACE;
}

//
// Up the ref count on this object. 
// If the component using this class keeps global ref counts, up that
// Global ref counts are accessed using hte static m_pfnAddRef variable
//
STDMETHODIMP_(ULONG) CUnknown::InternalAddRef(void)
{
	::InterlockedIncrement(&m_cRef);

    //
    // Up the global ref count
    //
    if (m_pfnAddRef) // global ref count is accessed using this function
        {
        (*m_pfnAddRef) ();
        }

	return m_cRef; 
}

//
// Decrement the ref count. If the count is 0, commit suicide
// If the component using this class keeps global ref counts, decrement that
// Global ref counts are accessed using hte static m_pfnRelease variable
//
STDMETHODIMP_(ULONG) CUnknown::InternalRelease(void)
{
    LONG cRef;
    
    cRef = ::InterlockedDecrement(&m_cRef);

    //
    // Lower the global count
    //
    if (m_pfnRelease)
        {
        (*m_pfnRelease) ();
        }
	if (cRef > 0)
		{
		return m_cRef;
		}
	//
	// The moment the ref count == 0, we kill this object
	//
	delete this;

	return 0; // ref count is now zero
}

//
// Just a simple useful little wrapper that does AddRef() on a retrieve interface
//
HRESULT CUnknown::GetInterface(IUnknown *pUnk, PVOID *ppv)
{
	ASSERT(pUnk);
    
    if (pUnk)
        {		
	    *ppv = pUnk;
	    ((IUnknown *) pUnk)->AddRef();
        
	    return NOERROR;
        }

    return E_INVALIDARG;
}


// IPersistStream implementation for CCeFont

STDMETHODIMP CCeFont::GetClassID(LPCLSID lpClassID)
{
	*lpClassID = IID_IFont;
	return S_OK;
}

STDMETHODIMP CCeFont::IsDirty()
{
	return m_bModified ? S_OK : S_FALSE;
}


STDMETHODIMP CCeFont::Load(LPSTREAM pStm)
{
	unsigned char buf1[11], buf2[LF_FACESIZE];
	ULONG cb, nLen;

	pStm->Read(buf1, 11, &cb);
	if(cb != 11)
		return E_OUTOFMEMORY;

	m_fr.cyLogical  = 1;
	m_fr.cyHimetric = 1;
	m_fr.sCharset   = (SHORT)buf1[1];
    m_fr.bFlags     = buf1[3];
	m_fr.sWeight    = ((SHORT)buf1[5] << 8) + (SHORT)buf1[4];
	m_fr.cySize.Lo  = (long)(((ULONG)buf1[9] << 24) + 
	                         ((ULONG)buf1[8] << 16) +
							 ((ULONG)buf1[7] << 8)  +
							 ((ULONG)buf1[6]));

	nLen = (ULONG)buf1[10];
	ASSERT(nLen < LF_FACESIZE);

	pStm->Read(buf2, nLen, &cb);
	if(cb != nLen)
		return E_OUTOFMEMORY;

	buf2[nLen] = L'\0';
	wce_AsciiToWide(m_fr.rgchFace, (char*)buf2);

	return S_OK;
}

STDMETHODIMP CCeFont::Save(LPSTREAM pStm, BOOL fClearDirty)
{
	unsigned char buf1[11], buf2[LF_FACESIZE];
	ULONG cb, nLen;

	nLen = (ULONG)wcslen(m_fr.rgchFace);

	buf1[0]  = 1;                              // unknown flag
	buf1[1]  = (BYTE)m_fr.sCharset;            // charset
	buf1[2]  = 0;                              // unknown flag
	buf1[3]  = m_fr.bFlags;                    // style bits
	buf1[4]  = LOBYTE((WORD)m_fr.sWeight);     // weight (lo byte)
	buf1[5]  = HIBYTE((WORD)m_fr.sWeight);     // weight (hi byte)
	DWORD dwSize = (DWORD)m_fr.cySize.Lo;      // size
	buf1[6]  = LOBYTE(LOWORD(dwSize));   
	buf1[7]  = HIBYTE(LOWORD(dwSize)); 
	buf1[8]  = LOBYTE(HIWORD(dwSize)); 
	buf1[9]  = HIBYTE(HIWORD(dwSize));                               
	buf1[10] = (BYTE)nLen;                     // length of face name (excluding null terminator)
	wce_T2CAHelper((char*)buf2, m_fr.rgchFace);

	pStm->Write(buf1, 11, &cb);
	if(cb != 11)
		return STG_E_MEDIUMFULL;

	pStm->Write(buf2, nLen, &cb);
	if(cb != nLen)
		return STG_E_MEDIUMFULL;
	
	if (fClearDirty)
		m_bModified = FALSE;

	return S_OK;
}

STDMETHODIMP CCeFont::GetSizeMax(ULARGE_INTEGER*)
{
	return E_NOTIMPL;
}




// IPersistStream implementation for CCeFont

STDMETHODIMP CCeFont::InitNew()
{
    FONTDESC* pfd = &s_fdDefault;

	m_fr.cyLogical  = 1;
	m_fr.cyHimetric = 1;
    m_fr.sWeight     = pfd->sWeight;
    m_fr.sCharset    = pfd->sCharset;
    m_fr.fItalic     = pfd->fItalic;
    m_fr.fUnderline  = pfd->fUnderline;
    m_fr.fStrikethrough = pfd->fStrikethrough;
    m_fr.cySize.Hi   = pfd->cySize.Hi;
    m_fr.cySize.Lo   = pfd->cySize.Lo;
    ::wcsncpy(m_fr.rgchFace, pfd->lpstrName, LF_FACESIZE);

	return S_OK;
}

// Note: Unlike the IPersistStream implementation, this IPersistPropertyBag implemenation
//        does NOT match the bahavior of OleCreateFontIndirect's.
STDMETHODIMP CCeFont::Load(LPPROPERTYBAG pPropBag, LPERRORLOG pErrorLog)
{
	ASSERT_POINTER(pPropBag, IPropertyBag);

	VARIANT varSig;
	AfxVariantInit(&varSig);
	if(FAILED(pPropBag->Read(OLESTR("OLECE_Signature"), &varSig, pErrorLog)))
		return E_OUTOFMEMORY;

	if((varSig.vt == VT_I2) && (V_I2(&varSig) == (short)0xA77A))
	{
		// Our test says that OLECE.DLL's CCeFont was used to save the data
		VARIANT var[NUM_FONTPROPS];
		int i;

		for(i=0; i < NUM_FONTPROPS; i++ )
			AfxVariantInit(&var[i]);

		for(i=0; i < NUM_FONTPROPS; i++ )
		{	
			if(FAILED(pPropBag->Read(szFontProps[i].szPersistName, &var[i], pErrorLog)))
				return E_OUTOFMEMORY;
		}

		m_fr.cyLogical  = 1;
		m_fr.cyHimetric = 1;
		put_Name(V_BSTR(&var[0])); 
		put_Size(V_CY(&var[1])); 
		put_Bold(V_BOOL(&var[2])); 
		put_Italic(V_BOOL(&var[3])); 
		put_Underline(V_BOOL(&var[4])); 
		put_Strikethrough(V_BOOL(&var[5])); 
		put_Weight(V_I2(&var[6])); 
		put_Charset(V_I2(&var[7]));
		
		SysFreeString(V_BSTR(&var[0])); // assuming pPropBag->Read allocated it
	}
	else 
	{
		// If we get here, we should assume OleCreateFontIndirect persisted the data, 
		// because that's the only thing that would make sense.  However, this is not 
		// a WinCE function.  For now, it's a bad situation.  
		ASSERT(FALSE);
		InitNew();
	}

	return S_OK;
}

STDMETHODIMP CCeFont::Save(LPPROPERTYBAG pPropBag, BOOL fClearDirty, 
						   BOOL fSaveAllProperties)
{
	HRESULT hResult = S_OK;

	ASSERT_POINTER(pPropBag, IPropertyBag);

	// Write out signature so that we know who persisted this data (i.e. OleCreateFontIndirect didn't)
	VARIANT varSig;
	AfxVariantInit(&varSig);
	varSig.vt = VT_I2;
	V_I2(&varSig) = (short)0xA77A; 
	if(FAILED(pPropBag->Write(OLESTR("OLECE_Signature"), &varSig)))
		return STG_E_MEDIUMFULL;

	// Write out font data (using our own CCeFont persistence format, NOT OleCreateFontIndirect's)
	VARIANT var[NUM_FONTPROPS];
	for(int i=0; i < NUM_FONTPROPS; i++)
		AfxVariantInit(&var[i]);

	var[0].vt = VT_BSTR;
	V_BSTR(&var[0]) = SysAllocString(m_fr.rgchFace);
	if(V_BSTR(&var[0]) == NULL)
		return E_OUTOFMEMORY;
	
	var[1].vt = VT_CY;
	V_CY(&var[1]) = m_fr.cySize;

	var[2].vt = VT_BOOL;
	V_BOOL(&var[2]) = m_fr.fBold;

	var[3].vt = VT_BOOL;
	V_BOOL(&var[3]) = m_fr.fItalic;

	var[4].vt = VT_BOOL;
	V_BOOL(&var[4]) = m_fr.fUnderline;

	var[5].vt = VT_BOOL;
	V_BOOL(&var[5]) = m_fr.fStrikethrough;

	var[6].vt = VT_I2;
	V_I2(&var[6]) = m_fr.sWeight;

	var[7].vt = VT_I2;
	V_I2(&var[7]) = m_fr.sCharset;

	for(i=0; i < NUM_FONTPROPS; i++ )
	{
		if(FAILED(pPropBag->Write(szFontProps[i].szPersistName, &var[i])))
		{
			hResult = STG_E_MEDIUMFULL;
			break;
		}
	}

	if (fClearDirty && (hResult == S_OK))
		m_bModified = FALSE;

	SysFreeString(V_BSTR(&var[0]));

	return hResult;
}
