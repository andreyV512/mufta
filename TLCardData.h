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
	// ���������
	// vector<vector<double> >LZone;
	// vector<vector<double> >CZone;
//	vector<vector<double> >GZone;
	//���������� ���������
	int countMeasures;
	// bool IsCrossStarted;
	// bool IsLineStarted;
	bool IsSGStarted;


	TLCard791* dtLcard791;
	// ����� ����� �� ����� �������
	double BufTime;
	//������ ������������� ��������. �� ������������ ������
//	vector<double>GVoltPercent;
	//vector<TLogCh791Params> ve�LogChannelsParams;
	//TLogCh791Params* TLogCh791Params;

	//����� ������� �������
	int freqNum;
	//����� ����� �������� ����� 0 �� ���������� ������
	int SearchOfZeroTransition(int _sensNum);
    //����� ������� ����� �������� ����� 0 �� ���������� ������
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
		//������ ���������
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
	//�������������� ��������� � ��������� �� �������
	int ReCalcMeasuresToChannels();
	//��������� ������� �����
	bool CheckMufta(int _sensNum);
	//�������� �������� ����������� �� �������
	vector<double> GetBarkValues(vector<int> &_Thresholds);
	//������ �����������
	double GetCorrelation(vector<double> &_XCoords,vector<double> &_YCoords);
	//������ ������������� ����������
	double GetSquareDeviation(vector<double> &_XCoords,vector<double> &_YCoords);
	//���������� ������� ����� �������
	inline int getFreqNum()
	{
		return freqNum;
	}
	//������������� ������� ����� �������
	inline void setFreqNum(int _freqNum)
	{
		freqNum = _freqNum;
	}
	//�������� ��������� �� �����
	inline TLCard791* GetLCard791()
	{
		return dtLcard791;
	}
};

// extern LCardData* lCardData;
// ---------------------------------------------------------------------------
#endif
