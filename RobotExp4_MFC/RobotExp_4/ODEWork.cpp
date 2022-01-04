//////////////////////////////////////////////////////////////////////////////////////////////////////////
// File		: ODEWork.cpp
// Version	: 1.0.1
// Date		: 2019.08.22
// Writer	: Lee, Seungmin (CDSL)
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//include//
#include "stdafx.h"
#include "ODEWork.h"
#include "ODE.h"


CODEWork::CODEWork(std::string name) :
	CWorkBase(name),
	_poSharedMemory(0),
	_width(640), _height(480)
{

}


CODEWork::~CODEWork()
{

}



void CODEWork::SetWindowSize(size_t width, size_t height) {

	_width = width;
	_height = height;
}



void CODEWork::SetSharedMemory(CSyncSharedMemory* poMemory) {

	_poSharedMemory = poMemory;
}



void CODEWork::_execute() {

	RunODE(_width, _height);
}