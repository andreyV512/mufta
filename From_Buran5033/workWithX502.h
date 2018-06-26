// ---------------------------------------------------------------------------

#ifndef workWithX502H
#define workWithX502H
// ---------
#include <System.hpp>
#include <SyncObjs.hpp>
#include <Classes.hpp>
#include <vector>
// ---------------------------------------------------------------------------
// ----------
#include "unTGlobalSettings.h"
//#include <math.h>
// ---------
#define strStatus "������: "

using namespace std;

// class workX502 : public TWorkX502(TObject owner, t_x502_hnd pHndX502);
class TWorkX502 {
public:
	// User declarations
	TWorkX502(void);
	~TWorkX502(void);

	// --------------
	t_x502_hnd hndX502; // ��������� ������
	bool threadRunning; // �������, ������� �� ����� ����� ������
	X502_ProcessThread* threadX502; // ����� ��� ����������� �����
	double kadr_data0[X502_LTABLE_MAX_CH_CNT]; // ������ ��� ����� �� �������� ���������� ������� ���
	// vector<double> dataThick;
	// �����������
	// double kadr_data1[X502_LTABLE_MAX_CH_CNT]; // ������ ��� ����� �� �������� ���������� ������� ���
	// double kadr_data2[X502_LTABLE_MAX_CH_CNT]; // ������ ��� ����� �� �������� ���������� ������� ���
	// double kadr_data3[X502_LTABLE_MAX_CH_CNT]; // ������ ��� ����� �� �������� ���������� ������� ���
	// double kadr_data4[X502_LTABLE_MAX_CH_CNT]; // ������ ��� ����� �� �������� ���������� ������� ���
	int syncDIN0; // ������ ��������� ����� 1 ���� ����������� ������
	// double strDinRes; // ������ ��������� ������ �������
	DynamicArray<t_x502_devrec>devrecList; // ������ ���������
	bool haveX502; // ������� ����� �502
	// ��������� �����
	bool isDacPresent; // ������� ������� �������������� ���������� ���
	bool isGalPresent; // ������� ������� ����������������
	bool isDspPresent; // ������� ������� ����������� ���������� BlackFin
	bool isEthPresent; // �������, ��� ���������� ������������ ��������� PCI/PCI-Express
	AnsiString strPldaVer; // ������ ���� ������
	AnsiString strFpgaVer; // ������ ����
	AnsiString strMcu_firmware_ver; // ������ ��������
	// ��������� ������
	int x502AdcFreq; // ������� ����������� ����� ��� ��� ����� ���������� �������
	int x502DinFreq; // ������� ����������� ����� ��������
	int x502AdcFreqLch; // ������� ����������� ����� ���  �� �����
	// --------
	int x502ChMode; // ������� ������ ������ ������� ����������������/� ����� ������
	int x502CountDev; // ���������� ������������ ����
	int x502EnableSyncADC; // ������� ������ ����� ����������/����������� c ���
	// X502_SYNC_INTERNAL = 0, < ���������� ������
	// X502_SYNC_DI_SYN2_RISE = 6, < �� ������ ������� DI_SYN2
	int x502SyncMode; // ������� ������ ������������� - ����������/������� (� ��� ������ ����������) ���, ���
	// X502_SYNC_DI_SYN2_RISE = 6, < �� ������ ������� DI_SYN2
	int x502SyncStartMode; // ������� ������� ����������� �����/������ ������

	bool x502EnableSyncDin; // ��������� ��� ��������� ����� ����������� ����� � �������� ������
	int x502FLAGS_VOLT; // �������� � ������� ��� � ������� ���
	// int x502ADC_RANGE_V; // �������� ��������� ��� ������ ��� � �������
	// int x502ADC_RANGE_VX[32]; // ��������� ��������� ��� ������� ��� � �������
	// int x502ADC_RANGE_INT; // �������� ��������� ��� ������ ��� � INT64
	int x502CountFIZch; // ���������� ��� ������� �������� 32
	int x502CountLOGch; // ���������� ��� �������  �������� 256, �� ��������� ���� � ���� ������ ��� 32-�!!!
	bool isX502Opened; // ����� ��� ���� ������������
	uint32_t realAdcFreq; // �������� ������� �����
	//�������� ������
	int  valuesIN[32];
	// �������� �� ������
	int valuesOUT[32];
	// ��������� �� ���������
	TGlobalSettings *wwProgramSettings;
	//TObject* owner;
	// ��������� �� ��������� ������ ����������� ������
	TTubeData* w502TubeData;
	AnsiString applPath;
	// ������ ������������� ������ ��� �� ����� � ���������� (��� �� �����!)
	unsigned int x502ADCBufSensorSize;
	// ������ ������������� ������ �������� ������ �� ����� � ���������� (��� �� �����!)
	unsigned int x502DinBufSize;

	// �������
	__fastcall void OnThreadTerminate(TObject *Sender);
	// void OnThreadTerminate();
	// ��� IP ����������
	void OpenByIP(AnsiString _strIP);
	// ----------------------
	void UpdateControls(); // ���������� ����������
	int GetDevInfo(void); // ���� �� �����
	// ������� ������
	void __fastcall StopThread();
	void FreeDevList();
	void RefreshDeviceList(int &_CountDev);
	void ShowDevInfo();
	// ��������� ���������� ����� ������
	int SetupParams(int _�ountCh, int _SyncMode, int _SyncStartMode);
	// ������������� ��������� ������� �����
	int SetAdcFreq(int &_x502AdcFreq, int &_x502AdcFreqLch, int &_x502DinFreq);
	// ������ ������ ������
	int StartRead(bool _enableSYNCDin, AnsiString &_msg);
	// ������������� ���-�� ���������� �������, ����� ��������� ���������� ������� �� �����
	int AdcFreqChange(int _ChCnt, AnsiString &_msg);
	// ��������� ��� ��������� ����� ����������� �����
	int SetSyncDinStream(bool _EnableSyncDin);
	// ����������/������� ����������� ����� ��������
	int SetSyncDin(AnsiString &_msg);
	// ������� �������� ���������� � ��������� �������� ������� ��� �������� ���,
	// ���� ��� ��� ���� �������
	int X502OpenClose(int _numDev, AnsiString &_msg);
	// ������������ ������ �� �������� �����
	int SetAsyncDigOut(int _AsyncDigOut, AnsiString &_msg);
	// ������������ ����� � �������� �����
	int GetAsyncDigIn(AnsiString &_msg);
	// ������� ����� �������� ������
	// ����������� �������� �����
	int GetAsyncValuesDigIn(int _valuesIN[16], AnsiString &_msg);
	// �������� ������ �������� �������
	int SetAsyncValuesDigOUT(int _valuesOUT[16], AnsiString &_msg);
	// ����� �� ��� �������� 5 �
	int SetAsyncDac1(double _valOut, AnsiString &_msg);
	int SetAsyncDac2(double _valOut, AnsiString &_msg);
	int SetAsyncDac(int _numADC, double _valOut, AnsiString &_msg);
	// �������� ���� � �������
	int GetAsyncAdcFrame(AnsiString &_msg);
	void InfDevListChange();
	void ChCntChange(int _ChCnt);
	// ������ � �������� ���������� ����� SQL
	void LoadX502Settings(AnsiString _pathUDL, AnsiString &_msg);
	void SaveX502Settings(AnsiString _pathUDL, AnsiString &_msg);
	// ��������� �������� ������ � 1
	// �� E502 �� �������������! ������ �� L502
	void SetDITo1(int _typeCard, bool _lowBytes, bool _hiBytes);
	// -------------------------
	void CloseDevice(AnsiString &_msg);
	int GetDataAsync(AnsiString &_msg);
	// �������� ������� �������� ������� �������
	int X502GetRefFreqValue(double _freq);
	int X502CreatePrepareWorkData(AnsiString &_msg);
	// �������� �������� �������� �� ������ �� ����������
	int GetVoltMaxLCh(int _numLCh);
	// ������ ��������� �������
	int StartDrive(double _speed, int _codeDOutDrive, AnsiString &_msg);
	// ���� ��������� �������
	int StopDrive(int _codeDOut, AnsiString &_msg);
	// ��������� ����� ��� ������ �������
	int AlarmSituiation(AnsiString &_msg);
	// ����� �������
	int SYNK_X502StreamsStart(t_x502_hnd _hndX502);
	// ���� �������
	int SYNK_X502StreamsStop(t_x502_hnd _hndX502);
	SetADCRange(AnsiString _strRange, int _ind);
	int GetADCRange(int _ind);
	double GetADCRangeValue(int _ind);
    //������ ������� �����
	int ClearValuesDig(int _values[16]);
	int CheckCurrentDrive(int _numZone, int _sensDrvNum, int _firstValue, int _sizeValues, double _valueNominal,double _valueMax) ;
    	// ������ �������� ������� ��� ������� � ���������
	double adc_Range_Value[X502_LTABLE_MAX_CH_CNT];
private:
	// ������ ������ � IP
	int ParseIpAddr(AnsiString _addr);

	// ������ ��� ������ ���������
	AnsiString msgLast;
	// ������ �������� ������� ��� �502
	int adc_Range[X502_LTABLE_MAX_CH_CNT];

};
// extern TWorkX502* workX502;
#endif
