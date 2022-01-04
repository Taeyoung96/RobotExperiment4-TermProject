#pragma once
#include "NTGraph.h"
#include "COCX.h"
#include "DataType.h"
#include "SystemMemory.h"


// CGraphDlg ��ȭ �����Դϴ�.
#define RED			RGB(127,0,0)
#define GREEN		RGB(0,127,0)
#define BLUE		RGB(0,0,127)
#define BLACK		RGB(0,0,0)
#define WHITE		RGB(255,255,255)
#define GRAY		RGB(192,192,192)

class CGraphDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGraphDlg)

public:
	CGraphDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CGraphDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRAPH_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void InitNTGraph();

	CNTGraph m_ntgPos;
	CNTGraph m_ntgVel;
	CNTGraph m_ntgTorque;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	double m_dCnt = 0.0;
};
