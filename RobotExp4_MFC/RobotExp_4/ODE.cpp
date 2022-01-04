#include "stdafx.h"
#include "ODE.h"
#include "SystemMemory.h"
#include "DataType.h"

#ifndef DRAWSTUFF_TEXTURE_PATH
#define DRAWSTUFF_TEXTURE_PATH "./../ode-0.13/drawstuff/textures"
#endif

#define GRAVITY 9.81
#define MAX_JOINT_NUM 3	// 원래 2 였음

#define DEG2RAD 0.0174533
#define RAD2DEG 57.2958

#define ROBOT_BODY_RADIUS 0.125f

dsFunctions g_Fn;

static dWorldID g_World;
static dSpaceID g_Space;
static dJointGroupID g_Contactgroup;

Object g_oObj[MAX_JOINT_NUM + 1];
static dJointID g_oJoint[MAX_JOINT_NUM + 1];

double g_tar_q[MAX_JOINT_NUM] = { 0.0, 0.0};
double g_cur_q[MAX_JOINT_NUM] = { 0.0, 0.0};

#define MAX_LINK_NUM 4

dGeomID g_Ground;	// 10-13 추가
dJointID g_Joint[MAX_JOINT_NUM];
Object g_Link[MAX_LINK_NUM];

void InitDrawStuff() {

	g_Fn.version = DS_VERSION;
	g_Fn.start = &StartDrawStuff;
	g_Fn.step = &SimLoopDrawStuff;
	g_Fn.command = &CommandDrawStuff;
	g_Fn.stop = StopDrawStuff;
	g_Fn.path_to_textures = DRAWSTUFF_TEXTURE_PATH;
}


void InitODE() {
	
	//10-13
	dInitODE();
	g_World = dWorldCreate();
	g_Space = dHashSpaceCreate(0);
	g_Contactgroup = dJointGroupCreate(0);
	dWorldSetGravity(g_World, 0, 0, -GRAVITY);	
	dWorldSetCFM(g_World, 1e-5);
	g_Ground = dCreatePlane(g_Space, 0, 0, 1, 0);

}


void RunODE(size_t width, size_t height) {

	// 10-13
	InitDrawStuff();
	InitODE();

	InitRobot();

	// run simulation
	dsSimulationLoop(0, 0, width, height, &g_Fn);

	StartDrawStuff();	// 시뮬레이션 상에서 카메라 위치
}



void ReleaseODE() {

	dJointGroupDestroy(g_Contactgroup);
	dSpaceDestroy(g_Space);
	dWorldDestroy(g_World);
	dCloseODE();
}



void StartDrawStuff() {

	// 10-13
	float dPos[3] = { 0.0, 0.0, 4.0 };	// X Y Z  좌표
	float dRot[3] = { 90.0, -90.0, 0.0 };	// Roll Pitch Yaw
	dsSetViewpoint(dPos, dRot);


}


void SimLoopDrawStuff(int pause) 
{
	// 11-10 추가
	DataType_t jointData;
	GET_SYSTEM_MEMORY("JointData", jointData);

	g_tar_q[0] = jointData.Q_tar[0];
	g_tar_q[1] = jointData.Q_tar[1];



	// 360 degree가 넘거나  -360 degree를 넘지 않으면 해주는 예외처리
	g_tar_q[0] = g_tar_q[0] * RAD2DEG;

	int quotient = (int)((g_tar_q[0]) / (360.0));

	double saturation = (double)(360.0 * quotient);

	g_tar_q[0] -= saturation;

	g_tar_q[0] = g_tar_q[0] * DEG2RAD;	// 예외처리 된 값을 그대로 대입

	jointData.Q_cur[0] = g_cur_q[0];
	jointData.Q_cur[1] = g_cur_q[1];

	jointData.Q_tar[0] = g_tar_q[0];
	jointData.Q_tar[1] = g_tar_q[1];

	SET_SYSTEM_MEMORY("JointData", jointData);
	
	// 10-13 추가
	PControl();
	double dt = 0.01;
	dWorldStep(g_World, dt);

	dsSetColor(1., 0., 0.);
	// position, rotation, length, radius
	dsDrawCapsuleD(dBodyGetPosition(g_Link[0].body),
		dBodyGetRotation(g_Link[0].body),
		0.5, 0.125);

	dsSetColor(0., 1., 0.);
	// position, rotation, length, radius
	dsDrawCapsuleD(dBodyGetPosition(g_Link[1].body),
		dBodyGetRotation(g_Link[1].body),
		1.0, 0.125);

	dsSetColor(0., 0., 1.);
	// position, rotation, length, radius
	dsDrawCapsuleD(dBodyGetPosition(g_Link[2].body),
		dBodyGetRotation(g_Link[2].body),
		0.5, 0.125);
}


void CommandDrawStuff(int cmd) {

	//TO DO

}


void StopDrawStuff() {

	
}


void InitRobot()
{
		
	// 11-10
	dMass mass;
	dMatrix3 R;

	dReal x[MAX_LINK_NUM] = { 0.00, 0.50, 1.25 };
	dReal y[MAX_LINK_NUM] = { 0.00, 0.00, 0.00 };
	dReal z[MAX_LINK_NUM] = { 0.25, 0.50, 0.50 };

	dReal ori_x[MAX_LINK_NUM] = { 0.00, 0.00, 0.00 };
	dReal ori_y[MAX_LINK_NUM] = { 0.00, 1.00, 1.00 };
	dReal ori_z[MAX_LINK_NUM] = { 1.00, 0.00, 0.00 };
	dReal ori_q[MAX_LINK_NUM] = { 0.00, 90 * DEG2RAD, 90 * DEG2RAD };

	dReal length[3] = { 0.50, 1.00, 0.50 };
	dReal weight[3] = { 0.50, 1.00, 0.50 };

	dReal radius[3];

	dReal c_x[3] = { 0.00,0.00,1.00 };
	dReal c_y[3] = { 0.00,0.00,0.00 };
	dReal c_z[3] = { 0.00,0.50,0.50 };

	dReal axis_x[3] = { 0.00,0.00,0.00 };
	dReal axis_y[3] = { 0.00,0.00,0.00 };
	dReal axis_z[3] = { 1.00,1.00,1.00 };

	for (int i = 0; i < 3; i++) {
		radius[i] = ROBOT_BODY_RADIUS;
	}

	// 힌지 Joint 연결
	for (int i = 0; i < 3; i++) {
		g_Link[i].body = dBodyCreate(g_World);
		dBodySetPosition(g_Link[i].body, x[i], y[i], z[i]);
		dMassSetZero(&mass);
		dMassSetCapsuleTotal(&mass, weight[i], 1, radius[i], length[i]);

		dBodySetMass(g_Link[i].body, &mass);
		g_Link[i].geom = dCreateCapsule(g_Space, radius[i], length[i]);
		dGeomSetBody(g_Link[i].geom, g_Link[i].body);

		dRFromAxisAndAngle(R, ori_x[i], ori_y[i], ori_z[i], ori_q[i]);
		dBodySetRotation(g_Link[i].body, R);
	}

	// 0번 고정 포인트
	g_Joint[0] = dJointCreateFixed(g_World, 0);
	dJointAttach(g_Joint[0], 0, g_Link[0].body);
	dJointSetFixed(g_Joint[0]);

	for (int i = 1; i < 3; i++) {
		g_Joint[i] = dJointCreateHinge(g_World, 0);
		dJointAttach(g_Joint[i], g_Link[i-1].body, g_Link[i].body);
		dJointSetHingeAnchor(g_Joint[i], c_x[i], c_y[i], c_z[i]);
		dJointSetHingeAxis(g_Joint[i], axis_x[i], axis_y[i], axis_z[i]);	//회전축
	}
}

void PControl()
{
	dReal dKp = 10, dMax = 100.0;
	dReal dError_Q[MAX_JOINT_NUM];

	DataType_t jointData;
	GET_SYSTEM_MEMORY("JointData", jointData);

	g_tar_q[0] = jointData.Q_tar[0];
	g_tar_q[1] = jointData.Q_tar[1];

	for (int i = 1; i < MAX_JOINT_NUM; i++)
	{
		g_cur_q[i - 1] = dJointGetHingeAngle(g_Joint[i]);

		if (g_tar_q[i - 1] - g_cur_q[i - 1] > 180.0 * DEG2RAD)
		{
			g_cur_q[i - 1] += 359.9 * DEG2RAD;
		}

		if (g_tar_q[i - 1] - g_cur_q[i - 1] < -180.0 * DEG2RAD)
		{
			g_cur_q[i - 1] -= 359.9 * DEG2RAD;
		}

		dError_Q[i - 1] = g_tar_q[i - 1] - g_cur_q[i - 1];

		dJointSetHingeParam(g_Joint[i], dParamVel, dKp*dError_Q[i - 1]);
		dJointSetHingeParam(g_Joint[i], dParamFMax, dMax);
	}

	jointData.Q_cur[0] = g_cur_q[0];
	jointData.Q_cur[1] = g_cur_q[1];

	SET_SYSTEM_MEMORY("JointData", jointData);

}