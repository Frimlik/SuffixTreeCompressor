
#include "Decoder.h"


Decoder::Decoder(short matchLengthSizeBits, short matchBytesCount)
{
	this->matchLengthSizeBits = matchLengthSizeBits;
	this->matchBytesCount = matchBytesCount;
}

DecodeResultEnum Decoder::DecodeBit(uchar byte, int bitIndex)
{
	int mask = 1 << (8 - bitIndex - 1);

	if ((byte & mask) > 0)
		return DecodeResultEnum::LiteralResult;

	return DecodeResultEnum::MatchResult;
}

Match Decoder::DecodeMatch(uchar * bytesToDecode)
{
	int value = 0;
	for (short i = 0; i < matchBytesCount; i++)
	{
		value |= bytesToDecode[i] << BytesCnt2BitsCnt(matchBytesCount - i - 1);
	}

	int matchLengthMask = (1 << this->matchLengthSizeBits) - 1;
	int matchIndex = value >> matchLengthSizeBits;
	int matchLength = value & matchLengthMask;

	return Match(matchIndex, matchLength);
}