
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		GridCellCheck.cpp
//		Description:	CGridCellCheck class implementation file
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.07.20:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCell.h"
#include "GridCtrl.h"

#include "GridCellCheck.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CGridCellCheck, CGridCell)

CGridCellCheck::CGridCellCheck() : CGridCell()
{
    m_bChecked = FALSE;
    //m_Rect.IsRectNull();
    m_bCentering = FALSE;
    m_bClickPtInBox = FALSE;
}

CSize CGridCellCheck::GetCellExtent(CDC* pDC)
{
    // Using SM_CXHSCROLL as a guide to the size of the checkbox
    int nWidth = GetSystemMetrics(SM_CXHSCROLL) + 2*GetMargin();	
    CSize	cellSize = CGridCell::GetCellExtent(pDC);	
    cellSize.cx += nWidth;	
    cellSize.cy = max (cellSize.cy, nWidth);	
    return  cellSize;
}

// i/o:  i=dims of cell rect; o=dims of text rect
BOOL CGridCellCheck::GetTextRect( LPRECT pRect)
{
    BOOL bResult = CGridCell::GetTextRect(pRect);
    if (bResult)
    {
        int nWidth = GetSystemMetrics(SM_CXHSCROLL) + 2*GetMargin();
        pRect->left += nWidth;
        if (pRect->left > pRect->right)
            pRect->left = pRect->right;
    }
    return bResult;
}

// Set the dimensions and placement of the checkbox in client coords.
void CGridCellCheck::SetCheckPlacement(int nFormat)
{
    if (nFormat == SCP_CENTERING) {
        m_bCentering = TRUE;
    }
    else if (nFormat == SCP_DEFAULT) {
        m_bCentering = FALSE;
    }
}

// Get/set the flag with validate if clicked point is inside the checkbox
void CGridCellCheck::SetClickPtInBoxFlag(BOOL bValue)
{
    m_bClickPtInBox = bValue;
}

BOOL CGridCellCheck::IsClickPtInBox(void)
{
    return m_bClickPtInBox;
}

// Override draw so that when the cell is selected, a drop arrow is shown in the RHS.
BOOL CGridCellCheck::Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
    BOOL bResult = CGridCell::Draw(pDC, nRow, nCol, rect,  bEraseBkgnd);

#ifndef _WIN32_WCE
    // Store the cell's dimensions for later
    m_Rect = rect;

    CRect CheckRect = GetCheckPlacement();
    rect.left = CheckRect.right;

    // enough room to draw?
    // if (CheckRect.Width() < rect.Width() && CheckRect.Height() < rect.Height()) {

    // Do the draw 
    pDC->DrawFrameControl(GetCheckPlacement(), DFC_BUTTON, 
	    (m_bChecked)? DFCS_BUTTONCHECK | DFCS_CHECKED : DFCS_BUTTONCHECK);

    // }
#endif
    return bResult;
}

void CGridCellCheck::OnClick(CPoint PointCellRelative)
{
	// PointCellRelative is relative to the topleft of the cell. Convert to client coords
	PointCellRelative += m_Rect.TopLeft();

#if 1
    // Anthony Lee Stark (2024.11.04):
    // Remove this so that the cell will handle and update the checked state itself
#else
    // Bail if cell is read-only
    CCellID cell = GetGrid()->GetCellFromPt(PointCellRelative);	
    if (!GetGrid()->IsCellEditable(cell))		
        return;
#endif

	// GetCheckPlacement returns the checkbox dimensions in client coords. 
    // Only check/uncheck if the user clicked in the box
    SetClickPtInBoxFlag(FALSE);
	if (GetCheckPlacement().PtInRect(PointCellRelative))
	{
		m_bChecked = !m_bChecked;
		GetGrid()->InvalidateRect(m_Rect);
        SetClickPtInBoxFlag(TRUE);
	}
}

//////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////

BOOL CGridCellCheck::SetCheck(BOOL bChecked /*=TRUE*/)
{
	BOOL bTemp = m_bChecked;
	m_bChecked = bChecked;
	if (!m_Rect.IsRectEmpty())
		GetGrid()->InvalidateRect(m_Rect);

	return bTemp;
}

BOOL CGridCellCheck::GetCheck()
{
	return m_bChecked;
}

//////////////////////////////////////////////////////////////////////
// Protected implementation
//////////////////////////////////////////////////////////////////////

// Returns the dimensions and placement of the checkbox in client coords.
CRect CGridCellCheck::GetCheckPlacement()
{
	int nWidth = GetSystemMetrics(SM_CXHSCROLL);
	CRect rect = m_Rect + CSize(GetMargin(), GetMargin());

    CRect place = rect;
    place.right = place.left + nWidth;
    place.bottom = place.top + nWidth;

	// for centering
    if (m_bCentering) {
        int nDiff = (rect.Width() - nWidth) / 2;
        if (nDiff > 0)
        {
            place.left += nDiff;
            place.right = place.left + nWidth;
        }
        nDiff = (rect.Height() - nWidth) / 2;
        if (nDiff > 0)
        {
            place.top += nDiff;
            place.bottom = place.top + nWidth;
        }
    }

    if (m_Rect.Height() < nWidth + 2 * static_cast<int> (GetMargin())) 
    {		
        place.top = m_Rect.top + (m_Rect.Height() - nWidth) / 2;	    
        place.bottom = place.top + nWidth;	
    }

	return place;
}