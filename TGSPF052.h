//---------------------------------------------------------------------------

#ifndef TGSPF052H
#define TGSPF052H
//---------------------------------------------------------------------------
//#include <RshApi.h>

//#ifdef RSH_MSWINDOWS
//	#include <conio.h>
//#endif
//#include <iomanip>
#include <System.hpp>
#include "TGlobalSettings.h"
#include <RshApi.h>
#include "unTUtils.h"
#include "unTExtFunction.h"

//#include <RshApi.cpp>
//#include "Rsh.h"
//#include "RshTypes.h"
//#include "RshMacro.h"
//#include "RshConsts.h"
//#include "RshStatusCodes.h"
//#include "IRshFactory.h"
//#include "IRshDevice.h"
//#include "RshDllClient.h"
//#include "RshError.h"
//#include "RshEventManager.h"


//#include <IniFiles.hpp>
//================================���������==================================
class TGSPF052
{
public:
	// ������������� �����
	//GSPF052(TIniFile* _ini);
	TGSPF052(TGlobalSettings* _globalSettings,int &_err);
    ~TGSPF052(void);
	// ������������ �������
	int FormSignal(int _valueFrec,double _valueAmpl);
	// ��������� ��������� �������
	int StartGSPF052();
	// ���� ����������
	int WaitInterrupt();
	// ������������� ��������� �������
	int StopGSPF052();
	// �������� �����
	int CloseCard(AnsiString desc);
	bool isStartedGSPF052;
	// ����� ��������� � ��������
	void toProtocol(AnsiString _msg);
	// ����� ��� �������� ���������� ����������.
	RshDllClient* rshClient;
	// ���������� ������� �������������
	void SetSampleFreq(double _SampleFreq);
private:
	// ������� ������������� ������ � ������
	double SignalFreq;
	// ��������� ������������� ������ � �������
	double SignalAmplitude;
		// ������� �������������
	double SampleFreq;
	// ���� � �����������
	TGlobalSettings* gspfGlobalSettings;
	// ��������� �������������
	RshInitGSPF gspfPar;
	// ��������� ��� ����������, � ������� ����� �������� ���������.
	char* BoardName;
	// ����� ����������. ���� � ������� ����������� ��������� ���������� ���������, ����� �������, � ������ �� ��� ������������. ��������� ���������� � 1.
	U32 DeviceID;
	// ���� ������ ������. (� ������ ������� � ���������� ��������� ���������� ����)
	RSH_LANGUAGE ErrorLanguage;
	//const RSH_LANGUAGE ErrorLanguage = RSH_LANGUAGE_ENGLISH;
	// 1��� ������������� ��������. L ������ ��������� ������������ ��� ������� ��������� ���������� ����������.
	double OutR;

	//��������� �� ������ � ����������� IRshDevice, ������� ������������ ��� ������� � ����������.
	IRshDevice* rshDevice;
	// ������������ ���������
	double DACMaxAmp;


//	static const AnsiString StringDelimiter = "----------";
//	static const AnsiString StringSuccess = "SUCCESS!";

};
//char* BoardName = "GSPF052PCI";
//double GSPF052::SampleFreq = 10000000.0;
//double GSPF052::SignalFreq = 1000.0;
//double GSPF052::SignalAmplitude = 1.0;
//double GSPF052::OutR = 1000000.0f;

#endif
