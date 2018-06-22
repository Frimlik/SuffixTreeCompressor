#pragma once
#include "Stream.hh"

/**
 * <summary> Helper to handle output bytes - save them to group them into groups of 8 and then output them.</summary>
 */

template <typename T> 
class OutputBytesHelper
{
	public:
		OutputBytesHelper(const char * outFileName, short bytesPerMatchCount, OutStream<T> * outStream);
		~OutputBytesHelper();

		/**
		 * <summary> Appends a symbol to bytesForOutput array.</summary>
		 *
		 * <param name="symbol"> The symbol to be appended.</param>
		 */

		void AppendSymbol(T symbol);

		/**
		 * <summary> Appends a match to bytesForOutput array.</summary>
		 *
		 * <param name="matchBytes"> Match bytes to be appended.</param>
		 */

		void AppendMatch(uchar * matchBytes);

		/**
		 * <summary> Gets number of flags that are remaining to be output.</summary>
		 *
		 * <returns> The remaining flags count.</returns>
		 */

		short GetRemainingFlagsCount();

		/**
		 * <summary> Finishes a work - write the remaining bytesToOutput to outStream prepended by zeroLengthMatch (signal of end).</summary>
		 *
		 * <param name="zeroLengthMatchBytes"> Bytes of match that is zero-length.</param>
		 */

		void FinishWork(uchar * zeroLengthMatchBytes);

	private:
		OutStream<T> * outStream;		///< The stream to write data to
		short bytesPerMatchCount;	///< The number of bytes per match - number of bits reserved for index in sliding window + number of bits reserved for match length
		uchar flagsByte;			///< The byte with flags signaling symbol or match (1 = symbol, 0 = match)
		short flagsCount;			///< The number of flags currently saved in flagsByte
		short bytesToOutputCount;	///< The number of bytes currently saved in bytesToOutput
		uchar * bytesToOutput;		///< The bytes to output

		/**
		 * <summary> Check flags count - if there is 8 of them, output them.</summary>
		 */

		void CheckFlagsCount();
};

//Definition----------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
OutputBytesHelper<T>::OutputBytesHelper(const char * outFileName, short bytesPerMatchCount, OutStream<T> * outStream)
{
	this->outStream = outStream;
	this->bytesPerMatchCount = bytesPerMatchCount;
	this->flagsCount = 0;
	this->bytesToOutputCount = 0;

	short maxBytesPerItemUsed = (bytesPerMatchCount > sizeof(T)) ? bytesPerMatchCount : sizeof(T);	//T místo int; 
	this->bytesToOutput = new uchar[8 * maxBytesPerItemUsed];
}

template <typename T>
OutputBytesHelper<T>::~OutputBytesHelper()
{
	delete[] this->bytesToOutput;
}

template <typename T>
void OutputBytesHelper<T>::AppendSymbol(T symbol)
{
	uchar flagMask = 1 << (8 - flagsCount - 1);
	flagsByte |= flagMask;

	//take byte by byte of symbol from left to right (shift to the right by multipliers of 8 and leave set only rightmost byte)
	for (short i = sizeof(T) - 1; i >= 0; i--)
	{
		bytesToOutput[bytesToOutputCount++] = (symbol >> (i * 8)) & 0xff;
	}

	flagsCount++;

	CheckFlagsCount();
}

template <typename T>
void OutputBytesHelper<T>::AppendMatch(uchar * matchBytes)
{
	uchar flagMask = 1 << (8 - flagsCount - 1);
	flagsByte &= ~flagMask;

	for (short i = 0; i < bytesPerMatchCount; i++)
	{
		bytesToOutput[bytesToOutputCount + i] = matchBytes[i];
	}

	bytesToOutputCount += bytesPerMatchCount;
	flagsCount++;

	CheckFlagsCount();
}

template <typename T>
short OutputBytesHelper<T>::GetRemainingFlagsCount()
{
	return flagsCount;
}

template <typename T>
void OutputBytesHelper<T>::FinishWork(uchar * zeroLengthMatchBytes)
{
	flagsByte >>= 1;
	flagsByte &= ~(1 << 7);

	outStream->WriteByte(flagsByte);

	for (short i = 0; i < bytesPerMatchCount; i++)
	{
		outStream->WriteByte(zeroLengthMatchBytes[i]);
	}

	for (short i = 0; i < bytesToOutputCount; i++)
	{
		outStream->WriteByte(bytesToOutput[i]);
	}
}

template <typename T>
void OutputBytesHelper<T>::CheckFlagsCount()
{
	if (flagsCount == 8)
	{
		outStream->WriteByte(flagsByte);
		for (short i = 0; i < bytesToOutputCount; i++)
		{
			outStream->WriteByte(bytesToOutput[i]);
		}

		flagsCount = bytesToOutputCount = flagsByte = 0;
	}
}