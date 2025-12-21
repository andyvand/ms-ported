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

#ifndef __WCEFONT_H__
#define __WCEFONT_H__

typedef void (CALLBACK *PFNADDREF) (void);
typedef void (CALLBACK *PFNRELEASE) (void);


DECLARE_INTERFACE(IInternalUnknown)
{
	STDMETHOD(InternalQueryInterface) (THIS_ REFIID riid, PVOID *ppv) PURE;
	STDMETHOD_(ULONG, InternalAddRef) (THIS) PURE;
	STDMETHOD_(ULONG, InternalRelease) (THIS) PURE;
};

class CUnknown : public IInternalUnknown
{
public:

	CUnknown(IUnknown *pUnk);
	CUnknown(void)
	{
		CUnknown(NULL);
	}
    //
    // virtual destructors are a must!
    //
	virtual ~CUnknown(void);
	//
    // IInternalUnknown
    //
	STDMETHODIMP InternalQueryInterface(REFIID riid, PVOID *ppv);
	STDMETHODIMP_(ULONG) InternalAddRef(void);
	STDMETHODIMP_(ULONG) InternalRelease(void);
	//
	// Return the interface to the object that owns this object
	//
	IUnknown* GetOwner(void)
	{
		return m_pUnkOwner;
	}
	
	//
    // A simple wrapper that does a QueryInterface and AddRef
    //
	HRESULT GetInterface(IUnknown *pUnk, PVOID *ppv);
	//
    // For synchronization. This way, all objects are guaranteed consistent locking/unlocking
    // mechanisms.
    //
	virtual void Lock(void)
	{
	}
	
	virtual void Unlock(void)
	{
	}

	static void SetFnAddRefRelease(PFNADDREF pfnAddRef, PFNRELEASE pfnRelease)
	{
        m_pfnAddRef = pfnAddRef;
        m_pfnRelease = pfnRelease;
	}

    void ClearRef(void)
	{
        Lock();
        m_cRef = 0;
        (*m_pfnRelease) ();
        Unlock();
	}

	//
	// Data	
	//
protected:
	LONG		m_cRef;			    // ref count
	IUnknown	*m_pUnkOwner;	    // owning object - for aggregation
	static PFNADDREF m_pfnAddRef;       // function to call when a ref count added
	static PFNRELEASE m_pfnRelease;     // function to call when a ref count is released
	static CRITICAL_SECTION  m_csUnkLock;  // common default critical section for all
};


//
// fr - font record
//
struct FR
{
    CY       cySize;		// height, in points
    LONG     cyLogical;
    LONG     cyHimetric;
    SHORT    sCharset;
    SHORT    sWeight;
	union {
		struct{
			BYTE fBold:1; // UNUSED: bold is determined from sWeight
			BYTE fItalic:1;
			BYTE fUnderline:1;
			BYTE fStrikethrough:1;
		};
		BYTE bFlags;
    };
    OLECHAR  rgchFace[LF_FACESIZE];
};
//
// Font types
//
#define FONT_fBold          0x01 // UNUSED
#define FONT_fItalic        0x02
#define FONT_fUnderline     0x04
#define FONT_fStrikethrough 0x08
#define FONT_fAll           0x0F
//
// Number of entries by which the font cache is grown
//
#define FONT_CACHE_GROWBY 8
//
// fcr - font cache record
//
struct FCR
{
    FR       frRequest;	// the font requested by the user
    FR       frActual;	// the font window's actually supplied
    ULONG    cRefs;
    HFONT    hfont;
    TEXTMETRICOLE tm;
};


//--------------------------------------------------------------------------------------------
//
// Classes
//
//--------------------------------------------------------------------------------------------
class CCeFont :  public CUnknown,
                 public IFont,
                 public IFontDisp,
                 public IPersistStream,
                 public IPersistStreamInit,
                 public IPersistPropertyBag
{
public:
    CCeFont(void);
    ~CCeFont(void);

	STDMETHODIMP QueryInterface(REFIID riid, PVOID *ppv)	
	{														
		return GetOwner()->QueryInterface(riid, ppv);		
	}													
															
	STDMETHODIMP_(ULONG) AddRef(void)						
	{													
		return GetOwner()->AddRef();						
	}													
															
	STDMETHODIMP_(ULONG) Release(void)						
	{													
		return GetOwner()->Release();						
	}													
	
	STDMETHODIMP GetTypeInfoCount(UINT *pctInfo);		
	
	STDMETHODIMP GetTypeInfo(UINT itinfo, LCID lcid,		
							ITypeInfo **pptinfo);								
	
	STDMETHODIMP GetIDsOfNames(REFIID riid,				
							OLECHAR **rgszNames,		
							UINT	cNames,				
							LCID	lcid,				
							DISPID	*rgdispids);			
	
	STDMETHODIMP Invoke(DISPID dispid,						
						REFIID riid, LCID lcid, WORD wFlags,
						DISPPARAMS *pdispparams,			
						VARIANT *pVarResult, EXCEPINFO *pei,
						UINT *puArgErr);						

    STDMETHODIMP get_Name(BSTR *pname);
    STDMETHODIMP put_Name(BSTR name);

    STDMETHODIMP get_Size(CY *psize);
    STDMETHODIMP put_Size(CY size);

    STDMETHODIMP get_Bold(BOOL *pbold);
    STDMETHODIMP put_Bold(BOOL bold);
    STDMETHODIMP get_Italic(BOOL *pitalic);
    STDMETHODIMP put_Italic(BOOL italic);
    STDMETHODIMP get_Underline(BOOL *punderline);
    STDMETHODIMP put_Underline(BOOL underline);
    STDMETHODIMP get_Strikethrough(BOOL *pstrikethrough);
    STDMETHODIMP put_Strikethrough(BOOL strikethrough);
    STDMETHODIMP get_Weight(short *pweight);
    STDMETHODIMP put_Weight(short weight);
    STDMETHODIMP get_Charset(short *pcharset);
    STDMETHODIMP put_Charset(short charset);
    STDMETHODIMP get_hFont(HFONT *phfont);
    STDMETHODIMP Clone(IFont **lplpfont);
    STDMETHODIMP IsEqual(IFont *lpFontOther);
    STDMETHODIMP SetRatio(long cyLogical, long cyHimetric);
    STDMETHODIMP QueryTextMetrics(LPTEXTMETRICOLE ptm);
    STDMETHODIMP AddRefHfont(HFONT hfont);
    STDMETHODIMP ReleaseHfont(HFONT hfont);
    STDMETHODIMP SetHdc(HDC hdc);
    //
    // CUnknown overrides
    //
    STDMETHODIMP InternalQueryInterface(REFIID riid, PVOID *ppv);
    //
    // CCeFont
    //
	static HDC Hdc(void)
	{
		// Currently, we don't support the setting of a DC for every font, but when we
		// do, this routine can return m_hdc
		return m_hdc;
	}

    HRESULT UpdateFont();
    HRESULT DiscardFont(BOOL fNotify, DISPID dispidChanged);
    HRESULT UpdateFr(HFONT hfont, TEXTMETRICOLE *ptm, OLECHAR rgchFace[LF_FACESIZE]);

    HRESULT NewFcr(int *pnFcr);
    HRESULT ChkName(LPOLESTR szName);
    HRESULT FindMatchingFont(int *pnFcr);
    HRESULT CreateMatchingFont(HFONT *phfont);
    HRESULT QueryFontInfo(HFONT hfont, TEXTMETRICOLE *ptm, OLECHAR rgchFace[LF_FACESIZE]);
    //
    // Statics
    //
	static HRESULT CreateFont(LPFONTDESC pfd, REFIID riid, PVOID *ppv);
	static double RoundFloatDiv(double numer, double denom);
	static HRESULT InitFontObject();
	static HRESULT UninitFontObject();
	static BOOL FontGetTextMetrics(HDC hdc, LPTEXTMETRICOLE ptm)
	{
		return GetTextMetricsW(hdc, ptm);
	}
	static int FontGetTextFace(HDC hdc, int cbBuffer, OLECHAR *szFace)
	{
		return GetTextFace(hdc, cbBuffer, szFace);
	}

	// IPersistStream methods
	STDMETHODIMP GetClassID(LPCLSID lpClassID);
	STDMETHODIMP IsDirty();
	STDMETHODIMP Load(LPSTREAM pStm);
	STDMETHODIMP Save(LPSTREAM pStm,BOOL fClearDirty);
	STDMETHODIMP GetSizeMax(ULARGE_INTEGER*);

	// IPersistPropertyBag methods
	STDMETHODIMP InitNew();
	STDMETHODIMP Load(LPPROPERTYBAG pPropBag, LPERRORLOG pErrorLog);
	STDMETHODIMP Save(LPPROPERTYBAG pPropBag, BOOL fClearDirty, BOOL);

//
// Data
//
protected:
    FR    m_fr;		    // Description of our font
    HFONT m_hfont;      // The font handle
    int   m_nFcr;		// Index to corresponding font cache record
				        // (-1 if this object does not yet have a corresponding HFONT). 
                        // NOTE: not a ptr because the font-cache-record may move
				        // (if the font cache is realloc'd).
    LONG  m_cRefs;
    BYTE  m_fNameDirty:1;	// Used to minimize ammount of CharSet
    BYTE  m_fCharSetDirty:1;	//  lookup we do.
	static int  s_cFcr;		// Count of elements in the font cache
	static FCR *s_rgfcr;	// The font cache - array of font cache records
	static BOOL s_fInit;	// Has the font cache been initialized?
	static HDC  m_hdc;

	BOOL m_bModified;
};


#endif //__WCEFONT_H__

