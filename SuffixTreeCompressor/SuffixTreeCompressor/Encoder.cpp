
#include <math.h>

#include "Encoder.h"

Encoder::Encoder(short matchLengthBits, short matchBytesCount)
{
	this->matchLengthBits = matchLengthBits;
	this->matchBytesCount = matchBytesCount;
}

void Encoder::EncodeMatch(Match match, uchar * bytes)
{
	int resultCode = match.MatchIndex << matchLengthBits;
	resultCode |= match.MatchLength;

	for (short i = 0; i < matchBytesCount; i++)
	{
		//take "matchBytesCount" of resultCode from left to right
		//shift resultCode to the right by multipliers of 8 and leave only rightmost 8 bits set
		bytes[i] = (resultCode >> (8 * (matchBytesCount - 1 - i))) & ((1 << 8) - 1);
	}
}