/////////////////////////////////////////////////////////////////////////////////////////////////////////
// File		: ODE.h
// Version	: 1.0.1
// Date		: 2019.08.22
// Writer	: Lee, Seungmin (CDSL)
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#define dDOUBLE

#pragma comment(lib, "ode_doubled.lib")
#pragma comment(lib, "drawstuffd.lib")

//include//
#include <drawstuff\drawstuff.h>
#include <ode/ode.h>

#include <vector>

typedef struct
{
	dBodyID body;
	dGeomID geom;
}Object;

void InitDrawStuff();

void InitODE();
void RunODE(size_t width = 640, size_t height = 480);
void ReleaseODE();


void StartDrawStuff();
void SimLoopDrawStuff(int pause);
void CommandDrawStuff(int cmd);
void StopDrawStuff();

void InitRobot();
void PControl();