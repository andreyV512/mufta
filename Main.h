// ---------------------------------------------------------------------------

#ifndef MainH
#define MainH
// ---------------------------------------------------------------------------
#include <System.hpp>
#include <Windows.hpp>
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Graphics.hpp>
#include <Forms.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ExtCtrls.hpp>
#include <vcl.h>
#include <dir.h>
#include "Chart.hpp"
#include "Series.hpp"
#include "TeEngine.hpp"
#include "TeeProcs.hpp"
#include <AppEvnts.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>
#include <Menus.hpp>
#include <ToolWin.hpp>
#include <Buttons.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
// ---------------
// #include <PlatformDefaultStyleActnCtrls.hpp>

#ifdef BERLIN_10
 #include <Vcl.Dialogs.hpp>
 #include <Vcl.AppEvnts.hpp>
 #include <System.ImageList.hpp>
 #include <Vcl.Menus.hpp>
 #include <Vcl.ImgList.hpp>
 #include <Vcl.ToolWin.hpp>
 #include <Vcl.ComCtrls.hpp>
 #include <VclTee.TeeGDIPlus.hpp>
 #include <VCLTee.Chart.hpp>
 #include <VCLTee.TeEngine.hpp>
 #include <VCLTee.TeeProcs.hpp>
 #include <VCLTee.TeCanvas.hpp>
 #include <VCLTee.Series.hpp>
 #include <System.IOUtils.hpp>
 #include <Vcl.ImgList.hpp>
#else
 #include <TeeGDIPlus.hpp>
 #include <Chart.hpp>
 #include <TeEngine.hpp>
 #include <TeeProcs.hpp>
 #include <TeCanvas.hpp>
 #include <Series.hpp>
 #include <IOUtils.hpp>
 #include <Dialogs.hpp>
 #include <AppEvnts.hpp>
 #include <Menus.hpp>
 #include <ImgList.hpp>
 #include <ToolWin.hpp>
 #include <ComCtrls.hpp>
 //#include <ImageList.hpp>
#endif
// -----------
// -----------------------
// #include <PlatformDefaultStyleActnCtrls.hpp>

#include "TGlobalSettings.h"
#include "TLogCh791Params.h"
#include "unSQLDbModule.h"
#include "ThreadWork.h"
#include "TLCardData.h"
#include "unEditEtalon.h"
#include "TypeSizeFrequencies.h"
#include "TGSPF052.h"
//-------------------
#ifndef TVIRTLCARD791
 #include "TLCard791.h"
#else
 #include "TVirtLCard791.h"
#endif

// ---------------------------------------------------------------------------
class TMainForm : public TForm {
__published: // IDE-managed Components

	TStatusBar *StatusBar;
	TMainMenu *MainMenu;
	TImageList *ImageList1;
	TToolBar *ToolBarTop;
	TMenuItem *N3;
	TMenuItem *menuSettings;
	TMenuItem *menuSGSett;
	TMenuItem *N6;
	TMenuItem *menuProtocol;
	TMenuItem *N9;
	TMenuItem *menuSave;
	TMenuItem *menuOpen;
	TMenuItem *N10;
	TMenuItem *menuAbout;
	TPanel *pThresholds;
	TPanel *BottomPanel;
	TLabel *lMinScrollbar;
	TLabel *lMaxScrollbar;
	TScrollBar *scbChartDragger;
	TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
	TButton *bStart;
	TButton *bCancel;
	TMenuItem *menuColorSett;
	TApplicationEvents *ApplicationEvents;
	TMenuItem *menuTypeSize;
	TMenuItem *mnuCheckGen;
	TMenuItem *mnuCheck1730;
	TMenuItem *menuEtalons;
	TComboBox *cbTypeSize;
	TGroupBox *GroupBoxNGr;
	TBitBtn *bbtCreateEtalon;
	TComboBox *cbxSG;
	TADOQuery *queryEtalon;
	TADOCommand *cmdEtalonVal;
	TPanel *Panel1;
	TChart *SignalChart;
	TPanel *PanelSG;
	TFastLineSeries *Series1;
	TFastLineSeries *Series2;
	TFastLineSeries *Series3;
	TPanel *Panel2;
	TChart *EtalonChart;
	TFastLineSeries *FastLineSeries1;
	TFastLineSeries *FastLineSeries2;
	TFastLineSeries *FastLineSeries3;
	TComboBox *cbSGGost;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall MenuProtocolClick(TObject *Sender);
	void __fastcall menuSaveClick(TObject *Sender);
	void __fastcall menuOpenClick(TObject *Sender); // открывает муфту
	void __fastcall SignalChartClick(TObject *Sender);
	void __fastcall SignalChartScroll(TObject *Sender);
	void __fastcall Edit1Exit(TObject *Sender);
	void __fastcall bStartClick(TObject *Sender);
	void __fastcall bCancelClick(TObject *Sender);
	void __fastcall menuColorSettClick(TObject *Sender);
	void __fastcall scbChartDraggerChange(TObject *Sender);
	void __fastcall ApplicationEventsMessage(tagMSG &Msg, bool &Handled);
	void __fastcall menuSettingsClick(TObject *Sender);
	void __fastcall menuTypeSizeClick(TObject *Sender);
	void __fastcall menuAboutClick(TObject *Sender);
	void __fastcall menuSGSettClick(TObject *Sender);
	void __fastcall PanelSQClick(TObject *Sender);
	void __fastcall PanelONClick(TObject *Sender);
	void __fastcall mnuCheckGenClick(TObject *Sender);
	void __fastcall mnuCheck1730Click(TObject *Sender);
	void __fastcall menuEtalonsClick(TObject *Sender);
	void __fastcall cbTypeSizeSelect(TObject *Sender);
	void __fastcall bbtCreateEtalonClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall cbSGGostSelect(TObject *Sender);

private: // User declarations

	// общие настройки программы
	TGlobalSettings mainGlobalSettings;
	// LCard791 - указатель на карту
//#ifndef TVIRTLCARD791
	TLCard791 *lСard791;
//#else
//	TVirtualLCard791 *lСard791;

	// указатель на массив данных
	TLCardData* lCardData;

	// vector<TEdit*> eThresholds; // Поля для отображения порогов
	void ClearCharts(void); // очищает график
	void DrawOnChart(); // рисует на чарте
	void DrawThresholds(); // рисует пороги на чарте
	void UpdateStatus(AnsiString _text1, AnsiString _text2);

	// vector<TLineSeries*> lsThresholds; // массив серий с порогами

	void SaveToFile(UnicodeString FileName); // сохраняет трубу

	long curPos;

	UINT set_msg;
    //группа прочности
	CSG csg;

	void __fastcall WmDropFiles(TWMDropFiles& Message);
	void __fastcall ReadFile(AnsiString FileName);
	BEGIN_MESSAGE_MAP MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WmDropFiles)
		END_MESSAGE_MAP(TForm);

	// TIniFile* ini;
	// vector<IU> SGSignal;
	// vector<IU> lSGSignal;
	// SGCoords* sgc;
	// vector<double>local_th;

	bool inWork; // установка в работе
	ThreadWork *threadWork;
	int thresholdCount;

	void ChangeColor();

	TColor ActiveIn;
	TColor NotActiveIn;
	// код ошибки
	int codeERR;
	//Частоты текущего типоразмера
	TSFrequencies *TSFreqs;
	//плата генератора сигнала
	TGSPF052* gen;

	//запуск рабочего цикла
	void Start();
	//перезапуск рабочего цикла
	void ReStart();
	//остановка рабочего цикла
	void Stop();
	//Перерисовываем графики
	void Redraw();

	//блокируем кнопки
	void SetAbleButtons(bool _enable);
public: // User declarations

	__fastcall TMainForm(TComponent* Owner);
	void LoadFromFile(UnicodeString FileName);

};

// ---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
// ---------------------------------------------------------------------------
#endif
