#pragma once
#include <iostream>

#include "Stream.hh"
#include "MatchStruct.h"

using namespace std;

/**
 * <summary> Class that handles current match found in suffix tree.</summary>
 */

 //Declaration----------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class MatchHelper
{
	public:
		int MatchPosition;			///< The match position relative to window front
		int MatchBufPosition;		///< The match position relative to start of buffer

		MatchHelper() = default;
		MatchHelper(int matchBitsSize, T * matchChars, short bytesPerMatchCount);
		~MatchHelper() = default;

		/**
		 * <summary> Gets a match and its length.</summary>
		 *
		 * <param name="returnMatch"> The match returned.</param>
		 *
		 * <returns> Length of match. If negative, there is no match.</returns>
		 */

		short GetMatch(Match & returnMatch);

		/**
		 * <summary> Gets match length.</summary>
		 *
		 * <returns> The match length.</returns>
		 */

		int GetMatchLength();

		/**
		 * <summary> Move match one symbol forward, if it is possible.</summary>
		 *
		 * <param name="position"> The new position, if needed.</param>
		 * <param name="symbol">   The symbol at the new position.</param>
		 *
		 * <returns> True if it succeeds, false if it fails.</returns>
		 */

		bool MoveMatchForward(int position, int positionInBuffer, T symbol);

		/**
		 * <summary> Gets match with zero length - the last match to output.</summary>
		 *
		 * <param name="remainingBytesCount"> Number of remaining bytes.</param>
		 *
		 * <returns> The match with zero length.</returns>
		 */

		Match GetMatchWithZeroLength(short remainingBytesCount);
	private:
		int matchLength;			///< Current match length
		int matchLengthMaxValue;	///< The maximum match length (not to overflow max bits size)
		T * matchChars;			///< The match characters that are used if the match is not long enough
		short bytesPerMatchCount;   ///< Number of bytes per one match
};

//Definition----------------------------------------------------------------------------------------------------------------------------------------------------------

template<typename T>
MatchHelper<T>::MatchHelper(int matchBitsSize, T * matchChars, short bytesPerMatchCount)
{
	this->matchLength = 0;
	this->MatchPosition = -1;
	this->matchLengthMaxValue = (1 << matchBitsSize) - 1;
	this->matchChars = matchChars;
	this->bytesPerMatchCount = bytesPerMatchCount;
}

template<typename T>
short MatchHelper<T>::GetMatch(Match & returnMatch)
{
	short returnValue;
	//reference match of length 1 is useless
	if (matchLength > bytesPerMatchCount)
	{
		returnMatch.MatchIndex = MatchPosition;
		returnMatch.MatchLength = matchLength;

		returnValue = -1;
	}
	else
	{
		returnValue = matchLength;
	}

	this->matchLength = 0;
	this->MatchPosition = -1;

	return returnValue;
}

template<typename T>
int MatchHelper<T>::GetMatchLength()
{
	return this->matchLength;
}

template<typename T>
bool MatchHelper<T>::MoveMatchForward(int position, int positionInBuffer, T symbol)
{
	//for case the match would not be long enough to be saved
	if (matchLength < bytesPerMatchCount)
	{
		matchChars[matchLength] = symbol;
	}
	matchLength++;

	if (matchLength >= matchLengthMaxValue)
	{
		return false;
	}

	if (MatchPosition == -1)
	{
		MatchPosition = position;
		MatchBufPosition = positionInBuffer;
	}

	return true;
}

template<typename T>
Match MatchHelper<T>::GetMatchWithZeroLength(short remainingBytesCount)
{
	return Match(remainingBytesCount, 0);
}
