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


#ifndef __WCEDB_H__
#define __WCEDB_H__

/////////////////////////////////////////////////////////////////////////////
// Wrapper class for CEPROPVAL structure
// Implementation file: wcedb.cpp
/////////////////////////////////////////////////////////////////////////////

// Note: you'll see references to "sort" properties.  They contain the
// application-defined identifier, the CEVT_* datatype, but do not contain
// any data.  Furthermore, it also contains a set of flags to specify
// sorting information.

// Properties have application-defined identifiers, labelled wIdent
class CCeDBProp : public CObject
{
	DECLARE_DYNAMIC(CCeDBProp);

public:
	// Enumerations
	// used in the construction of a CePropVal or a Blob prop to tell it
	// to allocate new memory to store the data passed into the constructor
	enum enCopyMode
	{
		NoCopy = 0,
		Copy = 1
	};
		
	// Wraps the CEVT_* data types, used in construction.  It is primarily
	// used when constructing sort properties.
	enum enType
	{
		Type_Short = 0,
		Type_UShort,
		Type_Long,
		Type_ULong,
		Type_Filetime,
		Type_String,
		Type_Blob,
		Type_Bool,
		Type_Double,
		Type_Empty,
	};

	// Wraps the CEDB_SORT_* flags, used when creating "sort" Properties..
	enum enSort
	{
		Sort_Ascending       = 0,
		Sort_Descending      = CEDB_SORT_DESCENDING,
		Sort_CaseInsensitive = CEDB_SORT_CASEINSENSITIVE,
		Sort_UnknownFirst    = CEDB_SORT_UNKNOWNFIRST,
		Sort_GenericOrder    = CEDB_SORT_GENERICORDER
	};

	// Public data
	CEPROPVAL m_CePropVal;  // See Ce Database docs
	WORD m_wSortFlags;       // if this is a "sort" property, this contains
	                         // the flags doc'd in SORTORDERSPEC

	// Constructors/destructors
	CCeDBProp();  // default constructor
	
	// ctor for creating sort properties.
	CCeDBProp(enum enType nType,WORD wIdent = 0,
	           WORD wSortFlags = Sort_Ascending);

	// ctor for short (CEVT_I2)
	CCeDBProp(short iVal,WORD wIdent = 0);

	// ctor for unsigned short (CEVT_UI2)
	CCeDBProp(USHORT uiVal,WORD wIdent = 0);

	// ctor for long (CEVT_I4)
	CCeDBProp(long lVal,WORD wIdent = 0);

	// ctor for unsigned long (CEVT_UI4)
	CCeDBProp(ULONG ulVal,WORD wIdent = 0);

	// ctor for FILETIME (CEVT_FILETIME)
	CCeDBProp(FILETIME filetime,WORD wIdent = 0);

	// ctor for LPWSTR (CEVT_LPWSTR).  Copy means that new memory
	// is allocated to store the passed in string.  NoCopy means
	// that this prop will point to the existing string, but won't
	// own it.
	CCeDBProp(LPWSTR lpwstr,WORD wIdent = 0,enCopyMode nMode = Copy);
	
	// ctor for CEBLOB (CEVT_CEBLOB).  Copy/Nobody works the same
	// way as above but with blobs instead of strings.
	CCeDBProp(CEBLOB blob,WORD wIdent = 0,enCopyMode nMode = Copy);

#if (_WIN32_WCE > 201)
	// ctor for BOOL (CEVT_BOOL)
	CCeDBProp(BOOL bBool,WORD wIdent = 0);

	// ctor for double (CEVT_R8)
	CCeDBProp(DOUBLE dVal,WORD wIdent = 0);
#endif // _WIN32_WCE

	// destructor
	virtual ~CCeDBProp();

	// copy constructor
	CCeDBProp(CCeDBProp& prop);

	// assignment operator 
	CCeDBProp& operator=(const CCeDBProp& prop);


	// Get/Set operations

	// The datatype (a CEVT_*) value. 
	// Note: in the private section there's corresponding calls that get/sets
	// the datatype using the CEVT_*, which is hidden inside the class.
	void SetType(enType nType);
	enType GetType() const;

	// The application-defined identifier
	void SetIdent(WORD wIdent);
	WORD GetIdent() const;

	// For "sort" properties (whether or not it really is isn't enforced).
	// Uses zero or more flags specified in enumeration above.
	void SetSortFlags(WORD wSortFlags);
	WORD GetSortFlags() const;

	// Get/set the short value.  
	// The property datatype must be a short.
	void SetShort(short iVal);
	short GetShort() const; // requires that the propid says this is a short

	// Get/set the unsigned short value.  
	// The property datatype must be an unsigned short.
	void SetUShort(USHORT uiVal);
	USHORT GetUShort() const; // requires ushort prop

	// Get/set the long value.  
	// The property datatype must be a long.
	void SetLong(long iVal);
	long GetLong() const;   // requires long prop 

	// Get/set the unsigned long value.  
	// The property datatype must be an unsigned long.
	void SetULong(ULONG ulVal);
	ULONG GetULong() const; // requires ulong prop

	// Get/set the filetime value
	// The property datatype must be a filetime.
	void SetFiletime(FILETIME filetime);
	FILETIME GetFiletime() const; // requires filetime prop
	
	// Get/set the string value, with the Copy flag described earlier.
	// The property datatype must be a string.
	// The value returned is the same string as pointed to by the property,
	// not a copy.
	void SetString(LPWSTR lpwstr,UINT nMode = Copy);
	LPWSTR GetString() const;   // requires string prop

	// Get/set the blob value, with the Copy flag described earlier.
	// The property datatype must be a string.
	// The returned blob is the same one used by the property, not a copy.
	void SetBlob(CEBLOB blob,UINT nMode = Copy);
	CEBLOB GetBlob() const;    // requires blob prop

#if (_WIN32_WCE > 201)
	// Get/set the BOOL value, with the Copy flag described earlier.
	// The property datatype must be a BOOL.
	// The returned BOOL is the same one used by the property, not a copy.
	void SetBool(BOOL bBool);
	BOOL GetBool() const;    // requires blob prop

	// Get/set the double value, with the Copy flag described earlier.
	// The property datatype must be a double.
	// The returned double is the same one used by the property, not a copy.
	void SetDouble(DOUBLE dDouble);
	DOUBLE GetDouble() const;    // requires blob prop
#endif // _WIN32_WCE

private:
	BOOL m_bOwnsStringOrBlob; 
	// m_bOwnsStringOrBlob applies only to string and blob properties.
	// It is FALSE if the data is stored in a common buffer owned by a
	// CCeDBRecord, as is the case when reading a whole record from
	// CeReadRecordProps. 
	BOOL m_bDeletePtr; // if a record allocates its property (and points 
	// to it) this property needs to be deleted.  This is independent from 
	// m_bOwnsStringOrBlob
	CEPROPID GetPropid() const; 
	void   SetCEVT(WORD wType); 
	WORD   GetCEVT() const;      
	static enType CEVTtoType(WORD wType);
	static WORD   TypeToCEVT(enType nType);	
	void   InitProp(); // Used in construction of the object
	void   FreeProp(); // Deletes memory used by string or blob properties
	               // but doesn't delete this C++ object.};

	// (ctor for creating a CCeDBProp based on an existing
	// CEPROPVAL (which is completely wrapped from the user's pt. of view).
	// The Copy flag applies to LPWSTR and CEBLOB properties
	CCeDBProp(CEPROPVAL val,enCopyMode nMode = Copy); 

	friend class CCeDBDatabase; // grants access to private members
	friend class CCeDBRecord;   // grants access to private members
};
/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
// Wrapper class for and array of CCeDBProp (which is basically the 
//   CEPROPVAL structure)
// Implementation file: wcedb.cpp
/////////////////////////////////////////////////////////////////////////////

class CCeDBRecord
{
	friend class CCeDBDatabase; // grants access to private members

public:
	
	// Public data
	CObArray m_pPropArray; // the array of properties

	// Constructor/destructor
	CCeDBRecord();
	virtual ~CCeDBRecord();

	// copy constructor
	CCeDBRecord(CCeDBRecord& rec);

	// assignment operator
	CCeDBRecord& operator=(const CCeDBRecord& rec);


	// Operations

	// Adds a property to this record (the CCeDBRecord object, not the
	// database's representation)
	BOOL AddProp(CCeDBProp* pProp);

	// Add more than one prop to this record.  Takes in a regular array
	// of props and a size.
	BOOL AddProps(CCeDBProp* pPropArray,int nNumProps);

	// Deletes a property from the record (again, just the class's copy. 
	// If you call WriteCurrRecord, it'll delete the extra props there
	// at that time.)  Note the property is referred to by the
	// application-defined wIdent
	void DeleteProp(WORD wPropIdent); // decrements array size by 1
	
	// Like DeleteProp() but for all properties. 
	void DeleteAllProps();


	// Queries

	// Returns the number of properties in the record.
	int  GetNumProps() const;

	// Retrieves a property based on the application-defined wIdent
	CCeDBProp* GetPropFromIdent(WORD wPropIdent) const;

	// Retrieves a property based on its index (used for looping up
	// to the GetNumProps())
	CCeDBProp* GetPropFromIndex(int nIndex) const;

	CEOID GetRecordID()
	{
		return m_iCEOID;
	}

	void SetRecordID( CEOID RecordId)
	{
		m_iCEOID = RecordId;
	}

	
	// Other operarions.  
private:
	CEOID	m_iCEOID; // CEOID of this record. When CCeDBDatabase::ReadCurrRecord, 
					  // the CEOID of the record is store in the m_iCEOID. 
					  // You can also use CCeDBDatabase::AddRecordEx with the specifyed 
					  // CEOID in m_iCEOID, so that this record is added 
					  // to the specified CEOID in the database.
	BYTE *m_pbBuffer; // Returned by CeReadRecordProps() (see doc).  
	                  // This buffer will be owned by the class, and will 
	                  // be freed on Delete() and ~CCeDBRecord().
	void FreeRecord(); // deletes all props, frees m_pbBuffer,
	                   // and sets array size to 0
};

/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
// The Ce Database class
// Implementation file: wcedb.cpp
/////////////////////////////////////////////////////////////////////////////

class CCeDBDatabase : public CObject
{
	DECLARE_DYNAMIC(CCeDBDatabase);

public:

	// Data
	// These members are public in the case that someone wants to call
	// the API directly.
	CEOID  m_CEOID;  // object store id as doc'd by Ce Database API
	HANDLE m_hHandle; // handle returned from CeOpenDatabase() (ditto)      
	BOOL   m_bAutoSeekNext; // flag to call SeekNext() after reading and 
  	                        // writing records, default is FALSE 
	BOOL   m_bEOF;          // End-Of-File flag, set to TRUE only by SeekNext().
	                        // Internally, this value is used only by ReadNextRecord().

	// Constructor/destructor
	CCeDBDatabase();      
	virtual ~CCeDBDatabase();

	// Basic operations

	// This creates a peg database, given a object store name, but doesn't open it. 
	// The return value are the same as by CeCreateDatabase().  If the db already
	// exists, it is NOT recreated.

	// dwIdent is the application-defined identifier (the Ce Database API calls
	// this dwDbaseType, but I didn't like it for several reasons... 
	// 1. it looks funny,
	// 2. type implies structure, which may not be the case (after all, it's app-defined)
	// 3. it's consistent with the app-defined wIdent for properties
	
	// The two sort parameters wrap the SORTORDERSPEC with an array of "sort" properties.  
	// (A sample illustrates usage)
	// Returns the new pegoid, or null of there's an error. Use GetLastError().
	CEOID Create(LPCWSTR szName,
	              DWORD  dwIdent = 0,      // Note: this can be set later with SetType()
	              int    nNumSortProps = 0, // Note: these can be set later with
		          const CCeDBProp* pSortProps = NULL); // SetSortOrder()

	// This opens the database given the object store name
	// Optionally you can give it a "sort" property that will affect the seek order. 
	// (All considerations such as efficiency, behavior, etc, are the same as the
	// Ce Database API.
	// The nofify CWnd parallels the CeDatabaseOpen hWndNotify argument.
	// Returns success of the open. Use GetLastError()
	BOOL Open(LPCWSTR szName,
	          const CCeDBProp* pKeyProp = NULL,
	          const CWnd* pWndNotify = NULL);

	// This opens the database given the object store id (returned from Create(),
	// CCeDBEnumeration::Next(), or elsewhere)
	// Returns success of the Open.  Use GetLastError()
	BOOL Open(CEOID CeOID,
	          const CCeDBProp* pKeyProp = NULL,
	          const CWnd* pWndNotify = NULL);

	// This closes the database.  Note that closing is automatically done upon
	// destruction of the object or when Delete() is called.
	// Returns success of the close.
	BOOL Close();  // Optional for user.  Automatically called by dtor.

	// Removes database from object store, closing if necessary.
	// Doesn't delete this C++ object, but it resets 
	// it so nothing further can be done with it.
	// Returns success of the delete.  Use GetLastError().
	BOOL Delete(); 
           

	// Static information functions (you can invoke these functions without a 
	// database object.)  (If they weren't static then you'd have a chicken-and-
	// egg problem or you'd have to make them global scope).
	// Note: this doesn't check to see if the objet store object is a database.
	static BOOL Exists(LPCWSTR szName);  // object store name
	static BOOL Exists(CEOID CeOID);   // object store id


	// Seeking operations (very close to SDK). Each return the new pegoid for 
	// the record seeked to, or NULL if none found.  The current seek position 
	// affects the record being written to and read from.
	// GetLastError() can be used to get errors.
	
	// This seeks the nth record (zero indexed) from the top or bottom of the
	// database.  Note, the sort order is in effect for all seeks.
	CEOID SeekToIndex(DWORD dwCurrIndex,BOOL bFromEnd = FALSE);

	// Seeks the first record.
	CEOID SeekFirst() { return SeekToIndex(0);      }
	
	// Seeks the last record.
	CEOID SeekLast()  { return SeekToIndex(0,TRUE); }

	// Seeks the next record.  This is automatically called when calling 
	// ReadCurrRecord() and m_bAutoSeekNext == TRUE. 
	CEOID SeekNext();

	// Seeks previous record.
	CEOID SeekPrev();

	// Seeks (or finds) the record specified by the id
	CEOID SeekToRecord(CEOID CeOID);

	// Seeks for the first record with a property that matches the given property
	CEOID SeekFirstEqual(CCeDBProp& Prop);

	// Seeks the next one...
	CEOID SeekNextEqual(CCeDBProp& Prop);

	// See SDK docs for the behavior of the next two... it's a little confusing.
	CEOID SeekValueSmaller(CCeDBProp& Prop);
	CEOID SeekValueGreater(CCeDBProp& Prop);

	// Returns the current index from the top of the database (respective to the
	// current sort order). Zero-based.
	DWORD  GetCurrIndex() const;

	// Returns the id of the current record, NULL if none pointed to.
	CEOID GetCurrRecord() const;



	// Read/write operations. 
	
	// Reads the current record, and fills in the record passed in (any properties
	// contained therein are deleted, and previous memory is freed).  Reading a subset
	// of a record (recommended by the SDK docs for efficiency reasons) is accomplished
	// by passing in an array of properties.  Properties don't need values, so use dummy
	// values of the correct type when constructing them.  A sample illustrates this.
	// Not passing in the array (or passing 0) will read the whole record.
	// See SeekNext() on autoseeking behavior.
	// Returns success.  Use GetLastError()
	BOOL ReadCurrRecord(CCeDBRecord* pRecord,
	                     int nNumFilterProps = 0, // 0 means all
	                     CCeDBProp *PropFilterArray = NULL);

	// The following 4 functions are implemented as inline so we don't incur the
	// cost of an unnecessary function call.  These broken-down calls are considerably more 
	// intuitive than the one-size-fits-all WriteRecordProps (which is even more
	// intuitive than the original CeWriteRecordProps!)

	// Same idea as ReadCurrRecord, except we're writing a subset instead of reading.
	// Note that writing a subset doesn't remove unwritten properties from the record.

	// The sort order is maintained when a key property is changed.
	// Returns success.  Use GetLastError()
	BOOL WriteCurrRecord(CCeDBRecord* pRecord,
	                     int nNumFilterProps = 0,
					     CCeDBProp *PropFilterArray = NULL)
	{
		return (WriteRecordProps(pRecord,ModifyProps,nNumFilterProps, 
								PropFilterArray) != NULL);
	}

	// Adds a whole record to the database, in the position determined by the 
	// m_iCEOID in the pRecord and the sort order.
	// Returns success.  Use GetLastError()
	BOOL AddRecord(CCeDBRecord* pRecord)
	{
		return (WriteRecordProps(pRecord,NewRecord,0,0) != NULL);
	}

	// Adds a whole record to the database, in the position determined by the 
	// m_iCEOID in the pRecord and the sort order.
	// Returns success.  Use GetLastError()
	// retID is the CEOID of the record added.
	BOOL AddRecord(CCeDBRecord* pRecord, CEOID& retID)
	{
		retID = WriteRecordProps(pRecord,NewRecord,0,0); 
		return (retID != NULL);
	}

	// Deletes the current record (from seeking)
	// Returns success.  Use GetLastError()
	// Note: this is really inefficient!  In order to delete a record, you have to find out what
	// properties are contained in it, which requires a reading of the whole record.  This limitation
	// is more due to the SDK rather than the MFC wrapper implementation
	BOOL DeleteCurrRecord(CEOID RecID = 0)
	{
		ASSERT(m_hHandle != INVALID_HANDLE_VALUE);
		if (RecID == 0)
		{
			RecID = GetCurrRecord();
			if(RecID == 0)
			{
				return FALSE;
			}
		}
		return ::CeDeleteRecord(m_hHandle,RecID);
	}

	// Deletes a subset of the properties of the current record by passing in 
	// an array of properties.  As with ReadCurrRecord and WriteCurrRecord, each
	// property will have a dummy value of the appropriate type and application-
	// defined wIdent.
	BOOL DeleteCurrRecordProps(int nNumProps = 0,CCeDBProp *pPropArray = NULL);


	// Get/Set operations

	// Gets the number of records in the db
	WORD  GetNumRecords() const; // no SetNumRecords()

	// Gets the size in bytes of the db
	DWORD GetSize() const;       // no SetSize()

	// Gets the object store name of the db
	BOOL  SetName(LPCWSTR wszName);

	// Sets the object store name of the db
	void  GetName(LPWSTR) const;

	// Sets the application-defined dwIdent of the db
	BOOL  SetIdent(DWORD dwIdent);

	// Gets the application-defined dwIdent of the db
	DWORD GetIdent() const;

	// Sets the modification time of the db
	BOOL  SetLastModified(FILETIME ftLastMotified);

	// Gets the modification time of the db
	FILETIME GetLastModified() const;

	// Gets the current sort order (an array[0-4] of "sort" properties) 
	void  GetSortProps(int *pnSize,CCeDBProp *SortProps) const;

	// Sets the new sort order (an array[0-4) of sort properties.)  Doing
	// this is identical to doing it on Create.  Other considerations
	// described in SDK doc.
	BOOL  SetSortProps(int nSize,const CCeDBProp *SortProps);


	//  Diagnostics (for CObject-compliancy)
	virtual void AssertValid() const; // overridable
	static void AssertValidDbaseName(LPCWSTR); // note its static
 
private:
	// Private helper functions
	BOOL         SetInfo(const CEDBASEINFO*); 
	CEDBASEINFO GetInfo(BOOL* pbOk) const;       

	// Auxiliary write/delete function
	enum { NewRecord, ModifyProps, DeleteProps};
	CEOID WriteRecordProps(CCeDBRecord *pRecord,
					      int nMode /*= CCeDBDatabase::ModifyProps*/,
	 					  int nNumFilterProps /*= 0*/, 
						  CCeDBProp *PropFilterArray /*= NULL*/);
}; 

/////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////
// Enumeration class for Ce Databases
// Implementation file: wcedb.cpp
/////////////////////////////////////////////////////////////////////////////

// This class is used to enumerate databases in the object store.  
// Constuction will start the enumeration, and destruction or exhausting
// the database ends it.
class CCeDBEnum
{
public:
	// Starts an enumeration.  If a value other than zero is passed in,
	// only databases with the specified dwIdent are enumerated.
	CCeDBEnum(DWORD dwIdent = 0);
	virtual ~CCeDBEnum();

	// Gets the next database in the enumeration, or NULL if no more are
	// found.
	CEOID Next();

private:
	HANDLE m_hHandle;
};

/////////////////////////////////////////////////////////////////////////////


#endif // __WCEDB_H__

