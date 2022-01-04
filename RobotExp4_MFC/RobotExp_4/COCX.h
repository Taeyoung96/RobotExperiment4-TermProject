#pragma once

// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated
// by Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////

#include "afxwin.h"

class COCX : public CWnd
{
protected:
	DECLARE_DYNCREATE(COCX)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= {0xc9fe01c2,0x2746,0x479b,{0x96,0xab,0xe0,0xbe,0x99,0x31,0xb0,0x18}};
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = nullptr)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID);
	}

	BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
				UINT nID, CFile* pPersist = nullptr, BOOL bStorage = FALSE,
				BSTR bstrLicKey = nullptr)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// Attributes
public:
enum
{
	Solid = 0,
	Dash = 1,
	Dot = 2,
	DashDot = 3,
	DashDotDot = 4,
	Null = 5,
	XYStep = 6,
	YXStep = 7,
	Bars = 8,
	Stick = 9
}LineType;

enum
{
	Nosym = 0,
	Dots = 1,
	Rectangles = 2,
	Diamonds = 3,
	Asterisk = 4,
	DownTriangles = 5,
	UpTriangles = 6,
	LeftTriangles = 7,
	RightTriangles = 8
}SymbolType;

enum
{
	Flat = 0,
	Scope = 1,
	Bitmap = 2
}FrameStyle;

enum
{
	None = 0,
	Track = 1,
	Zoom = 2,
	PanXY = 3,
	PanX = 4,
	PanY = 5
}TrackModeState;



// Operations
public:


};
