#ifndef TLCardDataH
#define TLCardDataH
#include <vector>
// #include <IniFiles.hpp>
//------------------
//#ifndef TVIRTLCARD791
 #include "TLCard791.h"
//---------------------
#include "TLogCh791Params.h"
#include "TMeasuresData.h"
#define LCard791_INIT_SIZE 2000
#define LCard791_ADD_PERCENT 50
using namespace std;

// ---------------------------------------------------------------------------
class TLCardData {
private:
	// измерение
	// vector<vector<double> >LZone;
	// vector<vector<double> >CZone;
//	vector<vector<double> >GZone;
	//количество измерений
	int countMeasures;
	// bool IsCrossStarted;
	// bool IsLineStarted;
	bool IsSGStarted;


	TLCard791* dtLcard791;
	// Время сбора по одной частоте
	double BufTime;
	//Массив коэффициентов усиления. Не используется больше
//	vector<double>GVoltPercent;
	//vector<TLogCh791Params> veсLogChannelsParams;
	//TLogCh791Params* TLogCh791Params;

	//Номер текущей частоты
	int freqNum;
	//поиск точки перехода через 0 по указанному каналу
	int SearchOfZeroTransition(int _sensNum);
    //поиск периода точки перехода через 0 по указанному каналу
	int SearchOfZeroTransitionPeriod(int _sensNum);
public:
//#ifndef TVIRTLCARD791
	TLCardData(TLCard791* _lcard791,int _countFrequencies, int _chLogCount);
//#else
//	TLCardData(TVirtualLCard791* _lcard791,int _countFrequencies, int _chLogCount);
//#endif
	~TLCardData(void);

	AnsiString lastError;

	void LoadSettings(void);
		//вектор измерений
	vector<double> vecMeasure;
	vector<TMeasuresData> vecMeasuresData;
	void StartSGM(int _freqNum);
	void StopSGM(void);
	void ClearSGM(void);

//	inline vector<vector<double> > * GetPointSG() {
//		return (&GZone);
//	}

	inline vector<vector<double> >* GetPointSG(int _num) {
		return (&vecMeasuresData[_num].vecSensorsData);
	}

	double GetValueAvg5ByChName(AnsiString _name);
	inline double GetBufTime()
	{
    	return BufTime;
	}
	bool Read(int _freqNum);
	//перессчитываем измерения и разделяем на датчики
	int ReCalcMeasuresToChannels();
	//проверяем наличие муфты
	bool CheckMufta(int _sensNum);
	//получаем значения Баркгаузена на порогах
	vector<double> GetBarkValues(vector<int> &_Thresholds);
	//Расчет коррелляции
	double GetCorrelation(vector<double> &_XCoords,vector<double> &_YCoords);
	//Расчет квадратичного отклонения
	double GetSquareDeviation(vector<double> &_XCoords,vector<double> &_YCoords);
	//Возвращает текущий номер частоты
	inline int getFreqNum()
	{
		return freqNum;
	}
	//Устанавливает текущий номер частоты
	inline void setFreqNum(int _freqNum)
	{
		freqNum = _freqNum;
	}
	//передает указатель на плату
	inline TLCard791* GetLCard791()
	{
		return dtLcard791;
	}
};

// extern LCardData* lCardData;
// ---------------------------------------------------------------------------
#endif
