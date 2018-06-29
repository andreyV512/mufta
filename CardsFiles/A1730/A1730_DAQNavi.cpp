// ---------------------------------------------------------------------------
#pragma hdrstop
//#ifdef A1730_DAQNaviR
#include "A1730_DAQNavi.h"
#include <SysUtils.hpp>
#pragma package(smart_init)

// ---------------------------------------------------------------------------
A1730_DAQNavi::A1730_DAQNavi(int _DevNum)
{
#ifndef TVIRTLCARD791
	ErrorCode errorCode;
	instantDiCtrl = AdxInstantDiCtrlCreate();
	instantDoCtrl = AdxInstantDoCtrlCreate();
	DeviceInformation devInfo(_DevNum);
	errorCode = instantDiCtrl->setSelectedDevice(devInfo);
	if (BioFailed(errorCode))
		throw(Exception("A1730_DAQNavi::A1730_DAQNavi: не смогли открыть плату Advantech1730"));
	errorCode = instantDoCtrl->setSelectedDevice(devInfo);
	if (BioFailed(errorCode))
		throw(Exception("A1730_DAQNavi::A1730_DAQNavi: не смогли открыть плату Advantech1730"));
#endif
}

// ---------------------------------------------------------------------------
A1730_DAQNavi::~A1730_DAQNavi(void)
{
#ifndef TVIRTLCARD791
	instantDiCtrl->Dispose();
	instantDoCtrl->Dispose();
#endif
}

// ---------------------------------------------------------------------------
DWORD A1730_DAQNavi::Read(void)
{
#ifndef TVIRTLCARD791
	BYTE buf[4];
	ErrorCode errorcode = instantDiCtrl->Read(0, 4, buf);
	if (errorcode != Success)
		throw(Exception("A1730_DAQNavi::Read: не могу прочитать плату"));
	DWORD v = buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24);
	return (v);
#else
return 0;
#endif
}

// ---------------------------------------------------------------------------
DWORD A1730_DAQNavi::ReadOut(void)
{
#ifndef TVIRTLCARD791
	BYTE buf[4];
	ErrorCode errorcode = instantDoCtrl->Read(0, buf[0]);
	if (errorcode != Success)
		throw(Exception("A1730_DAQNavi::Read: не могу прочитать плату"));
	DWORD v = buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24);
	return (v);
	#else
	return 0;
#endif
}

// ---------------------------------------------------------------------------
void A1730_DAQNavi::Write(DWORD _v)
{
#ifndef TVIRTLCARD791
	BYTE buf[4];
	buf[0] = _v & 0xFF;
	buf[1] = _v >> 8 & 0xFF;
	buf[2] = _v >> 16 & 0xFF;
	buf[3] = _v >> 24 & 0xFF;
	ErrorCode errorcode = instantDoCtrl->Write(0, 4, buf);
	if (errorcode != Success)
		throw(Exception("A1730_DAQNavi::Write: не могу записать на плату"));
#endif
}
// ---------------------------------------------------------------------------

