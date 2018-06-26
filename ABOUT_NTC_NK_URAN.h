// ----------------------------------------------------------------------------
#ifndef ABOUT_NTC_NK_URANH
#define ABOUT_NTC_NK_URANH
// ----------------------------------------------------------------------------
#include <System.hpp>
#include <Windows.hpp>
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Graphics.hpp>
#include <Forms.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>

// ----------------------------------------------------------------------------
class TfmAboutBox : public TForm {
__published:
	TPanel *Panel1;
	TImage *ProgramIcon;
	TLabel *lProductName;
	TLabel *lVersion;
	TLabel *lCopyright;
	TButton *OKButton;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *lCreationDate;
	TLabel *Label3;

	void __fastcall FormShow(TObject *Sender);

private:
public:
	virtual __fastcall TfmAboutBox(TComponent* AOwner);
};

// ----------------------------------------------------------------------------
extern PACKAGE TfmAboutBox *fmAboutBox;
// ----------------------------------------------------------------------------
#endif
