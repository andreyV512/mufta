//---------------------------------------------------------------------------

#ifndef TypeSizeFrequenciesH
#define TypeSizeFrequenciesH
#include <vector>
using namespace std;
//---------------------------------------------------------------------------
class TSFrequencies //����� �������� ��������� ���� ��� �����������
{
public:
	TSFrequencies(int _TS_id);
	~TSFrequencies();
	//�������
	vector <int> Frequency;
	//���������
	vector <double> Amplitude;
};
#endif
