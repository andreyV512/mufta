//---------------------------------------------------------------------------

#ifndef uTFRSensorH
#define uTFRSensorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "cspin.h"
//#include "RChannel_parameters.h"
#include "TLogCh791Params.h"
#include "unSQLDbModule.h"
//---------------------------------------------------------------------------
class TFRSensor : public TFrame
{
__published:	// IDE-managed Components
	TLabel *LName;
	TComboBox *CBRange;
	TCSpinEdit *SEChannel;
	TComboBox *CBMode;
private:	// User declarations
	//TIniFile* ini;
	int GetRange(void);
	int GetChannel(void);
	int GetMode(void);
	TLogCh791Params* ch_par;
public:		// User declarations
	__fastcall TFRSensor(TComponent* Owner,TLogCh791Params* _ch_par,AnsiString _title);
	void SetEnabledR(bool _v);
	void SaveSettings(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFRSensor *FRSensor;
//---------------------------------------------------------------------------
#endif
