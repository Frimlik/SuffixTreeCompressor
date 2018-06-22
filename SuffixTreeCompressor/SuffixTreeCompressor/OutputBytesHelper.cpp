//
//#include "OutputBytesHelper.h"
//
//template <typename T> 
//OutputBytesHelper<T>::OutputBytesHelper(const char * outFileName, short bytesPerMatchCount, OutStream<T> * outStream)
//{
//	this->outStream = outStream;
//	this->bytesPerMatchCount = bytesPerMatchCount;
//	this->flagsCount = 0;
//	this->bytesToOutputCount = 0;
//
//	short maxBytesPerItemUsed = (bytesPerMatchCount > sizeof(uchar)) ? bytesPerMatchCount : sizeof(uchar);	//T místo int; 
//	this->bytesToOutput = new uchar[8 * maxBytesPerItemUsed];
//}
//
//template <typename T> 
//OutputBytesHelper<T>::~OutputBytesHelper()
//{
//	delete[] this->bytesToOutput;
//}
//
//template <typename T> 
//void OutputBytesHelper<T>::AppendSymbol(uchar symbol)
//{
//	uchar flagMask = 1 << (8 - flagsCount - 1);
//	flagsByte |= flagMask;
//
//	//shift symbol to the right by multipliers of 8 and leave only rightmost 8 bits set	
//	bytesToOutput[bytesToOutputCount++] = symbol;
//
//	flagsCount++;
//
//	CheckFlagsCount();
//}
//
//template <typename T> 
//void OutputBytesHelper<T>::AppendMatch(uchar * matchBytes)
//{
//	uchar flagMask = 1 << (8 - flagsCount - 1);
//	flagsByte &= ~flagMask;
//
//	for (short i = 0; i < bytesPerMatchCount; i++)
//	{
//		bytesToOutput[bytesToOutputCount + i] = matchBytes[i];
//	}
//
//	bytesToOutputCount += bytesPerMatchCount;
//	flagsCount++;
//
//	CheckFlagsCount();
//}
//
//template <typename T> 
//short OutputBytesHelper<T>::GetRemainingFlagsCount()
//{
//	return flagsCount;
//}
//
//template <typename T> 
//void OutputBytesHelper<T>::FinishWork(uchar * zeroLengthMatchBytes)
//{
//	flagsByte >>= 1;
//	flagsByte &= ~(1 << 7);
//		
//	outStream->WriteSymbol(flagsByte);
//
//	for (short i = 0; i < bytesPerMatchCount; i++)
//	{
//		outStream->WriteSymbol(zeroLengthMatchBytes[i]);
//	}
//		
//	for (short i = 0; i < bytesToOutputCount; i++)
//	{
//		outStream->WriteSymbol(bytesToOutput[i]);
//	}
//}
//
//template <typename T> 
//void OutputBytesHelper<T>::CheckFlagsCount()
//{
//	if (flagsCount == 8)
//	{
//		outStream->WriteSymbol(flagsByte);
//		for (short i = 0; i < bytesToOutputCount; i++)
//		{
//			outStream->WriteSymbol(bytesToOutput[i]);
//		}
//
//		flagsCount = bytesToOutputCount = flagsByte = 0;
//	}
//}