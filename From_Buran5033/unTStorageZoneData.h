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

// класс для хранения данных по зонам
class TZoneData {
public:
	//TZoneData(int _zoneNum, int _fullSensorSize, int _firstSensorsThick, int _countSensorsThick,int _firstSensorsCross, int _countSensorsCross,int _countLogCh);
	TZoneData(TGlobalSettings* _zProgramSettings,int _zoneNum,int _fullSensorSize,int _countLogCh, double *_zdADC_Range_Value);
	~TZoneData(void);

	// ---------
	int zoneNum; // номер зоны
	bool isUsed; // 0- пустая, 1 - есть данные
	//int countSensorsCross; // количество сенсоров поперечника  из настроек
	//int firstSensorsCross; // первый датчик поперечника в массиве кадра из настроек
	//int countSensorsThick; // количество сенсоров толщиномера  из настроек
	//int firstSensorsThick; // первый датчик толщиномера в массиве кадра из настроек
	int countLogCh; // количество лог каналов в кадре из настроек
    int countSensors;//количество датчиков в зоне
	int fullZoneSize; // полный размер зоны данных в double  размер данных на канал * число каналов
	int realZoneSize; // актуальный размер зоны данных в double
	double *arrDataZone; // данные по зоне
	double *zdADC_Range_Value;  //указатель на значения напряжений по каналам
	int statusZone;// // 0 - годно. 1 - брак, 2-брак 2 кт,
	// данные по датчикам вектор указателей на классы по датчикам
	vector<TSensorData*>vectSensors;
	// распределено или нет
	bool distribute;
	//указатель на настройки
	TGlobalSettings* zProgramSettings;

	// функции
	// распределим данные по датчикам
	int DistributeSensorsData(void);
	//погоним сенсоры на брак
	int SetStatusSensors(double _crossBorder10, double _crossBorder20, double _thickBorder10, double _thickBorder20);
	// очистим данные в 0 по зоне и датчикам, сбросим часть признаков
	int ResetZone(void);

};

// класс для хранения данных по датчикам
class TSensorData {
public:
	// --------- переменные
	int sensorNum;
	int fullSensorSize; // полный размер данных в по датчику double
	int realSensorSize; // текущий (актуальный) размер
	int stastusSensor; // 0 - годно. 1 - брак, 2-брак 2 кт, <0 - не задействован для измерений
	vector<double>vecSensorData;
	int countLogCh;
	// -------
	// конструктор классса
	// _dataZone - данные по зоне
	// _blockZoneSize размер добавляемого блока зоны
	// _countNeedAdd сколько необходимо добавить элементов в данные датчика
	// _sensorNum номер датчика
	// общее количество датчиков по зоне  _countLogCh

	TSensorData(int _sensorNum, int _fullSensorSize, int _countLog);
	// добавить в сенсор данные
	int SensorDataAdd(double* _dataZoneBlock, int _blockZoneSize, int _countNeedAdd, AnsiString &_msg);
	~TSensorData(void);
	// функции
	// очистим данные в 0 , сбросим часть признаков
	int ResetSensor(void);
    //заготовка - пустышка
	int Empty(void);
};

// ----------------
#endif
