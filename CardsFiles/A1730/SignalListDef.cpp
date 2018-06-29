#pragma hdrstop
#include "SignalListDef.h"
#pragma package(smart_init)
SignalListDef* SLD = NULL;

//SignalListDef::SignalListDef(TIniFile* _ini) : SignalList(_ini)
//ñïèñîê ñèãíàëîâ îò 1730
SignalListDef::SignalListDef() : SignalList()
{
//	iCC = Find("ÖÅÏÈ ÓÏĞÀÂËÅÍÈß", true);
//	iLPCHA = Find("Ïğîä Ï× À", true);
//	iLPCHRUN = Find("Ïğîä Ï× RUN", true);
//	iCSTROBE = Find("ÑÒĞÎÁ 2", true);
//	iLSTROBE = Find("ÑÒĞÎÁ 3", true);
//	iCCYCLE = Find("ÖÈÊË 2", true);
//	iLCYCLE = Find("ÖÈÊË 3", true);
//	iREADY = Find("Ãîòîâíîñòü", true);
//	iCCONTROL = Find("ÊÎÍÒĞÎËÜ 2", true);
//	iLCONTROL = Find("ÊÎÍÒĞÎËÜ 3", true);
//	iSOP = Find("ĞÅÆÈÌ ÑÎÏ", true);

//	iINMODULE = Find("ÌÓÔÒÀ Â ÌÎÄÓËÅ", true);
//	iZEROTRANSITION = Find("ÏÅĞÅÕÎÄ ×ÅĞÅÇ 0", true);
	iCC = Find("ÖÅÏÈ ÓÏĞÀÂËÅÍÈß", true);
	iPOW12 = Find("ÏÈÒÀÍÈÅ 12Â", true);

	oSENSORON = Find("ÂÊË ÑÈËÎÂÎÅ", false);
	oSENSLOWPOW = Find("ÂÊË ÏÈÒÀÍÈÅ", false);
//	oRESERV1 = Find("ĞÅÇÅĞÂ 1", false);
	oRESERV2 = Find("ĞÅÇÅĞÂ 2", false);

//	oLPCHPOW = Find("ÏÈÒÀÍÈÅ Ï× ÌÎÄ.3", false);
//	oLSCANPOW = Find("ÏÈÒÀÍÈÅ ÑÓ ÌÎÄ.3", false);
//	oLSOLPOW = Find("ÍÀÌÀÃÍÈ×ÈÂÀÍÈÅ 3", false);
//	oCSOLPOW = Find("ÍÀÌÀÃÍÈ×ÈÂÀÍÈÅ 2", false);
//	oCWORK = Find("ĞÀÁÎÒÀ 2", false);
//	oLWORK = Find("ĞÀÁÎÒÀ 3", false);
//	oCMEAS = Find("ÈÇÌÅĞÅÍÈÅ 2", false);
//	oLMEAS = Find("ÈÇÌÅĞÅÍÈÅ 3", false);
//	oSHIFT = Find("ÏÅĞÅÊËÀÄÊÀ", false);
//	oCSTROBE = Find("ÑÒĞÎÁ ÇÎÍÛ 2", false);
//	oLSTROBE = Find("ÑÒĞÎÁ ÇÎÍÛ 3", false);
//	oCRESULT = Find("ĞÅÇÓËÜÒÀÒ 2", false);
//	oLRESULT = Find("ĞÅÇÓËÜÒÀÒ 3", false);

//	CrossCycle = false;
//	LinearCycle = false;
//	InvA = false;
}

//void SignalListDef::SetCrossCycle(bool _v)
//{
//	cs->Enter();
//	{
//		CrossCycle = _v;
//	} cs->Leave();
//}
//
//void SignalListDef::SetLinearCycle(bool _v)
//{
//	cs->Enter();
//	{
//		LinearCycle = _v;
//	} cs->Leave();
//}
//
//void SignalListDef::SetInvA(bool _v)
//{
//	cs->Enter();
//	{
//		InvA = _v;
//	} cs->Leave();
//}

bool SignalListDef::CheckAlarm(void)
{
//	if (iCC->WasConst0(false, 50))
//	{
//		AlarmList->Add("Íåò ñèãíàëà öåïè óïğàâëåíèÿ");
//		wasAlarm = true;
//	}
//	if (LinearCycle)
//	{
//		if (iLCYCLE->WasConst0(false, 50))
//		{
//			AlarmList->Add("Ïğîïàë ñèãíàë Ïğîäîëüíûé Öèêë");
//			wasAlarm = true;
//		}
//	}
//	if (CrossCycle)
//	{
//		if (iCCYCLE->WasConst0(false, 50))
//		{
//			AlarmList->Add("Ïğîïàë ñèãíàë Ïîïåğå÷íûé öèêë");
//			wasAlarm = true;
//		}
//	}
//	if (InvA)
//	{
//		if (iLPCHA->WasConst0(false, 50))
//		{
//			AlarmList->Add("Àâàğèÿ: ïğîïàë ñèãíàë Ïğîä Ï× À");
//			wasAlarm = true;
//		}
//	}
//	if (wasAlarm)
//		prAlarm();
	return (wasAlarm);
}

void SignalListDef::SetAlarm(bool _on)
{
	SetAlarm0(_on);
	cs->Enter();
	{
//		InvA = false;
	} cs->Leave();
}

void SignalListDef::Drop(void)
{
//	if (oCSTROBE->WasConst0(true, 100))
//		oCSTROBE->Set0(false);
//	if (oLSTROBE->WasConst0(true, 100))
//		oLSTROBE->Set0(false);
//	if (oCRESULT->WasConst0(true, 100))
//		oCRESULT->Set0(false);
//	if (oLRESULT->WasConst0(true, 100))
//		oLRESULT->Set0(false);
}
