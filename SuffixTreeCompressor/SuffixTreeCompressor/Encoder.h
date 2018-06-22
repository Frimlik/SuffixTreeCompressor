#pragma once
#include "MatchStruct.h"

/**
 * <summary> An encoder that encodes matches.</summary>
 */

typedef unsigned char uchar;

class Encoder
{
	public:
		Encoder() = default;
		Encoder(short matchLengthBits, short matchBytesCount);
		~Encoder() = default;

		/**
		 * <summary> Encode match into bytes.</summary>
		 *
		 * <param name="match"> Specifies the match.</param>
		 * <param name="bytes"> The match bytes.</param>
		 */

		void EncodeMatch(Match match, uchar * bytes);

	private:
		short matchLengthBits;  ///< The number of bits that are reserved for match length
		short matchBytesCount;  ///< Number of bytes per one match
};

