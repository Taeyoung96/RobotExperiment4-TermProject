#include "stdafx.h"
#include "DataType.h"
#include "CommWork.h"
#include "SystemMemory.h"

CCommWork::CCommWork(std::string name)
	:CWorkBase(name)
{
	memset(&_target, 0, sizeof(ControlData_t));
	memset(&_current, 0, sizeof(ControlData_t));

	_memname_tar = name + "_Controller_Target";
	_memname_cur = name + "_Controller_Current";
	CREATE_SYSTEM_MEMORY(_memname_tar, ControlData_t);
	CREATE_SYSTEM_MEMORY(_memname_cur, ControlData_t);

	_sendPacket.data.header[0] = _sendPacket.data.header[1] = _sendPacket.data.header[2] = _sendPacket.data.header[3] = 0xFF;
	_sendPacket.data.id = 1;
	_sendPacket.data.mode = 2;
	_sendPacket.data.size = sizeof(Packet_t);
}


CCommWork::~CCommWork() {

	DELETE_SYSTEM_MEMORY(_memname_tar);
	DELETE_SYSTEM_MEMORY(_memname_cur);

	ClosePort();
}



bool CCommWork::OpenPort(std::string name, int baudRate) {

	return _comm.Open(name.c_str(), baudRate);
}



void CCommWork::ClosePort() {

	_comm.Close();
}



void CCommWork::_execute() {

	GET_SYSTEM_MEMORY(_memname_tar, _target);

	static int mode, readSize = 0, checkSize;
	static unsigned char check;

	if (_comm.isOpen())
	{
		_sendPacket.data.check = 0;
		_sendPacket.data.pos = _target.position * 1000;
		_sendPacket.data.velo = _target.velocity * 1000;
		_sendPacket.data.cur = _target.current * 1000;

		for (int i = 8; i < sizeof(Packet_t); i++)
		{
			_sendPacket.data.check += _sendPacket.buffer[i];
		}

		_comm.Write((char*)_sendPacket.buffer, sizeof(Packet_t));

		readSize = _comm.Read((char*)_recvBuf, 4096);

		for (int i = 0; i < readSize; i++)
		{
			switch (mode)
			{
			case 0:
				if (_recvBuf[i] == 0xFE)
				{
					checkSize++;
					if (checkSize == 4)
					{
						mode = 1;
					}
				}
				else
				{
					checkSize = 0;
				}
				break;

			case 1:

				_packet.buffer[checkSize++] = _recvBuf[i];

				if (checkSize == 8)
				{
					mode = 2;
				}
				break;

			case 2:

				_packet.buffer[checkSize++] = _recvBuf[i];
				check += _recvBuf[i];

				if (checkSize == _packet.data.size)
				{
					if (check == _packet.data.check)
					{
						printf("pos : %d, velo : %d, cur : %d\n",
							_packet.data.pos,
							_packet.data.velo,
							_packet.data.cur);

						_current.position = _packet.data.pos / 1000;
						_current.velocity = _packet.data.velo / 1000;
						_current.current = _packet.data.cur / 1000;
					}

					// variable initialize
					check = 0;
					mode = 0;
					checkSize = 0;
				}
			}
		}
	}

	

	SET_SYSTEM_MEMORY(_memname_cur, _current);
}
