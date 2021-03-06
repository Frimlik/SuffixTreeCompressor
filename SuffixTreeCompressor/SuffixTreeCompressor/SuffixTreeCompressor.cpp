// SuffixTreeCompressor.cpp : Defines the entry point for the console application.
//


#include "cxxopts.hpp"
#include <math.h>
#include <chrono>
#include <iomanip>

#include "SuffixTree.hh"

int main(int argc, char ** argv)
{
	bool compress = true;
	bool binary = false;
	bool showProgress = false;

	string inputFile;
	string outputFile;

	int windowSizeBits = 17;
	int windowSize = 1 << windowSizeBits;

	cxxopts::Options options("MyProgram", "One line description of MyProgram");
	options.add_options()
		("c,comp", "compression", cxxopts::value<bool>())
		("d,decomp", "decompression", cxxopts::value<bool>())
		("b,binary", "compress binary file", cxxopts::value<bool>())
		("p,progress", "show progress", cxxopts::value<bool>())
		("s,windowSizeShifted", "window size - shifted 2", cxxopts::value<int>())
		("i,input", "Input", cxxopts::value<std::string>())
		("o,output", "Output file", cxxopts::value<std::string>())
		("positional",
			"Positional arguments: these are the arguments that are entered "
			"without an option", cxxopts::value<std::vector<std::string>>());
	options.parse_positional({ "input", "output", "positional" });

	auto result = options.parse(argc, argv);

	if (result.count("c"))
	{
		compress = true;
	}
	else if (result.count("d"))
	{
		compress = false;
	}

	if (result.count("s"))
	{
		windowSizeBits = result["s"].as<int>();
		windowSize = 1 << result["s"].as<int>();

		if (windowSizeBits < 2)
		{
			cerr << "Too small window. Choose value in range 2-25." << endl;
			exit(1);
		}
	}

	int matchLengthBits = (8 - windowSizeBits % 8) - 1;
	//if (matchLengthBits < 3) matchLengthBits += 8;

	int inputSize = 0;
	if (result.count("input"))
	{
		inputFile = result["input"].as<std::string>();
		ifstream in(inputFile.c_str(), std::ifstream::ate);
		inputSize = (double)in.tellg();
		in.close();
	}
	else
	{
		cerr << "Input file argument missing." << endl;
		return 0;
	}

	if (result.count("output"))
	{
		outputFile = result["output"].as<std::string>();
	}
	else
	{
		cerr << "Output file argument missing." << endl;
		exit(1);
	}

	if (result.count("b"))
	{
		binary = true;
	}

	if (result.count("p"))
	{
		showProgress = true;
	}

	cout << (compress ? "\tCompression" : "\tDecompression") << endl;
	cout << "\tSliding window size: " << windowSize << " (2^" << windowSizeBits << ") characters." << endl;
	cout << "\tInput file: " << inputFile << "; Size: " << (double)inputSize / 1000 << " Kbytes." << endl;
	cout << "\tOutput file: " << outputFile << endl;
	cout << (binary ? "\tInput is binary file." : "\tInput is text file.") << endl;

	unsigned short charSize = 1 << 8;
	unsigned int shortSize = 1 << 16;
	unsigned long long intSize = 1;
	intSize <<= 32;

	TokenStreamBase* tokenStream;
	SuffixTreeAux* tree;
	int tokensTotal = 0;

	chrono::time_point<chrono::steady_clock> starTime;
	chrono::time_point<chrono::steady_clock> endTime;

	starTime = chrono::high_resolution_clock::now();

	if (compress)
	{
		tokenStream = new TokenInputStream(inputFile, inputFile + ".dat");
		tokensTotal = static_cast<TokenInputStream*>(tokenStream)->tokensTotal;
		cout << "\tTokens totally: " << tokensTotal << endl;
		cout << "\tUnique tokens: " << static_cast<TokenInputStream*>(tokenStream)->NumberOfTokens();
		cout << "\tBytes read: " << static_cast<TokenInputStream*>(tokenStream)->bytesRead << endl;
	}
	else
	{
		string datFileName = outputFile.substr(0, outputFile.length() - 4) + ".dat";
		ifstream f(datFileName.c_str());
		if (!f.good())
		{
			cerr << "File " << datFileName << " is missing." << endl;
			return 0;
		};

		tokenStream = new TokenOutputStream(outputFile, datFileName);
	}
	cout << "\tDictionary loaded!" << endl;

	long long tokensNum = tokenStream->NumberOfTokens();
	if (tokensNum < charSize)
		tree = new SuffixTree<unsigned char>(windowSize, windowSizeBits, matchLengthBits);
	else if (tokensNum < shortSize)
		tree = new SuffixTree<unsigned short>(windowSize, windowSizeBits, matchLengthBits);
	else if (tokensNum < intSize)
		tree = new SuffixTree<unsigned int>(windowSize, windowSizeBits, matchLengthBits);
	else
		tree = new SuffixTree<unsigned long long>(windowSize, windowSizeBits, matchLengthBits);

	if (compress)
	{
		tree->InitForCompression(inputFile.c_str(), outputFile.c_str(), matchLengthBits, binary);

		cout << "\t---------------------------------------------" << endl;
		cout << "\tCompression started..." << endl;
		tree->Compress(tokensTotal, showProgress, static_cast<TokenInputStream*>(tokenStream));
		cout << endl << "\tCompression finished..." << endl;
		cout << "\t---------------------------------------------" << endl;
		endTime = chrono::high_resolution_clock::now();

		cout << "Compression was successful. Compressed file is here: " << outputFile << endl;

		tree->WriteCompressionStatistics();
		tree->FinishCompression();
	}
	else
	{
		tree->InitForDecompression(inputFile.c_str(), outputFile.c_str(), matchLengthBits, binary);

		starTime = chrono::high_resolution_clock::now();
		cout << "\t---------------------------------------------" << endl;
		cout << "\tDecompression started..." << endl;
		tree->Decompress(inputSize, showProgress, static_cast<TokenOutputStream*>(tokenStream));
		cout << endl << "\tDecompression finished..." << endl;
		cout << "\t---------------------------------------------" << endl;
		endTime = chrono::high_resolution_clock::now();

		cout << "Decompression was successful. Decompressed file is here: " << outputFile << endl;

		tree->WriteDecompressionStatistics();
	}

	delete (tree);

	auto time = endTime - starTime;
	double timeS = (double)chrono::duration_cast<chrono::microseconds>(time).count() / 1000000;

	cout << fixed << setprecision(3) << "Time elapsed during compression: " << timeS << " ms.\n";
	cout << "(De)Compression speed: " << (double)(inputSize / 1000) / timeS << " Kb/s" << endl;

	//tree->WriteTree();

	return 0;
}