//---------------------------------------------------------------------------
#ifndef TSGH
#define TSGH
#include<system.hpp>
//#include <IniFiles.hpp>
#include <vector>
#include <Graphics.hpp>
#include "CSG.h"
#include "IU.h"
//
#include "TGlobalSettings.h"
#include "TLCardData.h"
using namespace std;
//---------------------------------------------------------------------------
class TSG
{
public:
	//����� ��� ������ ��������� -  ������ � ����� ����������
	TSG(TGlobalSettings* _mainGlobalSettings,TLCardData* _sgLCardData);
	~TSG(void);
	int Exec(int _freqNum);
	CSG csg;
	// ������� �� �� ���� ��������
	CSG GetSG();
    // ������� �� �� ������ �������
	CSG Get1FrecSG();
	inline void ResetState(void){
		state=0;
	}
private:
	int state;
	int freqNum;   //����� ������� ������� ����������

	void pr(AnsiString _msg);
	void pr0(AnsiString _msg);

	AnsiString SGroupFix;
	TColor conv_color(int _col);
	DWORD startTime;
	//--------
	TGlobalSettings* sgGlobalSettings;
	TLCardData* sgLCardData;
};
//---------------------------------------------------------------------------
#endif
