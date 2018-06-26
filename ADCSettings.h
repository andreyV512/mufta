// ---------------------------------------------------------------------------

#ifndef ADCSettingsH
#define ADCSettingsH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "cspin.h"
//#include "IniFiles.hpp"
#include <AppEvnts.hpp>
#include "Filters.h"
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "uTFilterSettings.h"
#include "uTFRSensor.h"
#include <vector>
#include <Registry.hpp>
#include "uCRListT.h"
//------------------
#ifndef TVIRTLCARD791
 #include "TLCard791.h"
#else
 #include "TVirtLCard791.h"
#endif
//---------------------
//#include "RLCard_parameters.h"


// ---------------------------------------------------------------------------
class TADCSettForm : public TForm
{
__published: // IDE-managed Components

	TApplicationEvents *ApplicationEvents;
	TPanel *pBottom;
	TButton *bSave;
	TButton *bExit;
	TPageControl *PageControl;
	TTabSheet *ppPage;
	TLabel *Label2;
	TEdit *readRate;
	TGroupBox *gbSG;
	TEdit *sendRate;
	TLabel *Label1;
	TFilterSettings *FSSG;
	TTrackBar *tbCheckMuftaLevel;
	TLabel *lCheckMuftaLevelCaption;
	TLabel *lCheckMuftaLevel;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ApplicationEventsMessage(tagMSG &Msg, bool &Handled);
	void __fastcall bSaveClick(TObject *Sender);
	void __fastcall bExitClick(TObject *Sender);
	void __fastcall eThresholdResistKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall ContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall tbCheckMuftaLevelChange(TObject *Sender);

private:
	// User declarations
	//фреймы параметров
	CRListT<TFRSensor> listSens;
#ifndef TVIRTLCARD791
	TLCard791 *sLСard791;
#else
	TVirtualLCard791 *sLСard791;
#endif
	//int LinearItemIndex;
	//int CrossItemIndex;
	//TIniFile *ini;

	void block();

	bool flag; //активна ли блокировка

	void FillAllControls();
	//RLCard_parameters* Par;

public: // User declarations
#ifndef TVIRTLCARD791
	__fastcall TADCSettForm(TComponent* Owner,TLCard791 *_lСard791);
#else
	__fastcall TADCSettForm(TComponent* Owner,TVirtualLCard791 *_lСard791);
#endif
	bool need_redraw;

};

// ---------------------------------------------------------------------------
extern PACKAGE TADCSettForm *ADCSettForm;
// ---------------------------------------------------------------------------
#endif
