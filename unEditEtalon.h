//---------------------------------------------------------------------------

#ifndef unEditEtalonH
#define unEditEtalonH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <DB.hpp>
#include <ADODB.hpp>
#include <DBCtrls.hpp>
#include <Mask.hpp>
//------------------
#include "unSQLDbModule.h"
#include "unTUtils.h"
#include "TGlobalSettings.h"
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include "unFmViewMeasure.h"
#include <AppEvnts.hpp>
//---------------------------------------------------------------------------
class TfmEditEtalon : public TForm
{
__published:	// IDE-managed Components
	TADOQuery *queryEtalonVal;
	TDataSource *dsEtalonVal;
	TDataSource *dsEtalon;
	TADOQuery *queryEtalon;
	TGroupBox *GroupBox1;
	TDBNavigator *navEtalon;
	TGroupBox *GroupBox2;
	TDBNavigator *navEtalonVal;
	TDBGrid *gridEtalon;
	TDBGrid *gridEtalonVal;
	TPanel *PanelTop;
	TComboBox *cbTypeSize;
	TLabel *Label1;
	TBitBtn *bbtShowEt;
	TBitBtn *bbtRecalc;
	TADOCommand *cmdEtalonVal;
	TGroupBox *gbSGThreshold;
	TDBNavigator *navTS;
	TDBGrid *gridTSz;
	TADOQuery *queryTSz;
	TDataSource *sourseTSz;
	TBitBtn *bbtRecalcAll;
	TLabel *Label2;
	TComboBox *cbSGGost;
	TApplicationEvents *ApplicationEvents;
//	void __fastcall gridEtalonEditButtonClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall cbTypeSizeSelect(TObject *Sender);
	void __fastcall bbtShowEtClick(TObject *Sender);
	void __fastcall bbtRecalcClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall bbtRecalcAllClick(TObject *Sender);
	void __fastcall queryTSzBeforePost(TDataSet *DataSet);
	void __fastcall cbSGGostSelect(TObject *Sender);
	void __fastcall ApplicationEventsMessage(tagMSG &Msg, bool &Handled);
private:	// User declarations
	TGlobalSettings *pGlobalSettings;
	TLCardData* plCardData;
	bool isEnable;
	void BlockControls(bool _isEnable);
	void __fastcall TfmEditEtalon::UserWndProc(Messages::TMessage &_msg);
	int FillGrids(int _indTsz, int _indGost);

public:		// User declarations
	__fastcall TfmEditEtalon(TComponent* Owner,TGlobalSettings *_pGlobalSettings,TLCardData* lCardData);
	int InsertEtalon( int _frequency,int _fenable, AnsiString _address_file, int sg_id);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmEditEtalon *fmEditEtalon;
//---------------------------------------------------------------------------
#endif
