#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "TokenStreamBase.h"

using namespace std;

class TokenInputStream : public TokenStreamBase
{
public:
	TokenInputStream(const string& InputFilePath, const string& TokenDictionaryFilePath);
	~TokenInputStream();

	int ReadToken();

	int NumberOfTokens() const;
	bool FileEnd();

	int tokensTotal = 0;
	int bytesRead;

private:
	enum CharacterClasses
	{
		Alphanumeric,
		Whitespace,
		EndOfFile
	};

	enum States
	{
		InitState,
		WordState,
		WhitespaceState,
		EndOfFileState
	};

	void CreateTokenDictionary(const string& InputFilePath);
	bool ParseToken(string& Token);
	void ReadChar();
	void StoreTokenDictionary(const string& TokenDictionaryFilePath);

	wifstream inFile;
	States currentState;

	CharacterClasses lastCharacterClass;
	wchar_t lastCharacter;

	unordered_map<string, int> tokenDictionary;

	bool fileEnd = false;
};


inline int TokenInputStream::ReadToken()
{
	string Token;
	if (ParseToken(Token))
	{
		return tokenDictionary[Token];
	}
	return 255;
}

inline int TokenInputStream::NumberOfTokens() const
{
	return tokenDictionary.size();
}

inline bool TokenInputStream::FileEnd()
{
	return inFile.eof();
}