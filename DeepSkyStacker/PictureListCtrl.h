#if !defined(AFX_PICTURELISTCTRL_H__D2ACDE68_6141_4B93_BD30_6B71D7497D34__INCLUDED_)
#define AFX_PICTURELISTCTRL_H__D2ACDE68_6141_4B93_BD30_6B71D7497D34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PictureListCtrl.h : header file
//

#include "ListViewCtrlEx.h"
#include "DSSProgress.h"
#include "FrameList.h"
#include "StackingTasks.h"

/////////////////////////////////////////////////////////////////////////////
// CPictureListCtrl window

constexpr UINT WM_CHECKITEM = (WM_USER + 1);
constexpr UINT WM_LISTITEMCHANGED = (WM_USER + 2);
constexpr UINT WM_SELECTITEM = (WM_USER + 3);

class CPictureListCtrl : public CListCtrlEx,
					     public CFrameList
{
private :
	int					m_lSortColumn;
	bool				m_bAscending;
	CDSSProgress *		m_pProgress;
	CImageList			m_ImageList;
	uint16_t			currentGroupId;
	GUID				m_dwCurrentJobID;
	std::vector<int>	m_vVisibles;
	bool				m_bRefreshNeeded;
	CBitmap				m_bmpGrayScale;
	CBitmap				m_bmpCFACYMG;
	CBitmap				m_bmpCFARGB;
	CBitmap				m_bmpColor;
	CBitmap				m_bmpMask;

// Construction
public:
	CPictureListCtrl();

// Attributes
public:
	void				RefreshList();

// Operations
public:
	void	Initialize();
	void	AddFileToList(LPCTSTR szFile, uint16_t groupId, PICTURETYPE PictureType = PICTURETYPE_LIGHTFRAME, bool bCheck = false, int nItem = -1);
	virtual bool AddFile(LPCTSTR szFile, uint16_t groupId, PICTURETYPE PictureType = PICTURETYPE_LIGHTFRAME, bool bCheck = false)
	{
		AddFileToList(szFile, groupId, PictureType, bCheck);
		return true;
	};

	void	SetCurrentGroupID(uint16_t groupId)
	{
		if (currentGroupId != groupId)
		{
			currentGroupId = groupId;
			SetItemCount(0);
			RefreshList();
		};
	};

	uint16_t	GetCurrentGroupID()
	{
		return currentGroupId;
	};

	uint16_t	GetLastGroupID()
	{
		uint16_t			result = 0;

		for (LONG i = 0;i<m_vFiles.size();i++)
		{
			if (!m_vFiles[i].m_bRemoved)
				result = max(result, m_vFiles[i].m_groupId);
		};

		return result;
	};

	int		CompareItems(int lItem1, int lItem2);
	bool	GetSelectedFileName(CString & strFileName);
	bool	GetItemFileName(int nItem, CString & strFileName);
	bool	GetFirstCheckedLightFrame(CString & strFileName);
	bool	GetItemISOSpeedGainAndExposure(int nItem, int& lISOSpeed, int& lGain, double & fExposure);
	void	UpdateOffset(LPCTSTR szFileName, double fdX, double fdY, double fAngle, const CBilinearParameters & Transformation, const VOTINGPAIRVECTOR & vVotedPairs);
	void	ClearOffset(LPCTSTR szFileName);
	int		FindIndice(LPCTSTR szFileName);
	bool	IsLightFrame(LPCTSTR szFileName)
	{
		bool		bResult = false;
		int			nIndice = FindIndice(szFileName);

		if (nIndice>=0)
			bResult = m_vFiles[nIndice].IsLightFrame() && !m_vFiles[nIndice].m_bRemoved;

		return bResult;
	};

	void	FillJobs(CAllStackingJobs & jobs);

	void	CheckBest(double fPercent);
	void	UnCheckNonStackable();

	bool	AreCheckedPictureCompatible();
	bool	GetPictureSizes(int nItem, int& lWidth, int& lHeight, int& lNrChannels);

	bool	SaveState();

	int	GetNrCheckedFrames(int lGroupID = -1);
	int	GetNrCheckedDarks(int lGroupID = -1);
	int	GetNrCheckedDarkFlats(int lGroupID = -1);
	int	GetNrCheckedFlats(int lGroupID = -1);
	int	GetNrCheckedOffsets(int lGroupID = -1);
	int	GetNrFrames(int lGroupID = -1);
	bool	IsChecked(int nItem)
	{
		return m_vFiles[m_vVisibles[nItem]].m_bChecked;
	};

	bool	IsChecked(LPCTSTR szFileName)
	{
		bool		bResult = false;
		int			nIndice;

		nIndice = FindIndice(szFileName);
		if (nIndice>=0)
			bResult = m_vFiles[nIndice].m_bChecked == Qt::Checked;

		return bResult;
	};

	void	CheckAbove(double fThreshold);

	void	SortListQualityDesc();

	void	CheckAll(bool bCheck);
	void	CheckAllLights(bool bCheck);
	void	CheckAllDarks(bool bCheck);
	void	CheckAllFlats(bool bCheck);
	void	CheckAllOffsets(bool bCheck);

	void	CheckImage(LPCTSTR szImage, bool bCheck);

	void	SetUseAsStarting(int nItem, bool bUse);
	bool	GetTransformation(LPCTSTR szFile, CBilinearParameters & Transformation, VOTINGPAIRVECTOR & vVotedPairs);

	void	SetProgress(CDSSProgress * pProgress)
	{
		m_pProgress = pProgress;
	};

	void	Clear()
	{
		DeleteAllItems();
		m_vFiles.clear();
	};

	void	CopyToClipboard();

    void	ChangePictureType(int nItem, PICTURETYPE PictureType);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPictureListCtrl)
	//}}AFX_VIRTUAL

private:
	//void	UpdateOffset(int nItem);

	void PostItemChanged()
	{
		PostMessage(WM_LISTITEMCHANGED);
	};

	void ToggleCheckBox(int nItem)
	{
		//Change check box
		auto& file = m_vFiles[m_vVisibles[nItem]];

		if (file.m_bChecked == Qt::Checked)
		{
			file.m_bChecked = Qt::Unchecked;
		}
		else
		{
			file.m_bChecked = Qt::Checked;
		}

		//And redraw
		RedrawItems(nItem, nItem);
		m_bRefreshNeeded = true;
		PostItemChanged();
	}

public:
	virtual ~CPictureListCtrl();

	void	SortList(int nSubItem);
	void	UpdateItemScores(LPCTSTR szFileName);
	void	updateCheckedItemScores();
	void	BlankCheckedItemScores();
	void	ClearOffsets();

	void	SaveList(CMRUList & MRUList, CString & strFileList);
	void	LoadList(CMRUList & MRUList, CString & strFileList);

	const ListBitMap & GetItem(int nIndice);

	// Generated message map functions
protected:
	//{{AFX_MSG(CPictureListCtrl)
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown( UINT, CPoint );
	afx_msg void OnRButtonDown( UINT, CPoint );
	afx_msg void OnKeydownList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnListItemChanged(WPARAM, LPARAM);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/* ------------------------------------------------------------------- */
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICTURELISTCTRL_H__D2ACDE68_6141_4B93_BD30_6B71D7497D34__INCLUDED_)
