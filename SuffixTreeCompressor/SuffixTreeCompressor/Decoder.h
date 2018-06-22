#pragma once
#include "DecodeResult.h"
#include "MatchStruct.h"

/**
 * <summary> A decoder for matches and bits in flags byte.</summary>
 */

typedef unsigned char uchar;

class Decoder
{
	public:
		Decoder() = default;
		Decoder(short matchLengthSizeBits, short matchBytesCount);
		~Decoder() = default;

		/**
		 * <summary> Decode bit from flagsByte.</summary>
		 *
		 * <param name="byte">	   The flagsByte.</param>
		 * <param name="bitIndex"> Bit, which identity is under focus.</param>
		 *
		 * <returns> A flag if it is a symbol-literal (1) or a match (0).</returns>
		 */

		DecodeResultEnum DecodeBit(uchar byte, int bitIndex);

		/**
		 * <summary> Decode match from matchBytes into Match struct.</summary>
		 *
		 * <param name="bytesToDecode"> The bytes to decode.</param>
		 *
		 * <returns> A decoded match.</returns>
		 */

		Match DecodeMatch(uchar * bytesToDecode);

	private:
		short matchLengthSizeBits;  ///< The bits size of match length (max length of 3 occupy 2 bits, max length of 31 occupy 5 bits etc.)
		short matchBytesCount;		///< Total bytes num that one match occupy - (bits for index + bits for length) / 8
};

