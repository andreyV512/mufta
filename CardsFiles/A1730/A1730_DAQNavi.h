//---------------------------------------------------------------------------
#ifndef A1730_DAQNaviH
#define A1730_DAQNaviH
//#ifdef A1730_DAQNaviR
//---------------------------------------------------------------------------
#include "A1730.h"
#pragma warn -8060
#include "bdaqctrl.h"
#pragma warn .8060
#include<SyncObjs.hpp>
using namespace Automation::BDaq;

//---------------------------------------------------------------------------
class A1730_DAQNavi:public A1730
{
private:
	InstantDiCtrl* instantDiCtrl;
	InstantDoCtrl* instantDoCtrl;
public:
	A1730_DAQNavi(int _DevNum);
	~A1730_DAQNavi(void);
	//--------
	DWORD Read(void);
	DWORD ReadOut(void);
	void Write(DWORD _v);
	void WriteSignals(void);
	void ReadSignals(void);
};
//---------------------------------------------------------------------------
//#endif
#endif


