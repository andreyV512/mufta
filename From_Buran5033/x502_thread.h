// ---------------------------------------------------------------------------

#ifndef x502_threadH
#define x502_threadH
#include "l502api.h"
#include "e502api.h"
#include <vcl.h>
#include "unTTubeData.h"

// класс для чтения данных с карты X502 с использованием потока
class X502_ProcessThread : public TThread {

protected:
	void __fastcall Execute();

public:
	// поток для чтения
	__fastcall X502_ProcessThread(bool CreateSuspended, int _countLogCh, int _adcFreqLch, uint32_t _rcvSizeBufSensor, TTubeData* _workData,TGlobalSettings *_thProgramSettings);
	__fastcall ~X502_ProcessThread(void);

	bool stopThread; // запрос на останов (устанавливается из основного потока либо ставится при ошибке

	// описатель модуля
	t_x502_hnd threadHndX502;
	// для 0-го кадра  на максимум логических каналов АЦП
	double* treadADCdouble0[X502_LTABLE_MAX_CH_CNT];
	//double* treadADCdouble0;
	// эксперимент
   //	double* treadADCdouble1[X502_LTABLE_MAX_CH_CNT];
   //	double* treadADCdouble2[X502_LTABLE_MAX_CH_CNT];
   //	double* treadADCdouble3[X502_LTABLE_MAX_CH_CNT];
   //	double* treadADCdouble4[X502_LTABLE_MAX_CH_CNT];
	int* threadSyncDIN0;
	// uint32_t
	double* adcData; // массив приема с карты
	uint32_t adcSize;
	uint32_t* dinData;
	uint32_t dinSize;
	int err; // код ошибки при выполнении потока сбора
	TTubeData* threadTubeData;//указатель на экземпляр класса обработчика данных
private:
	//void __fastcall updateData(TZoneData* _zoneData);
	void __fastcall updateData();
		//указатель на настройки
	TGlobalSettings* thProgramSettings;
	int threadCountLogCh;
	int threadAdcFreqLch;
	uint32_t firstLch; // номер первого логического канала в буфере АЦП
	int tmpInt;
	uint32_t threadRcvSizeBufZone; //размер буфера зоны;
	uint32_t threadRcvCountReady; // сколько готово в буфере модуля
	uint32_t threadRcvPartNeed; //сколько нужно дочитать до буфера зоны
	uint32_t threadRcvSizeRealStep; // сколько считали - забрали за шаг цикла
	//uint32_t threadRcvZoneSizeReal; // сколько считали по факту
	AnsiString msgErr; // сообщение ошибки при выполнении потока сбора
	bool isNewZone;	// признак готовности данных для построения новой зоны
	int currentZone; //номер текущей зоны
	unsigned int thstart_time; // начальное время
	unsigned int thend_time; // конечное время
	unsigned int thelaps_time; // искомое время
	inline int ThreadRun(AnsiString &_msgErr);
};

// ---------------------------------------------------------------------------
#endif
