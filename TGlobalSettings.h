// ---------------------------------------------------------------------------

#ifndef TGlobalSettingsH
#define TGlobalSettingsH
// ---------------------------------------------------------------------------
#include <System.hpp>
#include <Windows.hpp>
#include <SysUtils.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
// -----------
// #include <Controls.hpp>
// #include <Classes.hpp>
// #include <ExtCtrls.hpp>
// #include <StdCtrls.hpp>
// ---------

// ----------------
class TGlobalSettings {
public:
	TGlobalSettings();
	~TGlobalSettings(void);
	// ------------
	bool isFilter;
	int filterCurrentType;
	int filterCurrentSubType;
	int filterOrder;
	double filterCutoffFrequency;
	double filterCenterFrequency;
	double filterWidthFrequency;
	double filterRippleDb;
	double filterRolloff;
	double filterSampleRate;
	// ----------
	AnsiString applPath;
	AnsiString currentTypeSize;
	AnsiString nameCurrentTypeSize;
	int indexCurrentTypeSize; //������ �������� �����������
	int indexCurrentSGGost; //������ �������� ����� ��
	int protocolInterval;
	int protocolToFile;
	int protocolVisible;
	int zeroTransitionChannel; //����� ��� ������������� �� �������� ����� 0
	int checkMuftaLevel; //������� ������� ������� �����
	int checkMuftaChannel; //����� ������� ������� �����
	int calcSGMode; //����� ������� ����������� ��

	int workStatus; // ������ ��������� ������
	// <0 ������� �� ������
	// 0 ����� ��������
	// 1 ����� ���������
	// 2 ����� ���������������
	// 3 ���������� ������� �����
	// 4 ����� ������
	unsigned int ThresholdMaxCount; //������������ ����� �������
//	unsigned int CurrThresholdCount; //������� ����� �������
	unsigned int SinusFrequenciesMaxCount; //������������ ����� ������
//	unsigned int CurrSinusFrequenciesCount; //������� ����� ������
	// ------------------- �������
	int LoadPRGSettings(AnsiString _fullPathNameUDL);
	// -----------
	//������� ������������� ����������
	int discrFrecGSPF;
	//������� ����������
	int currTypeSize;
	//����� ��������� � ���������� �������� ������
	unsigned int WM_MsgReadyTstData;
	//��������� ������
	unsigned int WM_MsgThread;
	// ��������� �� ��������� ������ ����������� ������
	// TWorkData* gsWorkData;
	// int countLOGchX502; // ����� ��� ������������ ������� ���
	// ������
	enum LinesColor {
		Black = 0, Blue = 1, Green = 2, Cyan = 3, Red = 4, Magenta = 5, Brown = 6, LightGray = 7, DarkGray = 8, LightBlue = 9, LightGreen = 10, LightCyan = 11, LightRed = 12,
		LightMagenta = 13, Yellow = 14, White = 15
	};
	//������� ������ � ��������� ������
	UINT threadMsg;
	HWND mainHandle;

private:
	//
	};
#endif
