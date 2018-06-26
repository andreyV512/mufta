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
#define strStatus "Статус: "

using namespace std;

// class workX502 : public TWorkX502(TObject owner, t_x502_hnd pHndX502);
class TWorkX502 {
public:
	// User declarations
	TWorkX502(void);
	~TWorkX502(void);

	// --------------
	t_x502_hnd hndX502; // описатель модуля
	bool threadRunning; // признак, запущен ли поток сбора данных
	X502_ProcessThread* threadX502; // поток для синхронного ввода
	double kadr_data0[X502_LTABLE_MAX_CH_CNT]; // массив для кадра на максимум логических каналов АЦП
	// vector<double> dataThick;
	// эксперимент
	// double kadr_data1[X502_LTABLE_MAX_CH_CNT]; // массив для кадра на максимум логических каналов АЦП
	// double kadr_data2[X502_LTABLE_MAX_CH_CNT]; // массив для кадра на максимум логических каналов АЦП
	// double kadr_data3[X502_LTABLE_MAX_CH_CNT]; // массив для кадра на максимум логических каналов АЦП
	// double kadr_data4[X502_LTABLE_MAX_CH_CNT]; // массив для кадра на максимум логических каналов АЦП
	int syncDIN0; // данные цифрового входа 1 байт синхронного потока
	// double strDinRes; // данные цифрового выхода строкой
	DynamicArray<t_x502_devrec>devrecList; // массив устройств
	bool haveX502; // наличие карты Х502
	// параметры карты
	bool isDacPresent; // Признак наличия двухканального канального ЦАП
	bool isGalPresent; // Признак наличия гальваноразвязки
	bool isDspPresent; // Признак наличия сигнального процессора BlackFin
	bool isEthPresent; // Признак, что устройство поддерживает интерфейс PCI/PCI-Express
	AnsiString strPldaVer; // версия ПЛИС аналог
	AnsiString strFpgaVer; // версия ПЛИС
	AnsiString strMcu_firmware_ver; // версия прошивки
	// параметры частот
	int x502AdcFreq; // частота синхронного сбора АЦП без учета количества каналов
	int x502DinFreq; // частота синхронного сбора цифровых
	int x502AdcFreqLch; // частота синхронного сбора АЦП  на канал
	// --------
	int x502ChMode; // признак режима работы каналов дифференциальный/с общей землей
	int x502CountDev; // количество подключенных карт
	int x502EnableSyncADC; // признак режима сбора синхронный/асинхронный c АЦП
	// X502_SYNC_INTERNAL = 0, < Внутренний сигнал
	// X502_SYNC_DI_SYN2_RISE = 6, < По фронту сигнала DI_SYN2
	int x502SyncMode; // признак режима синхронизации - внутренний/внешний (у нас всегда внутренний) был, бля
	// X502_SYNC_DI_SYN2_RISE = 6, < По фронту сигнала DI_SYN2
	int x502SyncStartMode; // условие запуска синхронного ввода/вывода данных

	bool x502EnableSyncDin; // разрешаем или запрещаем поток синхронного ввода с цифровых входов
	int x502FLAGS_VOLT; // работаем в вольтах или в отчетах АЦП
	// int x502ADC_RANGE_V; // Диапазон измерения для канала АЦП в вольтах
	// int x502ADC_RANGE_VX[32]; // Диапазоны измерения для каналов АЦП в вольтах
	// int x502ADC_RANGE_INT; // Диапазон измерения для канала АЦП в INT64
	int x502CountFIZch; // количество физ каналов максимум 32
	int x502CountLOGch; // количество лог каналов  максимум 256, но прописаны поля в базе только для 32-х!!!
	bool isX502Opened; // карта уже была подсоединена
	uint32_t realAdcFreq; // реальная частота сбора
	//значения входов
	int  valuesIN[32];
	// значения на выходе
	int valuesOUT[32];
	// указатель на настройки
	TGlobalSettings *wwProgramSettings;
	//TObject* owner;
	// указатель на экземпляр класса обработчика данных
	TTubeData* w502TubeData;
	AnsiString applPath;
	// размер вычитываемого буфера АЦП на канал в измерениях (это не байты!)
	unsigned int x502ADCBufSensorSize;
	// размер вычитываемого буфера цифровых входов на канал в измерениях (это не байты!)
	unsigned int x502DinBufSize;

	// функции
	__fastcall void OnThreadTerminate(TObject *Sender);
	// void OnThreadTerminate();
	// для IP соединения
	void OpenByIP(AnsiString _strIP);
	// ----------------------
	void UpdateControls(); // обновление интерфейса
	int GetDevInfo(void); // инфо по карте
	// останов потока
	void __fastcall StopThread();
	void FreeDevList();
	void RefreshDeviceList(int &_CountDev);
	void ShowDevInfo();
	// Установка параметров сбора данных
	int SetupParams(int _СountCh, int _SyncMode, int _SyncStartMode);
	// устанавливаем требуемую частоту сбора
	int SetAdcFreq(int &_x502AdcFreq, int &_x502AdcFreqLch, int &_x502DinFreq);
	// запуск потока чтения
	int StartRead(bool _enableSYNCDin, AnsiString &_msg);
	// устанавливаем кол-во логических каналов, чтобы правильно рассчитать частоту на канал
	int AdcFreqChange(int _ChCnt, AnsiString &_msg);
	// разрешаем или запрещаем поток синхронного ввода
	int SetSyncDinStream(bool _EnableSyncDin);
	// разрешения/запрета синхронного ввода цифровых
	int SetSyncDin(AnsiString &_msg);
	// функция открытия устройства с выбранным серийным номером или закрытия его,
	// если оно уже было открыто
	int X502OpenClose(int _numDev, AnsiString &_msg);
	// асинхронного вывода на цифровые линии
	int SetAsyncDigOut(int _AsyncDigOut, AnsiString &_msg);
	// асинхронного ввода с цифровых линий
	int GetAsyncDigIn(AnsiString &_msg);
	// получим масив значений входов
	// вылавливаем цифровые входы
	int GetAsyncValuesDigIn(int _valuesIN[16], AnsiString &_msg);
	// выставим выходы согласно массиву
	int SetAsyncValuesDigOUT(int _valuesOUT[16], AnsiString &_msg);
	// вывод на ЦАП максимум 5 В
	int SetAsyncDac1(double _valOut, AnsiString &_msg);
	int SetAsyncDac2(double _valOut, AnsiString &_msg);
	int SetAsyncDac(int _numADC, double _valOut, AnsiString &_msg);
	// забираем кадр в вольтах
	int GetAsyncAdcFrame(AnsiString &_msg);
	void InfDevListChange();
	void ChCntChange(int _ChCnt);
	// запись и загрузка параметров карты SQL
	void LoadX502Settings(AnsiString _pathUDL, AnsiString &_msg);
	void SaveX502Settings(AnsiString _pathUDL, AnsiString &_msg);
	// установка цифровых входов в 1
	// на E502 не привязываются! только на L502
	void SetDITo1(int _typeCard, bool _lowBytes, bool _hiBytes);
	// -------------------------
	void CloseDevice(AnsiString &_msg);
	int GetDataAsync(AnsiString &_msg);
	// получить текущее значение опорной частоты
	int X502GetRefFreqValue(double _freq);
	int X502CreatePrepareWorkData(AnsiString &_msg);
	// получить значение опорного на канале по настройкам
	int GetVoltMaxLCh(int _numLCh);
	// запуск двигателя каретки
	int StartDrive(double _speed, int _codeDOutDrive, AnsiString &_msg);
	// стоп двигателя каретки
	int StopDrive(int _codeDOut, AnsiString &_msg);
	// аварийный сброс или полный останов
	int AlarmSituiation(AnsiString &_msg);
	// старт потоков
	int SYNK_X502StreamsStart(t_x502_hnd _hndX502);
	// стоп потоков
	int SYNK_X502StreamsStop(t_x502_hnd _hndX502);
	SetADCRange(AnsiString _strRange, int _ind);
	int GetADCRange(int _ind);
	double GetADCRangeValue(int _ind);
    //чистим массивы битов
	int ClearValuesDig(int _values[16]);
	int CheckCurrentDrive(int _numZone, int _sensDrvNum, int _firstValue, int _sizeValues, double _valueNominal,double _valueMax) ;
    	// массив значений опорных для расчета в программе
	double adc_Range_Value[X502_LTABLE_MAX_CH_CNT];
private:
	// парсим строку с IP
	int ParseIpAddr(AnsiString _addr);

	// строка для вывода сообщений
	AnsiString msgLast;
	// массив значений опорных для Х502
	int adc_Range[X502_LTABLE_MAX_CH_CNT];

};
// extern TWorkX502* workX502;
#endif
