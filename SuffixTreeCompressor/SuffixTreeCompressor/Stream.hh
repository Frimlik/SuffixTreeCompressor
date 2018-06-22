#pragma once
#include <stdlib.h>
#include <fstream>

using namespace std;

/**
 * <summary> Class that represents reader from file.</summary>
 */

typedef unsigned char uchar;
//Declaration----------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T> 
class InStream
{
	private:
		ifstream inFile;		///< Input file to read from
		int bytesRead;			///< The bytes currently read from file
		bool compression;		///< Flag if compression/decompression is happening
		T symbol;			///< The symbol read

	public:
		InStream() = default;
		~InStream() = default;

		/**
		 * <summary> Opens the given file for reading symbols. File is opened in mode given by flag
		 *  compression (normal/binary).</summary>
		 *
		 * <param name="fileName">    Filename of the opened file.</param>
		 * <param name="compression"> Open in normal mode if compressing, otherwise open in binary mode.</param>
		 * <param name="binaryFile">  Whether compressed file is binary.</param>
		 */

		void Open(const char* fileName, bool compression, bool binaryFile = false);

		/**
		 * <summary> Reads byte while compression.</summary>
		 *
		 * <returns> The byte.</returns>
		 */

		uchar ReadByte();

		/**
		 * <summary> Reads single symbol from file.</summary>
		 *
		 * <returns> The symbol read from file.</returns>
		 */

		T ReadSymbol();

		/**
		* <summary> Reads single symbol from file in binary mode.</summary>
		*
		* <returns> The symbol read from file.</returns>
		*/

		T ReadSymbolBin();

		/**
		 * <summary> Gets bytes read from file up to now.</summary>
		 *
		 * <returns> The bytes read.</returns>
		 */

		int GetBytesRead();

		/**
		 * <summary> Closes the resource that I read from.</summary>
		 */

		void Close();

		/**
		 * <summary> Check if the input stream gets to the end of file.</summary>
		 *
		 * <returns> True if it accessed EOF, otherwise false.</returns>
		 */

		bool EndOfFile();
};

template <typename T> 
class OutStream
{
	private:
		ofstream outFile;   ///< Output file to write to
		int bytesWritten;   ///< The bytes currently written to file
		bool compression;   ///< Flag if compression/decompression is happening
		bool binaryFile;	///< Whether the output file is binary

	public:
		OutStream() = default;
		~OutStream() = default;

		/**
		 * <summary> Opens the given file for writing symbols. File is opened in mode given by flag
		 *  compression (normal/binary).</summary>
		 *
		 * <param name="fileName">    Filename of the opened file.</param>
		 * <param name="compression"> Open in binary mode if compressing, otherwise open in normal mode.</param>
		 * <param name="binaryFile">  Whether decompressed file is binary.</param>
		 */

		void Open(const char* fileName, bool compression, bool binaryFile = false);

		/**
		* <summary> Writes a match byte to file.</summary>
		*
		* <param name="symbol"> The symbol to write.</param>
		*/

		void WriteByte(uchar byte);

		/**
		 * <summary> Writes single symbol to file.</summary>
		 *
		 * <param name="symbol"> The symbol to write.</param>
		 */

		void WriteSymbol(T symbol);

		/**
		 * <summary> Gets bytes written to file up to now.</summary>
		 *
		 * <returns> The bytes written.</returns>
		 */

		int GetBytesWritten();

		/**
		 * <summary> Closes the resource that I write to.</summary>
		 *
		 * <remarks> Jirka, 16. 3. 2018.</remarks>
		 */

		void Close();
};

//Definition----------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
void InStream<T>::Open(const char* fileName, bool compression, bool binaryFile)
{
	this->bytesRead = 0;

	inFile.open(fileName, ifstream::in | ifstream::binary);

	if (inFile.fail())
	{
		fprintf(stderr, "FATAL:\tCan't open file \"%s\"\n", fileName);
		exit(1);
	}

	inFile >> noskipws;
	this->compression = compression;
}

template<typename T>
uchar InStream<T>::ReadByte()
{
	uchar byte;
	byte = inFile.get();
	this->bytesRead++;
	return byte;
}

template <typename T>
T InStream<T>::ReadSymbol()
{
	inFile >> symbol;

	if (!inFile.eof())
	{
		this->bytesRead += sizeof(T);
	}

	return symbol;
}

template<typename T>
inline T InStream<T>::ReadSymbolBin()
{
	inFile.read(reinterpret_cast<char *>(&symbol), sizeof symbol);

	if (!inFile.eof())
	{
		this->bytesRead += sizeof(T);
	}
	
	return symbol;
}

template <typename T>
int InStream<T>::GetBytesRead()
{
	return bytesRead;
}

template <typename T>
void InStream<T>::Close()
{
	this->inFile.close();
}

template <typename T>
bool InStream<T>::EndOfFile()
{
	return inFile.eof();
}


template <typename T>
void OutStream<T>::Open(const char * fileName, bool compression, bool binaryFile)
{
	this->bytesWritten = 0;

	if (compression || binaryFile)
		outFile.open(fileName, ofstream::out | ofstream::binary);
	else
		outFile.open(fileName);

	if (outFile.fail())
	{
		fprintf(stderr, "FATAL:\tCan't open file \"%s\"\n", fileName);
		exit(1);
	}

	this->compression = compression;
	this->binaryFile = binaryFile;
}

template<typename T>
inline void OutStream<T>::WriteByte(uchar byte)
{
	outFile << byte;

	this->bytesWritten++;
}

template <typename T>
void OutStream<T>::WriteSymbol(T symbol)
{
	outFile << symbol;

	this->bytesWritten += sizeof(T);
}

template <typename T>
int OutStream<T>::GetBytesWritten()
{
	return this->bytesWritten;
}

template <typename T>
void OutStream<T>::Close()
{
	this->outFile.close();
}