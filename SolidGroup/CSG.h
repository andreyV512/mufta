//---------------------------------------------------------------------------
#ifndef CSGH
#define CSGH
#include<system.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
// ������ ��������� � ���������� ����������� � �����
class CSG
{
public:
	CSG(void);
	AnsiString group;
	int group_id;
	double probability;
	TColor color;
};
#endif
