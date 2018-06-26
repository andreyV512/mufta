// ---------------------------------------------------------------------------

#ifndef x502_threadH
#define x502_threadH
#include "l502api.h"
#include "e502api.h"
#include <vcl.h>
#include "unTTubeData.h"

// ����� ��� ������ ������ � ����� X502 � �������������� ������
class X502_ProcessThread : public TThread {

protected:
	void __fastcall Execute();

public:
	// ����� ��� ������
	__fastcall X502_ProcessThread(bool CreateSuspended, int _countLogCh, int _adcFreqLch, uint32_t _rcvSizeBufSensor, TTubeData* _workData,TGlobalSettings *_thProgramSettings);
	__fastcall ~X502_ProcessThread(void);

	bool stopThread; // ������ �� ������� (��������������� �� ��������� ������ ���� �������� ��� ������

	// ��������� ������
	t_x502_hnd threadHndX502;
	// ��� 0-�� �����  �� �������� ���������� ������� ���
	double* treadADCdouble0[X502_LTABLE_MAX_CH_CNT];
	//double* treadADCdouble0;
	// �����������
   //	double* treadADCdouble1[X502_LTABLE_MAX_CH_CNT];
   //	double* treadADCdouble2[X502_LTABLE_MAX_CH_CNT];
   //	double* treadADCdouble3[X502_LTABLE_MAX_CH_CNT];
   //	double* treadADCdouble4[X502_LTABLE_MAX_CH_CNT];
	int* threadSyncDIN0;
	// uint32_t
	double* adcData; // ������ ������ � �����
	uint32_t adcSize;
	uint32_t* dinData;
	uint32_t dinSize;
	int err; // ��� ������ ��� ���������� ������ �����
	TTubeData* threadTubeData;//��������� �� ��������� ������ ����������� ������
private:
	//void __fastcall updateData(TZoneData* _zoneData);
	void __fastcall updateData();
		//��������� �� ���������
	TGlobalSettings* thProgramSettings;
	int threadCountLogCh;
	int threadAdcFreqLch;
	uint32_t firstLch; // ����� ������� ����������� ������ � ������ ���
	int tmpInt;
	uint32_t threadRcvSizeBufZone; //������ ������ ����;
	uint32_t threadRcvCountReady; // ������� ������ � ������ ������
	uint32_t threadRcvPartNeed; //������� ����� �������� �� ������ ����
	uint32_t threadRcvSizeRealStep; // ������� ������� - ������� �� ��� �����
	//uint32_t threadRcvZoneSizeReal; // ������� ������� �� �����
	AnsiString msgErr; // ��������� ������ ��� ���������� ������ �����
	bool isNewZone;	// ������� ���������� ������ ��� ���������� ����� ����
	int currentZone; //����� ������� ����
	unsigned int thstart_time; // ��������� �����
	unsigned int thend_time; // �������� �����
	unsigned int thelaps_time; // ������� �����
	inline int ThreadRun(AnsiString &_msgErr);
};

// ---------------------------------------------------------------------------
#endif
