#pragma once

/**
 * <summary> A struct to save match found in suffix tree.</summary>
 */

struct Match
{
	public:
		int MatchIndex;		///< Index of current match in sliding window
		int MatchLength;	///< Length of the current match

		Match() = default;
		Match(int index, int length)
		{
			this->MatchIndex = index;
			this->MatchLength = length;
		}
};

int BytesCnt2BitsCnt(int bytesCount);