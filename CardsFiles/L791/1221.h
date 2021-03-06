class DaqL1221: public LDaqBoard
{
public:
   // Base functions
   IFC(ULONG) GetWord_DM(USHORT Addr, PUSHORT Data);
   IFC(ULONG) PutWord_DM(USHORT Addr, USHORT Data);
   IFC(ULONG) PutWord_PM(USHORT Addr, ULONG Data);
   IFC(ULONG) GetWord_PM(USHORT Addr, PULONG Data);

   IFC(ULONG) GetArray_DM(USHORT Addr, ULONG Count, PUSHORT Data);
   IFC(ULONG) PutArray_DM(USHORT Addr, ULONG Count, PUSHORT Data);
   IFC(ULONG) PutArray_PM(USHORT Addr, ULONG Count, PULONG Data);
   IFC(ULONG) GetArray_PM(USHORT Addr, ULONG Count, PULONG Data);

   IFC(ULONG) SendCommand(USHORT cmd);
      
      // Service functions
   IFC(ULONG) PlataTest();

   IFC(ULONG) EnableCorrection(USHORT Ena);
      
   IFC(ULONG) LoadBios(char *FileName);

   IFC(ULONG) ReadFlashWord(USHORT Addr, PUSHORT Data);
   IFC(ULONG) WriteFlashWord(USHORT Addr, USHORT Data);

//   IFC(ULONG) GetParameter(ULONG name, PULONG param);
//   IFC(ULONG) SetParameter(ULONG name, PULONG param);

public:
   DaqL1221(ULONG Slot) :LDaqBoard(Slot) {}
   ULONG FillADCparameters(PDAQ_PAR sp);

   ULONG InputTTL(PDAQ_PAR sp); //2 in 1 all
   ULONG OutputTTL(PDAQ_PAR sp);  // in each set channel
   ULONG ConfigTTL(PDAQ_PAR sp); // 1221 and 1450

   ULONG InputADC(PDAQ_PAR sp);
   ULONG csGetParameter(ULONG name, PULONG param, ULONG status);
   ULONG csSetParameter(ULONG name, PULONG param, ULONG status);


protected:
   ULONG LOAD_ADSP2186(PUSHORT BiosCode);
   USHORT RoundFreq(double rate);

};
