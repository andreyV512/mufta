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
// класс для обработки  данных измерений
class TTubeData {

public:
	TTubeData(int _countZones, int _firstSensorsThick, int _countSensorsThick,   int _firstSensorsCross, int _countSensorsCross, int _wdCountLOGchX502, int _wdFullSensorSize,
	TGlobalSettings* _programSettings,double *_wdZone_ADC_Range_Value);
	~TTubeData(void);

	// ---------------
	// переменные
	// vector<double> dt2;
	// TStorageZoneData dt;
	// вектор указателей на классы зон
	vector<TZoneData*>vectZones;
	int wdFirstSensorsThick;
	int wdCountSensorsThick;
	int wdFirstSensorsCross; // первый датчик поперечника в массиве кадра
	int wdCountSensorsCross; // количество сенсоров
	int countZonesAll; // количество зон
	int wdFullSensorSize; // размер данных по датчику
	int countUsedZones; // сколько зон с данными
	// указатель на общие настройки
	TGlobalSettings* wdProgramSettings;

	// double *dataZone; // данные по зоне
	// -----------------
	// функции
	// создать новую зону с датчиками
	// int _zoneNum номер зоны,
	// double* _adcData данные по зоне,
	// int _adcDataSize размер зоны
	int AddNewZone(int _zoneNum, int _fullSensorSize, int _firstSensorsThick,int _countSensorsThick,int _firstSensorsCross, int _countSensorsCross, int _countLogCh,double *_wdZone_ADC_Range_Value);
	// дополним существующую зону
	int AddPartZone(int _zoneNum, TZoneData* _zoneData, double* _adcData, int _needZoneAdd);
	// получить данные по конкретной зоне, по конкретному датчику
	// передаем номер зоны, номер сенсора в зоне
	// передаем и получаем выходной массив, сообщение
	// возвращаем код ошибки
	int GetSensorArrayData(int _zoneNum, int _sensorNum, double* _arrayData, int _beginRead, int _needRead, AnsiString &_msg);
	// записать в текстовый - имя файла указывается полностью с путем
	int SaveToFileTxt(AnsiString _fullFileName);
	// загрузить из текстового
	int LoadFromFileTxt(AnsiString _fullFileName);
	// записать в бинарник с метками
	int SaveTubeToFileBin(AnsiString _fullFileName);
	// загрузить из бинарника с метками
	int LoadTubeFromFileBin(AnsiString _fullFileName);
	// сбросить все данные и очистить признаки использования
	int ResetAllData(AnsiString &_msg);
    //обнуляем долько данные по зонам и датчикам
	int TTubeData::TstResetData(AnsiString &_msg);
	// TfmViewZone* fmViewZone;
	// удаление предыдущего файла
	int TTubeData::RefreshFile(AnsiString _fullFileName);

private:
	// static AnsiString applPath;
	int wdCountLOGchX502; // количество лог каналов в кадре
	double *wdZone_ADC_Range_Value;  //указатель на значения напряжений по каналам
	int Empty(void); // заглушка

	AnsiString lastMsg;
};
#endif
