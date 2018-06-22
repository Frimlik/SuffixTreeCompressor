#include "TokenOutputStream.h"

TokenOutputStream::TokenOutputStream(const string& OutputFilePath, const string& TokenDictionaryFilePath)
{
	LoadTokenDictionary(TokenDictionaryFilePath);
	outFile.open(OutputFilePath);
}

TokenOutputStream::~TokenOutputStream()
{
	outFile.close();
}

void TokenOutputStream::LoadTokenDictionary(const string& TokenDictionaryFilePath)
{
	ifstream input(TokenDictionaryFilePath, ios::in | ios::binary);
	int DictionarySize;
	input.read(reinterpret_cast<char *>(&DictionarySize), sizeof(int));
	int *TokenIdBuffer = new int[DictionarySize];
	input.read(reinterpret_cast<char *>(TokenIdBuffer), sizeof(int) * DictionarySize);
	tokenDictionary.resize(DictionarySize);
	int MaxSize = 1024;
	char* buffer = new char[MaxSize];
	for (int i = 0; i < DictionarySize; i++)
	{
		int id = TokenIdBuffer[i];
		int size;
		input.read(reinterpret_cast<char *>(&size), sizeof(int));
		if (size > MaxSize)
		{
			delete[] buffer;
			MaxSize = size;
			buffer = new char[MaxSize];
		}
		input.read(buffer, size);
		tokenDictionary[id].assign(buffer, size);
	}
	input.close();
	delete[]buffer;
	delete[] TokenIdBuffer;
}
