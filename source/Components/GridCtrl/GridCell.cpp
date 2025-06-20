﻿/**
 * @file		GridCell.cpp
 * @brief		Main grid cell class
 * @date		2024.07.20
 */

#include "Components/GridCtrl/GridCell.h"
#include "Components/GridCtrl/InPlaceEdit.h"
#include "Components/GridCtrl/GridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CGridCell, CGridCellBase)
IMPLEMENT_DYNCREATE(CGridDefaultCell, CGridCell)

/////////////////////////////////////////////////////////////////////////////
// GridCell

CGridCell::CGridCell()
{
    m_plfFont = NULL;
	CGridCell::Reset();
}

CGridCell::~CGridCell()
{
    delete m_plfFont;
}

/////////////////////////////////////////////////////////////////////////////
// GridCell Attributes

void CGridCell::operator=(const CGridCell& cell)
{
    if (this != &cell) CGridCellBase::operator=(cell);
}

void CGridCell::Reset()
{
    CGridCellBase::Reset();

    m_strText.Empty();
    m_nImage   = -1;
    m_lParam   = NULL;           // BUG FIX J. Bloggs 20/10/03
    m_pGrid    = NULL;
    m_bEditing = FALSE;
    m_pEditWnd = NULL;

    m_nFormat = (DWORD)-1;       // Use default from CGridDefaultCell
    m_crBkClr = CLR_DEFAULT;     // Background colour (or CLR_DEFAULT)
    m_crFgClr = CLR_DEFAULT;     // Forground colour (or CLR_DEFAULT)
	m_crHBkClr = CLR_DEFAULT;     // Background colour (or CLR_DEFAULT)
	m_crHFgClr = CLR_DEFAULT;     // Forground colour (or CLR_DEFAULT)
    m_nMargin = (UINT)-1;        // Use default from CGridDefaultCell

    delete m_plfFont;
    m_plfFont = NULL;            // Cell font
}

void CGridCell::SetFont(const LOGFONT* plf)
{
    if (plf == NULL)
    {
        delete m_plfFont;
        m_plfFont = NULL;
    }
    else
    {
        if (!m_plfFont)
            m_plfFont = new LOGFONT;
        if (m_plfFont)
            memcpy(m_plfFont, plf, sizeof(LOGFONT)); 
    }
}

LOGFONT* CGridCell::GetFont() const
{
    if (m_plfFont == NULL)
    {
        CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
        if (!pDefaultCell)
            return NULL;

        return pDefaultCell->GetFont();
    }

    return m_plfFont; 
}

CFont* CGridCell::GetFontObject() const
{
    // If the default font is specified, use the default cell implementation
    if (m_plfFont == NULL)
    {
        CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
        if (!pDefaultCell)
            return NULL;

        return pDefaultCell->GetFontObject();
    }
    else
    {
        static CFont Font;
        Font.DeleteObject();
        Font.CreateFontIndirect(m_plfFont);
        return &Font;
    }
}

DWORD CGridCell::GetFormat() const
{
    if (m_nFormat == (DWORD)-1)
    {
        CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
        if (!pDefaultCell)
            return 0;

        return pDefaultCell->GetFormat();
    }

    return m_nFormat; 
}

UINT CGridCell::GetMargin() const           
{
    if (m_nMargin == (UINT)-1)
    {
        CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
        if (!pDefaultCell)
            return 0;

        return pDefaultCell->GetMargin();
    }

    return m_nMargin; 
}

/////////////////////////////////////////////////////////////////////////////
// GridCell Operations

BOOL CGridCell::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
    if ( m_bEditing )
	{      
        if (m_pEditWnd)
		    m_pEditWnd->SendMessage ( WM_CHAR, nChar );    
    }  
	else  
	{   
		DWORD dwStyle = ES_LEFT;
		if (GetFormat() & DT_RIGHT) 
			dwStyle = ES_RIGHT;
		else if (GetFormat() & DT_CENTER) 
			dwStyle = ES_CENTER;
		
		m_bEditing = TRUE;
		
		// InPlaceEdit auto-deletes itself
		CGridCtrl* pGrid = GetGrid();
		m_pEditWnd = new CInPlaceEdit(pGrid, rect, dwStyle, nID, nRow, nCol, GetText(), nChar);
    }
    return TRUE;
}

void CGridCell::EndEdit()
{
    if (m_pEditWnd)
        ((CInPlaceEdit*)m_pEditWnd)->EndEdit();
}

void CGridCell::OnEndEdit()
{
    m_bEditing = FALSE;
    m_pEditWnd = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CGridDefaultCell

CGridDefaultCell::CGridDefaultCell() 
{
#ifdef _WIN32_WCE
    m_nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX;
#else
    m_nFormat = DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX | DT_END_ELLIPSIS;
#endif
    m_crFgClr = CLR_DEFAULT;
    m_crBkClr = CLR_DEFAULT;
	m_crHFgClr = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_crHBkClr = ::GetSysColor(COLOR_HIGHLIGHT);
    m_Size    = CSize(30,10);
    m_dwStyle = 0;

#ifdef _WIN32_WCE
    LOGFONT lf;
    GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);
    SetFont(&lf);
#else // not CE
    NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof(NONCLIENTMETRICS); // NONCLIENTMETRICS has an extra element after VC6
    VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));
    SetFont(&(ncm.lfMessageFont));
#endif
}

CGridDefaultCell::~CGridDefaultCell()
{
    m_Font.DeleteObject(); 
}

void CGridDefaultCell::SetFont(const LOGFONT* plf)
{
    ASSERT(plf);

    if (!plf) return;

    m_Font.DeleteObject();
    m_Font.CreateFontIndirect(plf);

    CGridCell::SetFont(plf);

    // Get the font size and hence the default cell size
    CDC* pDC = CDC::FromHandle(::GetDC(NULL));
    if (pDC)
    {
        CFont* pOldFont = pDC->SelectObject(&m_Font);

        SetMargin(pDC->GetTextExtent(_T(" "), 1).cx);
        m_Size = pDC->GetTextExtent(_T(" XXXXXXXXXXXX "), 14);
        m_Size.cy = (m_Size.cy * 3) / 2;

        pDC->SelectObject(pOldFont);
        ReleaseDC(NULL, pDC->GetSafeHdc());
    }
    else
    {
        SetMargin(3);
        m_Size = CSize(40,16);
    }
}

LOGFONT* CGridDefaultCell::GetFont() const
{
    ASSERT(m_plfFont);  // This is the default - it CAN'T be NULL!
    return m_plfFont;
}

CFont* CGridDefaultCell::GetFontObject() const
{
    ASSERT(m_Font.GetSafeHandle());
    return (CFont*) &m_Font; 
}
