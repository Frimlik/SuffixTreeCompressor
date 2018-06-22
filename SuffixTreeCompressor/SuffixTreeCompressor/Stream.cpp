//
//#include "Stream.h"
//
//template <typename T>
//void InStream<T>::Open(const char* fileName, bool compression, bool binaryFile)
//{
//	this->bytesRead = 0;
//
//	if (compression && !binaryFile)
//		inFile.open(fileName);
//	else
//		inFile.open(fileName, ifstream::binary);
//
//	if (inFile.fail())
//	{
//		fprintf(stderr, "FATAL:\tCan't open file \"%s\"\n", fileName);
//		exit(1);
//	}
//
//	inFile >> noskipws;
//	this->compression = compression;
//}
//
//template <typename T>
//T InStream<T>::ReadSymbol()
//{
//	inFile >> symbol;
//
//	if (!inFile.eof())
//	{
//		this->bytesRead++;
//	}
//
//	return symbol;
//}
//
//template <typename T>
//int InStream<T>::GetBytesRead()
//{
//	return bytesRead;
//}
//
//template <typename T>
//void InStream<T>::Close()
//{
//	this->inFile.close();
//}
//
//template <typename T>
//bool InStream<T>::EndOfFile()
//{
//	return inFile.eof();
//}
//
//template <typename T>
//void OutStream<T>::Open(const char * fileName, bool compression, bool binaryFile)
//{
//	this->bytesWritten = 0;
//
//	if (compression || binaryFile)
//		outFile.open(fileName, ifstream::binary);
//	else
//		outFile.open(fileName);
//
//	if (outFile.fail())
//	{
//		fprintf(stderr, "FATAL:\tCan't open file \"%s\"\n", fileName);
//		exit(1);
//	}
//
//	this->compression = compression;
//	this->binaryFile = binaryFile;
//}
//
//template <typename T>
//void OutStream<T>::WriteSymbol(T symbol)
//{
//	if (compression)
//	{
//		outFile << symbol;
//
//		this->bytesWritten++;
//	}
//	else
//	{
//		outFile << symbol;
//
//		this->bytesWritten++;
//	}
//}
//
//template <typename T>
//int OutStream<T>::GetBytesWritten()
//{
//	return this->bytesWritten;
//}
//
//template <typename T>
//void OutStream<T>::Close()
//{
//	this->outFile.close();
//}