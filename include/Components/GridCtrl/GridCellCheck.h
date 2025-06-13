/**
 * @file		GridCellCheck.h
 * @brief		CGridCellCheck class header file
 * @date		2024.07.20
 */

#if !defined(AFX_GRIDCELLCHECK_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_)
#define AFX_GRIDCELLCHECK_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "GridCell.h"

// Checkbox placement
enum {
    SCP_DEFAULT = 0,
    SCP_CENTERING,
};

class CGridCellCheck : public CGridCell
{
    friend class CGridCtrl;
    DECLARE_DYNCREATE(CGridCellCheck)

public:
    CGridCellCheck();

public:
	BOOL SetCheck(BOOL bChecked = TRUE);
	BOOL GetCheck();

// Operations
	virtual CSize GetCellExtent(CDC* pDC);
    virtual void OnClick( CPoint PointCellRelative);
    virtual BOOL GetTextRect( LPRECT pRect);
    virtual void SetCheckPlacement(int nFormat);
    virtual BOOL IsClickPtInBox(void);

protected:
	CRect GetCheckPlacement(void);
    void  SetClickPtInBoxFlag(BOOL bValue);

    virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);

protected:
    BOOL  m_bChecked;
    CRect m_Rect;
    BOOL  m_bCentering;
    BOOL  m_bClickPtInBox;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCELLCHECK_H__ECD42822_16DF_11D1_992F_895E185F9C72__INCLUDED_)
