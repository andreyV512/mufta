// ---------------------------------------------------------------------------

#ifndef unTTubeDataH
#define unTTubeDataH
// ---------------------------------------------------------------------------

#include <System.hpp>
#include <Windows.hpp>
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstddef>
// #include<IniFiles.hpp>
#include "unTGlobalSettings.h"
#include "unTStorageZoneData.h"
// -----------
#include <Forms.hpp>
#include <Controls.hpp>
// #include "unTUtils.h"
// --------
#include <vector>
#include <iterator>
using namespace std;

// ---------------------------------------------------------------------------
// ����� ��� ���������  ������ ���������
class TTubeData {

public:
	TTubeData(int _countZones, int _firstSensorsThick, int _countSensorsThick,   int _firstSensorsCross, int _countSensorsCross, int _wdCountLOGchX502, int _wdFullSensorSize,
	TGlobalSettings* _programSettings,double *_wdZone_ADC_Range_Value);
	~TTubeData(void);

	// ---------------
	// ����������
	// vector<double> dt2;
	// TStorageZoneData dt;
	// ������ ���������� �� ������ ���
	vector<TZoneData*>vectZones;
	int wdFirstSensorsThick;
	int wdCountSensorsThick;
	int wdFirstSensorsCross; // ������ ������ ����������� � ������� �����
	int wdCountSensorsCross; // ���������� ��������
	int countZonesAll; // ���������� ���
	int wdFullSensorSize; // ������ ������ �� �������
	int countUsedZones; // ������� ��� � �������
	// ��������� �� ����� ���������
	TGlobalSettings* wdProgramSettings;

	// double *dataZone; // ������ �� ����
	// -----------------
	// �������
	// ������� ����� ���� � ���������
	// int _zoneNum ����� ����,
	// double* _adcData ������ �� ����,
	// int _adcDataSize ������ ����
	int AddNewZone(int _zoneNum, int _fullSensorSize, int _firstSensorsThick,int _countSensorsThick,int _firstSensorsCross, int _countSensorsCross, int _countLogCh,double *_wdZone_ADC_Range_Value);
	// �������� ������������ ����
	int AddPartZone(int _zoneNum, TZoneData* _zoneData, double* _adcData, int _needZoneAdd);
	// �������� ������ �� ���������� ����, �� ����������� �������
	// �������� ����� ����, ����� ������� � ����
	// �������� � �������� �������� ������, ���������
	// ���������� ��� ������
	int GetSensorArrayData(int _zoneNum, int _sensorNum, double* _arrayData, int _beginRead, int _needRead, AnsiString &_msg);
	// �������� � ��������� - ��� ����� ����������� ��������� � �����
	int SaveToFileTxt(AnsiString _fullFileName);
	// ��������� �� ����������
	int LoadFromFileTxt(AnsiString _fullFileName);
	// �������� � �������� � �������
	int SaveTubeToFileBin(AnsiString _fullFileName);
	// ��������� �� ��������� � �������
	int LoadTubeFromFileBin(AnsiString _fullFileName);
	// �������� ��� ������ � �������� �������� �������������
	int ResetAllData(AnsiString &_msg);
    //�������� ������ ������ �� ����� � ��������
	int TTubeData::TstResetData(AnsiString &_msg);
	// TfmViewZone* fmViewZone;
	// �������� ����������� �����
	int TTubeData::RefreshFile(AnsiString _fullFileName);

private:
	// static AnsiString applPath;
	int wdCountLOGchX502; // ���������� ��� ������� � �����
	double *wdZone_ADC_Range_Value;  //��������� �� �������� ���������� �� �������
	int Empty(void); // ��������

	AnsiString lastMsg;
};
#endif
