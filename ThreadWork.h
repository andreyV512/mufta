#ifndef ThreadWorkH
#define ThreadWorkH
// #include <IniFiles.hpp>
#include <Graphics.hpp>
#include "TSG.h"
#include <SyncObjs.hpp>
#include "TGlobalSettings.h"
#include "TGSPF052.h"
#include "TLCardData.h"

class ThreadWork : public TThread {
protected:
	void __fastcall Execute();
public:
	// ��������
	// __fastcall ThreadWork(TIniFile* _ini, AnsiString _SGFix, TLCardData* _lCardData);
	__fastcall ThreadWork(bool _createSuspended, TLCardData* _lCardData,TGlobalSettings* _globalSettings, TGSPF052* _gen);
	__fastcall ~ThreadWork(void);
	static const WPARAM REDRAW = 0;
	static const WPARAM COMPUTE = 1;
	static const WPARAM UPDATE_STATUS = 3;
	static const WPARAM COMPLETE = 5;
	static const WPARAM NEXT_MUFT = 6;

	AnsiString GetStext1(void);
	AnsiString GetStext2(void);
	// ���������� ��������� ��������� ������� //todo � ����� ��?
	void SetCalc(); // (bool _IsBrak);

	// ���������� ������� ������ ���������
	CSG GetSG(void);
	// ---------------
private:
	int pauseWorkSpeed;
	int pauseStop;
    bool createSuspended;
	bool stopThread; // ������ �� ������� (��������������� �� ��������� ������ ���� �������� ��� ������
	// ! ����� ��� ������� � ���������� � ������ 1
	AnsiString stext1;
	// ! ����� ��� ������� � ���������� � ������ 2
	AnsiString stext2;
	int delay;
	bool Collect;
	TCriticalSection* cs;
	// ����� ������� ������ ���������
	TSG* solidGroup;
	//���������
	TGlobalSettings* thGlobalSettings;
	//������ ���������
	TLCardData* lCardData;
	// ����� ���������� �������
	TGSPF052* GSPF;
	//��������� ������
//	TLogCh791Params* lCard_parameters;
	void SetStext1(AnsiString _s);
	void SetStext2(AnsiString _s);
	TEvent* calc_event;
	// ������ � ��������
	void pr(AnsiString _msg);
	// ������ ������ �� ����� � �� ��� ����������
	void SaveMuftToDB();
	inline int ThreadRun(AnsiString &_msgErr);
	// ��������� ��������� � ������
	CSG csg;
	// ���������� ��������� ������ ���������
	void SetSG(CSG _csg);
	// ! ����� ��������� ���� ���� ����������
	bool OnlineCycle(void);
	// ! ���������� �������� ��������� ������� ����� �������
	UnicodeString PrepareForWork();
	// �������� ������� �����. ���������� true ���� �� ��������� ����� ������ ��������� �� ���������
	bool CheckMufta(bool _waitStatus, int _waitTime);
	// ! ��������� ����� (���������� �������� � �.�.)
	void Finally();
	void ErrFinally(AnsiString _msg, bool* _Collect, bool* _result);
	void Post(WPARAM _w, LPARAM _l = 0);

};
// ---------------------------------------------------------------------------

#endif
