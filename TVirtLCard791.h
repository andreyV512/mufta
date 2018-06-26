// ---------------------------------------------------------------------------

#ifndef TVirtLCard791H
#define TVirtLCard791H
// ---------------------------------------------------------------------------
#include <System.hpp>
#include <Windows.hpp>
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <vector>
#include "TGlobalSettings.h"
#include "TLogCh791Params.h"
#include "TProtocol.h"
//#include <DB.hpp>
//#include <ADODB.hpp>
using namespace std;


// ����� ����� ������ LCard791.h
class TVirtualLCard791 {
public:
	//������ ������� - ��������� �������
	vector<TLogCh791Params> vecLogChannels;
	// ����������� + �������� �����
	//TLCard791(TIniFile* _ini);//, AnsiString _type);
	TVirtualLCard791(TGlobalSettings* _mainGlobalSettings,int &_codeErr);
	// �������� ����� � ����������
	~TVirtualLCard791(void);
	// ������ ��������� ��  � ������� � �����
	void LoadAndSetSettings(vector<TLogCh791Params>& _logChannels);
	// ����� �����
	void Start();
	// ��������� �����
	void Stop(void);
	// ����� �����
	void StartRead(void);
	// ��������� �����
	void StopRead(void);
	// ������ ������ � �����
	double* Read(int* _readSize);
	//���������� �������������� �������
	int countLogCh;

	// ! ������� ��� ����� (� ����� �������� ����� ��������� ������ �� ��� � ������� �����,��)
	int RECV_TOUT;
	// ! @brief ����� �������������
	// ! @li 0 ���������� ������
	// ! @li 1 �� �������� ������� �� ������� �������������
	// ! @li 2 �� ������ ������� DI_SYN1
	// ! @li 3 �� ������ ������� DI_SYN2
	// ! @li 6 �� ����� ������� DI_SYN1
	// ! @li 7 �� ����� ������� DI_SYN2
	uint32_t syncMode;
	// ! �������� ������� ����������� �����/������ ��. ������ @link syncMode
	uint32_t syncStartMode;
		//������� ���
	double measures_frequency_kHz;
	// ! ������� �� ����� (��.)
	double frequencyPerChannel_Hz;
	// ! ������� ������
	double sendFrequency;
		// ��������� ������
	AnsiString lastError;
	// �������� �������� ������
	int GetValueMeasure(int _ch, int _gain, bool _diff, bool* _ret = NULL);
	// ��������� �������� � ������
	double GetValueVolt(int _val, int _gain);
	// ��������� �������� � ��������
	double GetValueProcent(int _val);
	// �������� �������� ������ �� ������
	int GetValueByChannel(TLogCh791Params& _channel);
	// �������� �������� ������ � ������� �� ������
	double GetValueVolt(TLogCh791Params& _channel);
	TLogCh791Params*	FindChByName(AnsiString _name, int &err);
	//��������� ���������� �������� ���� �����
	double GetValueAsync(int _chNum);
private:
	// �����
	HANDLE t_stop_event;
	//
	HINSTANCE hComponent;
	// ������� ���������
	void* pI_void;
	// INI-���� � �����������
	//TIniFile* ini;
//	AnsiString type;
	// ����� �������� ����������� ������ ��� ����������� ������� ������
	int CL_index;
	// �������� � ����������� ������
	void LFATAL(AnsiString _msg);
	// ����� ����� �����
	int slot;
	//��������� �� ���������
	TGlobalSettings* globalSettings;
    // ��������� ���������� ����� ������ � �������� � ������ ��� �����
	ULONG CalcPortBits(int _port, int _gain, bool _diff);
	// �������� �����
	ULONG GetValueRaw(int _ch, int _gain, bool _diff, bool* _ret = NULL);
	// �������� ����� ������ �� ������
	int GetChannel(ULONG _v);

	// ����� ������
	double *data_buf;
	ULONG *data;
	ULONG *sync;
	// ����� ���������� �������������
	ULONG prev_sync;
	// ������ ������
	ULONG bufSize;
	int Period;
	int Period_kadr;
	// ����� ���������� ���������
	DWORD LastRead;
	// ����� ��������� � ��������
	void ToProtocol(AnsiString _msg);
	// � ������ �� �����
	bool started;
	//������ ������ ������
	int szDataBuf;
	//�������� ������
	int sizeBuf;
	//�������� � ����������� ������
	int offsetBuf;
	//��� �����
	vector<double*> virtBuf;
};
#endif
