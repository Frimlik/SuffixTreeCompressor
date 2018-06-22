#pragma once
#include <fstream>
#include <string>
#include <vector>

#include "TokenStreamBase.h"

using namespace std;

class TokenOutputStream : public TokenStreamBase
{
public:
	TokenOutputStream(const string& OutputFilePath, const string& TokenDictionaryFilePath);
	~TokenOutputStream();

	void WriteToken(const int TokenId);

	int NumberOfTokens() const;

private:
	void LoadTokenDictionary(const string& TokenDictionaryFilePath);

	ofstream outFile;
	vector<string> tokenDictionary;
};


inline void TokenOutputStream::WriteToken(const int TokenId)
{
	outFile.write(tokenDictionary[TokenId].c_str(), tokenDictionary[TokenId].size());
}

inline int TokenOutputStream::NumberOfTokens() const
{
	return tokenDictionary.size();
}