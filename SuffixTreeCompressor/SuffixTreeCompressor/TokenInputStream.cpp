#include "TokenInputStream.h"

TokenInputStream::TokenInputStream(const string& InputFilePath, const string& TokenDictionaryFilePath)
{
	bytesRead = 0;
	CreateTokenDictionary(InputFilePath);
	StoreTokenDictionary(TokenDictionaryFilePath);
	inFile.open(InputFilePath, wifstream::binary);
	inFile >> noskipws;
	currentState = InitState;
	ReadChar();
}

TokenInputStream::~TokenInputStream()
{
	inFile.close();
}

void TokenInputStream::CreateTokenDictionary(const string& InputFilePath)
{
	string Token;
	int TokenIdCounter = 0;
	inFile.open(InputFilePath, wifstream::binary);
	inFile >> noskipws;
	currentState = InitState;
	ReadChar();
	while (ParseToken(Token))
	{
		if (tokenDictionary.find(Token) == tokenDictionary.end())
		{
			tokenDictionary.emplace(Token, TokenIdCounter);
			TokenIdCounter += 1;
		}
		tokensTotal++;
	}
	inFile.close();
}

bool TokenInputStream::ParseToken(string& Token)
{
	bool bRet = true;
	bool flag = true;
	Token.clear();
	while (flag)
	{
		switch (currentState)
		{
		case States::InitState:
			if (lastCharacterClass == CharacterClasses::EndOfFile)
			{
				currentState = States::EndOfFileState;
				flag = false;
				bRet = false;
			}
			else
			{
				Token += lastCharacter;
				currentState = lastCharacterClass == CharacterClasses::Alphanumeric ? States::WordState : States::WhitespaceState;
				ReadChar();
			}
			break;

		case States::WordState:
			if (lastCharacterClass == CharacterClasses::Alphanumeric)
			{
				Token += lastCharacter;
				ReadChar();
			}
			else
			{
				currentState = lastCharacterClass == CharacterClasses::Whitespace ? States::WhitespaceState : States::EndOfFileState;
				flag = false;
			}
			break;

		case States::WhitespaceState:
			if (lastCharacterClass == CharacterClasses::Whitespace)
			{
				Token += lastCharacter;
				ReadChar();
			}
			else
			{
				currentState = lastCharacterClass == CharacterClasses::Alphanumeric ? States::WordState : States::EndOfFileState;
				flag = false;
			}
			break;

		case States::EndOfFileState:
			flag = false;
			bRet = false;
			break;
		}
	}
	return bRet;
}

void TokenInputStream::ReadChar()
{
	if (inFile.get(lastCharacter))
	{
		lastCharacterClass = iswalnum(lastCharacter) != 0 ? CharacterClasses::Alphanumeric : CharacterClasses::Whitespace;
	}
	else
	{
		lastCharacterClass = CharacterClasses::EndOfFile;
	}
	bytesRead += sizeof(lastCharacter);
}

void TokenInputStream::StoreTokenDictionary(const string& TokenDictionaryFilePath)
{
	ofstream output(TokenDictionaryFilePath, ios::out | ios::binary);
	int size = tokenDictionary.size();
	output.write(reinterpret_cast<char *>(&size), sizeof(int));
	vector<int> TokenIdBuffer;
	for (auto &x : tokenDictionary)
	{
		TokenIdBuffer.push_back(x.second);
	}
	output.write(reinterpret_cast<char *>(TokenIdBuffer.data()), sizeof(int) * size);
	for (auto &x : tokenDictionary)
	{
		size = x.first.size();
		output.write(reinterpret_cast<char *>(&size), sizeof(int));
		output.write(x.first.c_str(), size);
	}
	output.close();
}
