// ---------------------------------------------------------------------------

#ifndef TMeasuresDataH
#define TMeasuresDataH
// ---------------------------------------------------------------------------
#include <System.hpp>
#include <Windows.hpp>
#include <SysUtils.hpp>
#include <vector>
#include <iterator>
using namespace std;
// ---------------------------------------------------------------------------

#pragma package(smart_init)

class TMeasuresData {
public:
	TMeasuresData(int _chLogCount);
	~TMeasuresData(void);
//	// ����������
//	int realSensorSize;
//	int fullSensorSize;
	// ������ ������ �� ��������  [�����] - [������] �� �������
	vector<vector<double> >vecSensorsData;
	//������ ������������ ������� - ���� �����������
	vector<AnsiString> namesChannels;
	//�������
	int ClearData(void);
private:

};
#endif
