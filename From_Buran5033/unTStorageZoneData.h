// ---------------------------------------------------------------------------

#ifndef unTStorageZoneDataH
#define unTStorageZoneDataH
// ---------------------------------------------------------------------------
#include <System.hpp>
// --------
#include <vector>
#include <iterator>
#include "unTGlobalSettings.h"
//#include "e502api.h"
#include "x502api.h"
using namespace std;
class TSensorData;

// ����� ��� �������� ������ �� �����
class TZoneData {
public:
	//TZoneData(int _zoneNum, int _fullSensorSize, int _firstSensorsThick, int _countSensorsThick,int _firstSensorsCross, int _countSensorsCross,int _countLogCh);
	TZoneData(TGlobalSettings* _zProgramSettings,int _zoneNum,int _fullSensorSize,int _countLogCh, double *_zdADC_Range_Value);
	~TZoneData(void);

	// ---------
	int zoneNum; // ����� ����
	bool isUsed; // 0- ������, 1 - ���� ������
	//int countSensorsCross; // ���������� �������� �����������  �� ��������
	//int firstSensorsCross; // ������ ������ ����������� � ������� ����� �� ��������
	//int countSensorsThick; // ���������� �������� �����������  �� ��������
	//int firstSensorsThick; // ������ ������ ����������� � ������� ����� �� ��������
	int countLogCh; // ���������� ��� ������� � ����� �� ��������
    int countSensors;//���������� �������� � ����
	int fullZoneSize; // ������ ������ ���� ������ � double  ������ ������ �� ����� * ����� �������
	int realZoneSize; // ���������� ������ ���� ������ � double
	double *arrDataZone; // ������ �� ����
	double *zdADC_Range_Value;  //��������� �� �������� ���������� �� �������
	int statusZone;// // 0 - �����. 1 - ����, 2-���� 2 ��,
	// ������ �� �������� ������ ���������� �� ������ �� ��������
	vector<TSensorData*>vectSensors;
	// ������������ ��� ���
	bool distribute;
	//��������� �� ���������
	TGlobalSettings* zProgramSettings;

	// �������
	// ����������� ������ �� ��������
	int DistributeSensorsData(void);
	//������� ������� �� ����
	int SetStatusSensors(double _crossBorder10, double _crossBorder20, double _thickBorder10, double _thickBorder20);
	// ������� ������ � 0 �� ���� � ��������, ������� ����� ���������
	int ResetZone(void);

};

// ����� ��� �������� ������ �� ��������
class TSensorData {
public:
	// --------- ����������
	int sensorNum;
	int fullSensorSize; // ������ ������ ������ � �� ������� double
	int realSensorSize; // ������� (����������) ������
	int stastusSensor; // 0 - �����. 1 - ����, 2-���� 2 ��, <0 - �� ������������ ��� ���������
	vector<double>vecSensorData;
	int countLogCh;
	// -------
	// ����������� �������
	// _dataZone - ������ �� ����
	// _blockZoneSize ������ ������������ ����� ����
	// _countNeedAdd ������� ���������� �������� ��������� � ������ �������
	// _sensorNum ����� �������
	// ����� ���������� �������� �� ����  _countLogCh

	TSensorData(int _sensorNum, int _fullSensorSize, int _countLog);
	// �������� � ������ ������
	int SensorDataAdd(double* _dataZoneBlock, int _blockZoneSize, int _countNeedAdd, AnsiString &_msg);
	~TSensorData(void);
	// �������
	// ������� ������ � 0 , ������� ����� ���������
	int ResetSensor(void);
    //��������� - ��������
	int Empty(void);
};

// ----------------
#endif
