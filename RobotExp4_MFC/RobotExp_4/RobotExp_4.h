
// RobotExp_4.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CRobotExp_4App:
// �� Ŭ������ ������ ���ؼ��� RobotExp_4.cpp�� �����Ͻʽÿ�.
//

class CRobotExp_4App : public CWinApp
{
public:
	CRobotExp_4App();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CRobotExp_4App theApp;