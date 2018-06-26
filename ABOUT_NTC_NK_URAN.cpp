// ---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ABOUT_NTC_NK_URAN.h"
#include "IOUtils.hpp"
// ---------------------------------------------------------------------
#pragma resource "*.dfm"
TfmAboutBox *fmAboutBox;

// ---------------------------------------------------------------------
__fastcall TfmAboutBox::TfmAboutBox(TComponent* AOwner) : TForm(AOwner) {
}
// ---------------------------------------------------------------------

void __fastcall TfmAboutBox::FormShow(TObject *Sender) {
	TFile *f = new TFile();
	lCreationDate->Caption = "Дата сборки: " + f->GetLastWriteTime(Application->ExeName);
	delete f;
}
// ---------------------------------------------------------------------------
