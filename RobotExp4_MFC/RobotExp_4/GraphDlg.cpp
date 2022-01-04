// GraphDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "RobotExp_4.h"
#include "GraphDlg.h"
#include "afxdialogex.h"


// CGraphDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGraphDlg, CDialogEx)

CGraphDlg::CGraphDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GRAPH_DIALOG, pParent)
{

}

CGraphDlg::~CGraphDlg()
{
}

void CGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_NTGRAPH_POS, m_ntgPos);
	DDX_Control(pDX, IDC_NTGRAPH_VEL, m_ntgVel);
	DDX_Control(pDX, IDC_NTGRAPH_TORQ, m_ntgTorque);
}

BOOL CGraphDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	return TRUE;
}

void CGraphDlg::InitNTGraph()
{
	m_ntgPos.ClearGraph();
	m_ntgVel.ClearGraph();
	m_ntgTorque.ClearGraph();

	m_ntgPos.SetFrameStyle(0);
	m_ntgVel.SetFrameStyle(0);
	m_ntgTorque.SetFrameStyle(0);

	m_ntgPos.SetPlotAreaColor(WHITE);
	m_ntgVel.SetPlotAreaColor(WHITE);
	m_ntgTorque.SetPlotAreaColor(WHITE);

	m_ntgPos.SetShowGrid(TRUE);
	m_ntgVel.SetShowGrid(TRUE);
	m_ntgTorque.SetShowGrid(TRUE);

	m_ntgPos.SetFormatAxisBottom(_T("%.2f"));
	m_ntgVel.SetFormatAxisBottom(_T("%.2f"));
	m_ntgTorque.SetFormatAxisBottom(_T("%.2f"));

	m_ntgPos.SetCaption(_T("Position"));
	m_ntgVel.SetCaption(_T("Velocity"));
	m_ntgTorque.SetCaption(_T("Torque"));

	m_ntgPos.SetXLabel(_T("Time[s]"));
	m_ntgVel.SetXLabel(_T("Time[s]"));
	m_ntgTorque.SetXLabel(_T("Time[s]"));

	m_ntgPos.SetYLabel(_T("Degree[deg]"));
	m_ntgVel.SetYLabel(_T("Velocity[deg/s]"));
	m_ntgTorque.SetYLabel(_T("Torque[Nm]"));

	m_ntgPos.AddElement();
	m_ntgPos.SetElementWidth(3);
	m_ntgPos.SetElementLineColor(RED);	// Target

	m_ntgPos.AddElement();
	m_ntgPos.SetElementWidth(3);
	m_ntgPos.SetElementLineColor(BLUE); // Current

	m_ntgPos.SetRange(0.0, 10.0, -360.0, 360.0);
	m_ntgPos.SetYGridNumber(4);

	m_ntgVel.AddElement();
	m_ntgVel.SetElementWidth(4);
	m_ntgVel.SetElementLineColor(RED);// Target

	m_ntgVel.AddElement();
	m_ntgVel.SetElementWidth(3);
	m_ntgVel.SetElementLineColor(BLUE); // Current

	m_ntgVel.SetRange(0.0, 10.0, -200.0, 200.0);
	m_ntgVel.SetYGridNumber(4);

	m_ntgTorque.AddElement();
	m_ntgTorque.SetElementWidth(4);
	m_ntgTorque.SetElementLineColor(RED);// Target

	m_ntgTorque.AddElement();
	m_ntgTorque.SetElementWidth(3);
	m_ntgTorque.SetElementLineColor(BLUE); // Current

	m_ntgTorque.SetRange(0.0, 10.0, -1.0, 1.0);	// y축 범위 -1 ~ 1
	m_ntgTorque.SetYGridNumber(4);

	SetTimer(1, 100, NULL);
}

BEGIN_MESSAGE_MAP(CGraphDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CGraphDlg 메시지 처리기입니다.


void CGraphDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	m_dCnt += 0.1;

	DataType_t jointData;
	ControlData_t motor_data_tar;
	ControlData_t motor_data_cur;

	GET_SYSTEM_MEMORY("JointData", jointData);
	GET_SYSTEM_MEMORY("Comm1Work_Controller_Target", motor_data_tar);	
	GET_SYSTEM_MEMORY("Comm1Work_Controller_Current", motor_data_cur);	

	if (m_dCnt >= 10.0)
	{
		m_ntgPos.SetRange(m_dCnt - 10.0, m_dCnt, -360.0, 360.0);
		m_ntgVel.SetRange(m_dCnt - 10.0, m_dCnt, -200.0, 200.0);
		m_ntgTorque.SetRange(m_dCnt - 10.0, m_dCnt, -1.0, 1.0);
	}

	// Position에 대한 Graph
	m_ntgPos.PlotXY(m_dCnt, jointData.Q_tar[0] * RAD2DEG, 1);	// target
	m_ntgPos.PlotXY(m_dCnt, motor_data_cur.position * RAD2DEG, 2);	// current

	// Velocity에 대한 Graph
	m_ntgVel.PlotXY(m_dCnt, motor_data_tar.velocity * RAD2DEG, 1);
	m_ntgVel.PlotXY(m_dCnt, motor_data_cur.velocity * RAD2DEG, 2);

	// Torque에 대한 Graph
	m_ntgTorque.PlotXY(m_dCnt, motor_data_tar.current * 0.0683, 1);	// torque constant
	m_ntgTorque.PlotXY(m_dCnt, motor_data_cur.current * 0.0683, 2);

	CDialogEx::OnTimer(nIDEvent);
}
