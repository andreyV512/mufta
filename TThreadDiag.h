//---------------------------------------------------------------------------

#ifndef TThreadDiagH
#define TThreadDiagH
//---------------------------------------------------------------------------
#ifdef BERLIN_10
  #include <VclTee.TeeGDIPlus.hpp>
  #include <VCLTee.Chart.hpp>
  #include <VCLTee.TeEngine.hpp>
  #include <VCLTee.TeeProcs.hpp>
  #include <VCLTee.TeCanvas.hpp>
  #include <VCLTee.Series.hpp>
  #include <System.IOUtils.hpp>
#else
  #include <Classes.hpp>
  #include "Chart.hpp"
  #include "TeEngine.hpp"
  #include "TeeProcs.hpp"
#endif
//---------------------------------------------------------------------------
#include "TLCard791.h"
#include "TGSPF052.h"
#include <ctime.h>
#include <vector>
//------------------

 #include "TLCardData.h"

//---------------------
#include "SignalListDef.h"
//---------------------
using namespace std;

class TThDiag : public TThread
{
private:
 TChart *thChartGPSF052;

 TLCard791* thLСard791;

 TGSPF052* thGen;
 double Y0,Y1,Y3,Y5;
 unsigned int startTime;// начальное время
 unsigned int currTime;
 unsigned int elapsTime;
 int chCount;
 unsigned int timeGen;
protected:
	void __fastcall Execute();
	void __fastcall UpdateChart();
public:

	__fastcall TThDiag(bool CreateSuspended,TChart *_chartGPSF052,TLCard791* _thLСard791,TGSPF052* _Gen,int _chCount,int _timeGen);

	__fastcall ~TThDiag(void);
	bool stopThread;
	//количество измерений
	int countMeasure;
	vector<double*> vecMeasure;
};
//---------------------------------------------------------------------------
#endif
