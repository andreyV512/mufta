#ifndef TLCard791H
#define TLCard791H
//#include <IniFiles.hpp>
#include <System.hpp>
#include <Windows.hpp>
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <vector>
#include "TGlobalSettings.h"
#include "TLogCh791Params.h"
#include "TProtocol.h"
#include "CardsFiles\L791\ioctl.h"
using namespace std;
//класс карты бывший LCard791.h
class TLCard791
{
public:
	//вектор каналов - настройки каналов
	vector<TLogCh791Params> vecLogChannels;
	// Конструктор + проверка платы
	//TLCard791(TIniFile* _ini);//, AnsiString _type);
	TLCard791(TGlobalSettings* _mainGlobalSettings,int &_codeErr);
	// Закрытие платы и деструктор
	~TLCard791(void);
	// Читает настройки и вбивает в плату
	void LoadAndSetSettings(vector<TLogCh791Params>& _logChannels);
	// Старт платы
	void Start();
	// Остановка платы
	void Stop(void);
	// Старт сбора
	void StartRead(void);
	// Остановка сбора
	void StopRead(void);
	// Читаем данные с платы
	double* Read(int* _readSize);
	//количество задействованых каналов
	int countLogCh;

	// ! ТаймАут для сбора (с каким периодом будем скидывать данные из ацп в большой буфер,мс)
	int RECV_TOUT;
	// ! @brief Режим синхронизации
	// ! @li 0 Внутренний сигнал
	// ! @li 1 От внешнего мастера по разъему синхронизации
	// ! @li 2 По фронту сигнала DI_SYN1
	// ! @li 3 По фронту сигнала DI_SYN2
	// ! @li 6 По спаду сигнала DI_SYN1
	// ! @li 7 По спаду сигнала DI_SYN2
	uint32_t syncMode;
	// ! Источник запуска синхронного ввода/вывода см. режимы @link syncMode
	uint32_t syncStartMode;
		//частота АЦП
	double measures_frequency_kHz;
	// ! Частота на канал (Гц.)
	double frequencyPerChannel_Hz;
	// ! Частота вывода
	double sendFrequency;
		// Последняя ошибка
	AnsiString lastError;
	// Выделяем значение замера
	int GetValueMeasure(int _ch, int _gain, bool _diff, bool* _ret = NULL);
	// Переводим значение в вольты
	double GetValueVolt(int _val, int _gain);
	// Переводим значение в проценты
	double GetValueProcent(int _val);
	// Получить значение замера по каналу
	int GetValueByChannel(TLogCh791Params& _channel);
	// Получить значение замера в вольтах по каналу
	double GetValueVolt(TLogCh791Params& _channel);
	TLogCh791Params*	FindChByName(AnsiString _name, int &err);
	//попробуем асинхронно получить один отчет
	double GetValueAsync(int _chNum);
	//указатель на настройки
	TGlobalSettings* globalSettings;
	//описание платы
	PLATA_DESCR_L791 plataDescr;

private:
	// Хэндл
	HANDLE t_stop_event;
	//
	HINSTANCE hComponent;
	// Нулевой указатель
	void* pI_void;
	// Номер текущего логического канала для правильного порядка чтения
	int currChLIndex;
	// Сообщает о критической ошибке
	void LFATAL(AnsiString _msg);
	// Номер слота платы
	int slotL791;//{укажите тут слот с котороым хотите работать. если плата одна то слот 0}
//	//указатель на настройки
//	TGlobalSettings* globalSettings;
    // Переводит логический номер канала и усиление в формат для платы
	ULONG CalcPortBits(int _port, int _gain, bool _diff);
	// Получаем замер
	ULONG GetValueRaw(int _ch, int _gain, bool _diff, bool* _ret = NULL);
	// Выделить номер канала из замера
	int GetChannel(ULONG _v);

	// Буфер данных
	double *data_buf;
	ULONG *data;
	ULONG *sync;
	// Адрес предыдущей синхронизации
	ULONG prev_sync;
	// Размер буфера
	ULONG bufSize;
	int Period;
	int Period_kadr;
	// Время последнего прочтения
	DWORD LastRead;
	// Вывод сообщения в протокол
	void ToProtocol(AnsiString _msg);
	// В работе ли плата
	bool started;

};
//extern LCard791 *lcard;
#endif
