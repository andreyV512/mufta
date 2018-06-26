#pragma hdrstop
#include "TLogCh791Params.h"
//#include "uFunctions.h"
#include "TProtocol.h"
#pragma package(smart_init)

// ---------------------------------------------------------------------------
TLogCh791Params::TLogCh791Params()
{
	chName="";
	logicalChannel=0;
	collectedMode=0;
	adcRangeIndex=0;
	//adcRange=0;
	chGain=1;
}
TLogCh791Params::~TLogCh791Params(void)
{
//
}
// ---------------------------------------------------------------------------
AnsiString TLogCh791Params::ToString(void)
{
	AnsiString ret=chName;
	ret+=":  ch=";
	ret+=UIntToStr(logicalChannel);
	ret+=",mode=";
	ret+=UIntToStr(collectedMode);
	ret+=",range=";
	ret+=IntToStr(adcRangeIndex);
	return(ret);
}
// ---------------------------------------------------------------------------


