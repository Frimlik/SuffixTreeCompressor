#pragma once
#include "TokenInputStream.h"
#include "TokenOutputStream.h"

class SuffixTreeAux
{
public:
	virtual void InitForCompression(const char * inFile, const char * outFile, short matchLengthSizeBits, bool binaryFile) = 0;
	virtual void InitForDecompression(const char * inFile, const char * outFile, short matchLengthSizeBits, bool binaryFile) = 0;
	virtual void FinishCompression() = 0;
	virtual void FinishDecompression() = 0;
	virtual void Compress(long int inputFileSize, bool showProgress, TokenInputStream * inputStream) = 0;
	virtual void Decompress(long int inputFileSize, bool showProgress, TokenOutputStream * outputStream) = 0;
	virtual void WriteCompressionStatistics() = 0;
	virtual void WriteDecompressionStatistics() = 0;
};