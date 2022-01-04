// RobotExp_4Dlg.h : ��� ����
//

#pragma once
#include "Comm.h"
#include "CommWork.h"
#include "DeviceListReader.h"

// Graph Dialog ������� �߰�
#include "GraphDlg.h"

// ��Ƽ ������ �ø��� ���
#include "ThreadWorker.h"
#include "SharedMemory.h"
#include "SystemMemory.h"
#include "DataType.h"

#include "stdafx.h"
#include <math.h>
#include <stdio.h>


// CRobotExp_4Dlg ��ȭ ����
class CRobotExp_4Dlg : public CDialogEx
{
// �����Դϴ�.
public:
	CRobotExp_4Dlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROBOTEXP_4_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


public:
	void SolveForwardKinematics(double dAngle, double dAngle2, double* pdPos);
	void SolveInverseKinematics(double dX, double dY, double dZ, double* pdAngle);

private:
	CGraphDlg* m_pGraphDlg;

// �����Դϴ�.
protected:
	HICON m_hIcon;
	CComm m_comm;
	CThreadedWorker _commWorker;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboPort;
	CComboBox m_ComboBaud;
	CButton m_CheckOpen;
	afx_msg void OnCbnDropdownComboPort();
	afx_msg void OnBnClickedCheckOpen();


	CEdit m_EditSend;
	CEdit m_EditRecv;
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	// 2 DOF ���� ����
	CEdit m_editTarPos1;
	CEdit m_editCurPos1;
	CEdit m_editTarPos2;
	CEdit m_editCurPos2;
	CEdit m_editTarVel;
	CEdit m_editCurVel;
	CEdit m_editTarTorq;
	CEdit m_editCurTorq;
	CEdit m_editTarX;
	CEdit m_editCurX;
	CEdit m_editTarY;
	CEdit m_editCurY;
	CEdit m_editTarZ;
	CEdit m_editCurZ;

	// ��ư �Լ�
	afx_msg void OnBnClickedButtonInit();
	afx_msg void OnBnClickedButtonForward();
	afx_msg void OnBnClickedButtonBackward();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonGraph();
	afx_msg void OnBnClickedButtonSet();
};
