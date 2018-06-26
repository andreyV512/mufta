// ---------------------------------------------------------------------------

#ifndef unTExtFunctionH
#define unTExtFunctionH
// ---------------------------------------------------------------------------
#include <System.hpp>
#include <Windows.hpp>
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#ifdef BERLIN_10
 #include <VCLTee.Chart.hpp>
 #include <VCLTee.Series.hpp>
 #include <System.IOUtils.hpp>
#else
 #include <Chart.hpp>
 #include <Series.hpp>
 #include <IOUtils.hpp>
#endif
#define  sizeArr 150;
// #include "workWithX502.h"
// ---------------------------------------------------------------------------
// ��������� ��������������� �������
class TExtFunction {
	TExtFunction();
	~TExtFunction();

public:
	// ������� ��������  (*a)[2]
	static int FillArrayModule(AnsiString _tubeNum, int _tubeModule, int _maxZones, int _countSensors, int _arrDataModule[8][100]);
	// ����� �����
	static int ShowArrayModule(int _countSensors, int _maxZones, int _arrDataModule[8][100], TChart* _chart);
	// ����������� ����� ������������, ��� ������ �������, �� �������
	static void PrepareBarChart(int _countSensors, int _maxZones, TChart* _chart);
	static void PrepareFastLineChart(int _countSensors,  int _lengthChart,TChart* _chart);
	// ����� ���� (�����) � ���� �� ����� � ������ ������� � ���� ������
	// _moduleType 1-���������� 2 - ���������� 3 - ���������� 4 - ������ ��������� 5 - ��������� ���������
	static int SaveChartToDB(AnsiString _serialTubeNum,AnsiString _tubeNum, int _codeTubeTypeSize, int _moduleType, int _maxZones, int _codeCustomer, int _codeOperatorU, int _codeStatusTube,
		TChart* _chart);

	// ����� � ������ ���������
	static int UpdateStatusBar(TStatusBar *_StatusBar, AnsiString _strTitle, AnsiString _strText, TColor _colorFont);
	// ����� � �����-�� �����
	static int UpdateLabelStatus(TLabel *_label, AnsiString _strTitle, AnsiString _strText, TColor _colorFont);
	// ����� ��������� � ��������� �����
	static int ShowBigModalMessage(AnsiString _strMsg, TColor _color);
		//���������� �����
	static void FATAL(AnsiString _msg);
	//��� ��������� �������� �����������
	static void PrepareChartToTst(TChart *chart, int _countChLog, int _lengthChart,int _maxYChart);
	// ������ ��� ������� � ��������� ��� - ������ �� ��������� �� 100 ���, ������� �� ���������� �������� 8 (�������� �� ��������)
	static int arrDataThick[8][100];
	static int arrDataCross[8][100];
	static int arrDataSum[8][100];
private:
	// static int a;
	// ������ ��� ������ ���������
	AnsiString msgLast;
};

// ������� ��� �������� ��������� �� ����� (������ - ����)
int TExtFunction::arrDataThick[8][100];
int TExtFunction::arrDataCross[8][100];
int TExtFunction::arrDataSum[8][100];
// int TExtFunction::a;
// int TExtFunction::a=0;
#endif
