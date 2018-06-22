#pragma once

#include <string>
#include <chrono>
#include "ActivePoint.h"
#include "Vertex.hh"
#include "Leaf.h"
#include "Buffer.hh"
#include "MatchHelper.hh"
#include "Stream.hh"
#include "Encoder.h"
#include "Decoder.h"
#include "OutputBytesHelper.hh"
#include "SuffixTreeAux.hh"
#include "TokenInputStream.h"
#include "TokenOutputStream.h"

using namespace std;

/**
 * <summary> The suffix tree itself.</summary>
 */

 //Declaration----------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T> 
class SuffixTree : public SuffixTreeAux
{
	public:
		SuffixTree(int windowSize, short slidingWindowSizeBits, short matchLengthSizeBits);
		~SuffixTree();

		/**
		 * <summary> Initializes for compression.</summary>
		 *
		 * <param name="inFile">			  The input file.</param>
		 * <param name="outFile">			  The output file.</param>
		 * <param name="matchLengthSizeBits"> The number of bits that are occupied by match length.</param>
		 * <param name="binaryFile">		  Compressing binary file.</param>
		 */

		void InitForCompression(const char * inFile, const char * outFile, short matchLengthSizeBits, bool binaryFile);

		/**
		 * <summary> Initializes for decompression.</summary>
		 *
		 * <param name="inFile">			  The input file.</param>
		 * <param name="outFile">			  The output file.</param>
		 * <param name="matchLengthSizeBits"> The number of bits that are occupied by match length.</param>
		 */

		void InitForDecompression(const char * inFile, const char * outFile, short matchLengthSizeBits, bool binaryFile);

		/**
		 * <summary> Finishes a compression.</summary>
		 */

		void FinishCompression();

		/**
		 * <summary> Finishes a decompression.</summary>
		 */

		void FinishDecompression();

		/**
		 * <summary> Do the compression.</summary>
		 *
		 * <param name="inputFileSize"> Size of the input file.</param>
		 * <param name="showProgress">  True to show, false to hide the progress.</param>
		 */

		void Compress(long int inputFileSize, bool showProgress, TokenInputStream * inputStream);

		/**
		 * <summary> Do the decompression.</summary>
		 *
		 * <param name="inputFileSize"> Size of the input file.</param>
		 * <param name="showProgress">  True to show, false to hide the progress.</param>
		 */

		void Decompress(long int inputFileSize, bool showProgress, TokenOutputStream* outputStream);

		/**
		 * <summary> Writes the suffix tree.</summary>
		 */

		void WriteTree();

		/**
		 * <summary> Writes the compression statistics.</summary>
		 */

		void WriteCompressionStatistics();

		/**
		 * <summary> Writes the decompression statistics.</summary>
		 */

		void WriteDecompressionStatistics();

	private:
		Match match;				///< Global match that is sent to MatchHelper to get matches
		short matchBytesCount;		///< Number of bytes per one match
		uchar * matchBytes;			///< Global match bytes that are sent to MatchHelper to save bytes for current match
		T * shortMatch;				///< Global short match bytes (symbols) that are sent to MatchHelper; used when the match is too short
		Vertex<T> * root;			///< The root of the suffix tree
		VertexChild * bot;			///< The bot of the suffix tree
		Vertex<T> * lastVertexWithoutSuffixLinkEnd;	///< The last vertex that has no suffix link - there is no vertex at the end of the link
		ActivePoint activePoint;	///< The active point, that moves through graph
		Buffer<T> * buffer;			///< The buffer for underlying string
		Leaf * leaves;				///< The suffix tree leaves
		int windowSize;				///< Size of the sliding window
		int newLeafIndex;			///< The index of the new leaf
		int oldestLeafIndex;		///< The oldest leaf index - for oldest leaf removal
		MatchHelper<T> matchHelper;	///< The match helper - for handling matches
		InStream<T> inStream;		///< Stream to read data from
		OutStream<T> outStream;		///< Stream to write data to
		Encoder encoder;			///< The encoder for matches
		Decoder decoder;			///< The decoder
		OutputBytesHelper<T> * outputBytesHelper;  ///< The helper for collecting output bytes

		bool matchAfterLongestSufixRemoval;		///< Whether there is a match that stayed after longest suffix removal
		int symbolsProcessed = 0;
		
		/**
		* <summary> Creates empty graph.</summary>
		*/

		void CreateEmptyGraph();

		/**
		* <summary> Creates bot and root.</summary>
		*/

		void CreateBotRoot();

		/**
		* <summary> Appends a symbol to graph.</summary>
		*
		* <param name="symbol"> The symbol.</param>
		*/

		void AppendSymbol(T symbol);

		/**
		* <summary> Gets oldest leaf index.</summary>
		*
		* <returns> The oldest leaf index.</returns>
		*/

		int GetOldestLeafIndex();

		/**
		 * <summary> Move active point to root.</summary>
		 */

		void MoveActivePointToRoot();

		/**
		 * <summary> Move active point down with the given symbol.</summary>
		 *
		 * <param name="symbol"> The symbol.</param>
		 */

		void MoveActivePointDown(T symbol);

		/**
		 * <summary> Check if it is possible to move active point down and moves it if yes.</summary>
		 *
		 * <param name="symbol"> The symbol.</param>
		 *
		 * <returns> True if it succeeds, false if it fails.</returns>
		 */

		bool MoveActivePointDownIfPossible(T symbol);

		/**
		 * <summary> Gets new leaf.</summary>
		 *
		 * <param name="parent"> Parent of the new leaf.</param>
		 *
		 * <returns> Null if it fails, else the new leaf.</returns>
		 */

		Leaf * GetNewLeaf(VertexBase * parent);

		/**
		 * <summary> Move active point sideways through suffix link.</summary>
		 */

		void MoveActivePointSideways();

		/**
		 * <summary> Move the tree forward - append symbol to buffer.</summary>
		 *
		 * <param name="symbol"> The symbol.</param>
		 */

		void MoveForward(T symbol);

		/**
		 * <summary> Canonize active point - jump down if it is possible - if ap length is bigger that edge length.</summary>
		 */

		void CanonizeAP();

		/**
		 * <summary> Removes the oldest symbol.</summary>
		 */

		void RemoveOldestSymbol();

		/**
		 * <summary> Writes the current match.</summary>
		 */

		void WriteMatch();

		/**
		 * <summary> Changes match position to new offset.</summary>
		 *
		 * <param name="child">  The child vertex - the remaining child after oldest leaf removal.</param>
		 * <param name="parent"> The parent of the removed and remaining child.</param>
		 */

		void ChangeMatchPositionToNew(VertexBase * child, VertexBase * parent);

		/**
		 * <summary> Updates the offsets.</summary>
		 */

		void UpdateOffsets();
		
		/**
		* <summary> Searches for the end of edge, that starts with given symbol.</summary>
		*
		* <param name="symbol"> The symbol.</param>
		* <param name="buffer"> Buffer object, to be able to search in symbol array.</param>
		*
		* <returns> Vertex that is at the end of edge.</returns>
		*/

		//VertexBase * FindOutEdgeEnd(int symbol);
};

//Definition----------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
SuffixTree<T>::SuffixTree(int windowSize, short slidingWindowSizeBits, short matchLengthSizeBits)
{
	this->windowSize = windowSize;
	this->buffer = new Buffer<T>(slidingWindowSizeBits);
	this->matchBytesCount = ceil((double)(slidingWindowSizeBits + matchLengthSizeBits) / 8);
	this->matchAfterLongestSufixRemoval = false;
}

template <typename T>
SuffixTree<T>::~SuffixTree()
{
	delete (buffer);
}

template<typename T>
void SuffixTree<T>::InitForCompression(const char * inFile, const char * outFile, short matchLengthSizeBits, bool binaryFile)
{
	this->inStream.Open(inFile, true, binaryFile);
	this->outStream.Open(outFile, true);
	this->outputBytesHelper = new OutputBytesHelper<T>(outFile, matchBytesCount, &outStream);

	this->encoder = Encoder(matchLengthSizeBits, matchBytesCount);
	this->shortMatch = new T[matchBytesCount];
	this->matchHelper = MatchHelper<T>(matchLengthSizeBits, shortMatch, matchBytesCount);
	this->matchBytes = new uchar[matchBytesCount];

	CreateEmptyGraph();
}

template <typename T>
void SuffixTree<T>::InitForDecompression(const char * inFile, const char * outFile, short matchLengthSizeBits, bool binaryFile)
{
	this->inStream.Open(inFile, false);
	this->outStream.Open(outFile, false, binaryFile);

	decoder = Decoder(matchLengthSizeBits, matchBytesCount);
	this->matchBytes = new uchar[matchBytesCount];
}

template <typename T>
void SuffixTree<T>::FinishCompression()
{
	root->DeleteChildren();

	delete[] leaves;
	delete[] matchBytes;
	delete[] shortMatch;

	delete(root);
	delete(bot);
}

template <typename T>
void SuffixTree<T>::FinishDecompression()
{
	delete[] matchBytes;
}

template <typename T>
void SuffixTree<T>::Compress(long int inputFileSize, bool showProgress, TokenInputStream * inputStream)
{
	double donePart, currentlyDonePart;
	donePart = 0;
	short dashesCount = 0;
	cout.precision(2);
	
	T tokenId;
	while (!inputStream->FileEnd() && (tokenId = inputStream->ReadToken()) != -1)
	{
		//check status every 64th run
		if (showProgress && (symbolsProcessed & 0x3f) == 0)
		{
			double currentlyDonePart = (double)symbolsProcessed / inputFileSize;
			if (currentlyDonePart - donePart > 0.04)
			{
				donePart = currentlyDonePart;
				dashesCount = donePart / 0.04;
				cout << "\r\t";
				for (short i = 0; i < dashesCount; i++)
				{
					cout << "-";
				}
				cout << fixed << " " << donePart * 100 << " %";
			}
		}

		AppendSymbol(tokenId);
		symbolsProcessed++;
	}

	if (showProgress)
	{
		cout << "\r\t" << "-";
		for (short i = 1; i < dashesCount; i++)
		{
			cout << "-";
		}
		cout << " " << 100.0 << " %";
	}

	//if there exist a match at the very end of string
	if (matchHelper.GetMatchLength() > 0)
	{
		WriteMatch();
	}

	encoder.EncodeMatch(matchHelper.GetMatchWithZeroLength(outputBytesHelper->GetRemainingFlagsCount()), matchBytes);
	outputBytesHelper->FinishWork(matchBytes);

	inStream.Close();
	outStream.Close();
}

template <typename T>
void SuffixTree<T>::Decompress(long int inputFileSize, bool showProgress, TokenOutputStream* outputStream)
{
	uchar flagsByte;
	T decodedSymbol;
	uchar byte;
	T symbolForOutput;
	bool finish = false;
	short bitsInByteCount = 8;

	flagsByte = inStream.ReadByte();

	chrono::time_point<chrono::steady_clock> startTime = chrono::high_resolution_clock::now();
	chrono::time_point<chrono::steady_clock> currentTime;
	long long elapsedTime, totalElapsedTime;
	elapsedTime = totalElapsedTime = 0;

	//repeat until EOF
	while (true)
	{
		for (short i = 0; i < bitsInByteCount; i++)
		{
			if (showProgress)
			{
				currentTime = chrono::high_resolution_clock::now();
				elapsedTime = chrono::duration_cast<chrono::microseconds>(currentTime - startTime).count() / 1000;
				if (elapsedTime > 500)
				{
					totalElapsedTime += elapsedTime;
					startTime = currentTime;
					double donePart = (double)inStream.GetBytesRead() / inputFileSize;
					int dashesCount = donePart / 0.05;
					cout << "\r\t";
					for (short i = 0; i < dashesCount; i++)
					{
						cout << "-";
					}
					cout << fixed << setprecision(2) << " " << donePart * 100 << " % [" << setprecision(3) << (double)totalElapsedTime / 1000 << " s]";
				}
			}

			DecodeResultEnum result = decoder.DecodeBit(flagsByte, i);

			if (result == DecodeResultEnum::LiteralResult)
			{
				decodedSymbol = 0;
				decodedSymbol = inStream.ReadSymbolBin();

				buffer->AppendSymbolToSlidingWindow(decodedSymbol);

				if (buffer->SlidingWindowIsEmptyWhileDecoding())
				{
					symbolForOutput = buffer->GetLastSymbol();
					outputStream->WriteToken(symbolForOutput);
				}
			}
			else
			{
				for (short j = 0; j < matchBytesCount; j++)
				{
					matchBytes[j] = inStream.ReadByte();
				}

				Match match = decoder.DecodeMatch(matchBytes);

				//signal, that very last part is processed
				if (match.MatchLength == 0)
				{
					finish = true;
					bitsInByteCount = match.MatchIndex + 1;
					continue;
				}

				buffer->SetMatchIndex(match.MatchIndex);
				for (int j = 0; j < match.MatchLength; j++)
				{
					buffer->AppendMatchSymbol();

					if (buffer->SlidingWindowIsEmptyWhileDecoding())
					{
						symbolForOutput = buffer->GetLastSymbol();
						outputStream->WriteToken(symbolForOutput);
					}
				}
			}
		}

		if (finish)
			break;

		flagsByte = inStream.ReadByte();
	}

	if (showProgress)
	{
		totalElapsedTime = chrono::duration_cast<chrono::microseconds>(currentTime - startTime).count() / 1000;
		cout << "\r\t" << "-";
		for (short i = 1; i < 20; i++)
		{
			cout << "-";
		}
		cout << fixed << setprecision(2) << " " << 100.0 << " % [" << (double)totalElapsedTime / 1000 << " s]";
	}

	if (!buffer->SlidingWindowIsEmptyWhileDecoding())
	{
		do
		{
			decodedSymbol = buffer->GetLastSymbol();
			outputStream->WriteToken(decodedSymbol);
		} while (!buffer->SlidingWindowIsEmptyWhileDecoding());
	}

	inStream.Close();
	outStream.Close();
}

template <typename T>
void SuffixTree<T>::WriteTree()
{
	Vertex * vertex = this->root;

	int difference = abs(buffer->GetSlidingWindowFront() - buffer->GetSlidingWindowBack());
	int front = this->buffer->GetSlidingWindowFront();

	vertex->WriteVertex(0, &front, this->buffer->GetSlidingWindowBack(), 0, this->buffer);

	cout << "\n----------------------" << endl;
}

template <typename T>
void SuffixTree<T>::WriteCompressionStatistics()
{
	cout << "======================================================================================" << endl;
	cout << "Bytes written / read: " << outStream.GetBytesWritten() << " / " << inStream.GetBytesRead() << endl;
	cout << "Compression ratio: " << (double)outStream.GetBytesWritten() / inStream.GetBytesRead() * 100 << " (" << (double)(outStream.GetBytesWritten() * 8) / inStream.GetBytesRead() << " bpB)" << endl;
	cout << "======================================================================================" << endl;
}

template <typename T>
void SuffixTree<T>::WriteDecompressionStatistics()
{
	cout << "======================================================================================" << endl;
	cout << "Bytes read: " << inStream.GetBytesRead() << endl;
	cout << "Bytes written: " << outStream.GetBytesWritten() << endl;
	cout << "======================================================================================" << endl;
}

template <typename T>
void SuffixTree<T>::AppendSymbol(T symbol)
{
	MoveActivePointDown(symbol);
	MoveForward(symbol);
	//WriteTree();

	//if buffer is full, remove oldest symbol - free position for new symbol, occasionally update offsets
	if (this->buffer->SlidingWindowIsFull())
	{
		RemoveOldestSymbol();

		if (this->buffer->FrontIsOnEdge())
		{
			UpdateOffsets();
		}
	}
}

template <typename T>
void SuffixTree<T>::CreateEmptyGraph()
{
	this->leaves = new Leaf[this->windowSize];
	this->newLeafIndex = 0;
	this->oldestLeafIndex = 0;
	this->lastVertexWithoutSuffixLinkEnd = NULL;

	this->CreateBotRoot();
	this->root->SetSuffixLink(this->bot);
	MoveActivePointToRoot();
}

template <typename T>
void SuffixTree<T>::CreateBotRoot()
{
	this->root = new Vertex<T>(0);
	this->bot = new VertexChild();
}

template <typename T>
int SuffixTree<T>::GetOldestLeafIndex()
{
	return this->oldestLeafIndex;
}

template <typename T>
void SuffixTree<T>::MoveActivePointToRoot()
{
	this->activePoint.start = this->root;
	this->activePoint.end = NULL;
}

template <typename T>
void SuffixTree<T>::MoveActivePointDown(T symbol)
{
	//Repeat until successful move down
	while (!MoveActivePointDownIfPossible(symbol))
	{
		if (this->activePoint.length == 0)
		{
			//In node
			Leaf * newLeaf = this->GetNewLeaf(activePoint.start);
			static_cast<Vertex<T>*>(activePoint.start)->AddChild(newLeaf);

			MoveActivePointSideways();
		}
		else
		{
			//On edge
			//Create new edge between AP start and AP end
			Vertex<T> * newNode = new Vertex<T>(this->activePoint.end->offsetInBuffer);
			Vertex<T> * apStart = static_cast<Vertex<T>*>(this->activePoint.start);
			newNode->parent = apStart;
			newNode->depthInTree = apStart->depthInTree + this->activePoint.length;
			newNode->SetSuffixLink(NULL);
			//Change apStart's child (end for newNode)
			apStart->ReplaceChild(activePoint.end, newNode);
			newNode->AddChild(activePoint.end);
			//Set child's parent
			this->activePoint.end->parent = newNode;

			Leaf * newLeaf = this->GetNewLeaf(newNode);
			newNode->AddChild(newLeaf);

			if (lastVertexWithoutSuffixLinkEnd != NULL)
			{
				this->lastVertexWithoutSuffixLinkEnd->SetSuffixLink(newNode);
				this->lastVertexWithoutSuffixLinkEnd = NULL;
			}

			MoveActivePointSideways();
			//move sideways and canonize
			//active node will move, but the newnode will not move

			if (this->activePoint.length > 0)
			{
				this->lastVertexWithoutSuffixLinkEnd = newNode;
			}
			else
			{
				newNode->SetSuffixLink(this->activePoint.start);
			}
		}
	}
}

template <typename T>
bool SuffixTree<T>::MoveActivePointDownIfPossible(T symbol)
{
	//In BOT - only move to root
	if (this->activePoint.start == this->bot)
	{
		this->activePoint.start = this->root;
		this->activePoint.end = NULL;
		++this->activePoint.offsetOnBuffer %= 2 * windowSize;

		if (activePoint.length > 0)
		{
			this->activePoint.length--;
		}

		outputBytesHelper->AppendSymbol(symbol);

		return true;
	}

	if (this->activePoint.length == 0)
	{
		//In node
		Vertex<T>* apStart = static_cast<Vertex<T>*>(activePoint.start);

		VertexBase* child = apStart->firstChild;
		VertexBase* prevChild = NULL;
		int position, positionInBuffer;
		T sym;

		while (child != NULL)
		{
			position = apStart->depthInTree + child->offsetInBuffer;
			sym = buffer->GetSymbolFromBuffer(position);

			if (sym == symbol)
			{
				if (matchAfterLongestSufixRemoval && matchHelper.GetMatchLength() > 0)
				{
					WriteMatch();
					matchAfterLongestSufixRemoval = false;
				}

				positionInBuffer = position;
				position = child->offsetInBuffer + apStart->depthInTree - buffer->GetSlidingWindowFront();

				if (position < 0) position += 2 * windowSize;
				if (positionInBuffer >= 2 * windowSize) positionInBuffer -= 2 * windowSize;
				//when branching, match position may change (all occurences of right branching factor are elsewhere in string -> it is needed to update it
				if (matchHelper.GetMatchLength() > 0)
				{
					matchHelper.MatchPosition = position;
					matchHelper.MatchBufPosition = positionInBuffer - matchHelper.GetMatchLength();
				}

				if (!matchHelper.MoveMatchForward(position, positionInBuffer, symbol))
				{
					WriteMatch();
				}

				this->activePoint.end = child;
				this->activePoint.length++;

				if (prevChild != NULL) apStart->MoveToFront(prevChild, child);

				return true;
			}

			prevChild = child;
			child = child->nextSibling;
		}
	}
	else
	{
		Vertex<T>* apStart = static_cast<Vertex<T>*>(activePoint.start);
		//On edge
		T sym = this->buffer->GetSymbolFromBuffer(
			apStart->depthInTree +
			this->activePoint.end->offsetInBuffer +
			this->activePoint.length);

		if (sym == symbol)
		{
			int position = apStart->depthInTree + activePoint.end->offsetInBuffer + activePoint.length - buffer->GetSlidingWindowFront();
			int positionInBuffer = apStart->depthInTree + activePoint.end->offsetInBuffer + activePoint.length;
			if (position < 0) position += 2 * windowSize;
			if (positionInBuffer >= 2 * windowSize) positionInBuffer -= 2 * windowSize;

			if (!matchHelper.MoveMatchForward(position, positionInBuffer, symbol))
			{
				WriteMatch();
			}

			this->activePoint.length++;
			return true;
		}
	}

	//suitable edge was not found -> write out the match if there is any
	if (matchHelper.GetMatchLength() > 0)
	{
		WriteMatch();
	}

	return false;
}

template <typename T>
Leaf * SuffixTree<T>::GetNewLeaf(VertexBase * parent)
{
	Leaf * newLeaf = &(this->leaves[newLeafIndex]);
	++newLeafIndex %= this->windowSize;
	Vertex<T> * apStart = static_cast<Vertex<T>*>(this->activePoint.start);
	newLeaf->parent = parent;
	newLeaf->nextSibling = NULL;

	int offset = this->buffer->GetSlidingWindowFront() - (apStart->depthInTree + this->activePoint.length);
	if (offset < 0)					offset += 2 * windowSize;

	newLeaf->offsetInBuffer = offset;
	return newLeaf;
}

template <typename T>
void SuffixTree<T>::MoveActivePointSideways()
{
	Vertex<T>* apStart = static_cast<Vertex<T>*>(this->activePoint.start);
	this->activePoint.start = apStart->GetSuffixLink();

	CanonizeAP();
}

template <typename T>
void SuffixTree<T>::MoveForward(T symbol)
{
	this->buffer->AppendSymbolToSlidingWindow(symbol);

	CanonizeAP();
}

template <typename T>
void SuffixTree<T>::CanonizeAP()
{
	if (activePoint.length > 0)
	{
		Vertex<T>* apStart = dynamic_cast<Vertex<T>*>(this->activePoint.start);
		this->activePoint.end = (apStart) ? apStart->FindOutEdgeEnd(this->buffer->GetSymbolFromBuffer(this->activePoint.offsetOnBuffer), this->buffer) : this->root;
		Vertex<T>* apEnd = dynamic_cast<Vertex<T>*>(this->activePoint.end);

		int endDepth = (apEnd) ? apEnd->depthInTree : this->windowSize;
		int edgeLength = (apStart) ? (apEnd) ? endDepth - apStart->depthInTree : this->windowSize : 1;

		while (this->activePoint.length >= edgeLength)
		{
			this->activePoint.start = static_cast<VertexChild*>(this->activePoint.end);
			this->activePoint.offsetOnBuffer += edgeLength; this->activePoint.offsetOnBuffer %= 2 * windowSize;
			this->activePoint.length -= edgeLength;

			if (this->activePoint.length == 0)
				return;

			apStart = static_cast<Vertex<T>*>(activePoint.start);
			int symbol = this->buffer->GetSymbolFromBuffer(activePoint.offsetOnBuffer);
			activePoint.end = apStart->FindOutEdgeEnd(this->buffer->GetSymbolFromBuffer(this->activePoint.offsetOnBuffer), this->buffer);
			apEnd = dynamic_cast<Vertex<T>*>(activePoint.end);
			edgeLength = (apEnd) ? apEnd->depthInTree - apStart->depthInTree : this->windowSize;
		}
	}
}

template <typename T>
void SuffixTree<T>::RemoveOldestSymbol()
{
	Leaf * oldestLeaf = &(this->leaves[oldestLeafIndex++]);
	oldestLeafIndex %= this->windowSize;
	Vertex<T> * apStart = static_cast<Vertex<T>*>(this->activePoint.start);

	if (this->activePoint.end == oldestLeaf)
	{
		Leaf * newLeaf = GetNewLeaf(apStart);
		apStart->ReplaceChild(oldestLeaf, newLeaf);

		if (matchHelper.GetMatchLength() == 1 && this->activePoint.start == this->root)
		{
			WriteMatch();
		}
		else
		{
			matchAfterLongestSufixRemoval = true;
		}

		VertexBase * parent = apStart;

		MoveActivePointSideways();
	}
	else
	{
		Vertex<T> * parent = static_cast<Vertex<T>*>(oldestLeaf->parent);
		parent->RemoveChild(oldestLeaf);

		if (parent != this->root)
		{
			VertexBase * _parentOfParent = parent->parent;

			//if the first child is now the only child
			if (parent->firstChild->nextSibling == NULL)
			{
				Vertex<T> * parentOfParent = static_cast<Vertex<T>*>(_parentOfParent);
				VertexBase * child = parent->firstChild;

				//if active point is at parent - start must move to parent of parent
				if (parent == this->activePoint.start)
				{
					if (activePoint.start == activePoint.end)
					{
						if (matchAfterLongestSufixRemoval)
						{
							WriteMatch();
							matchAfterLongestSufixRemoval = false;
						}

						ChangeMatchPositionToNew(child, parent);
					}

					int edgeLength = parent->depthInTree - parentOfParent->depthInTree;

					activePoint.start = parentOfParent;
					activePoint.length += edgeLength;
					activePoint.offsetOnBuffer = (activePoint.offsetOnBuffer - edgeLength >= 0) ? activePoint.offsetOnBuffer - edgeLength : activePoint.offsetOnBuffer - edgeLength + 2 * windowSize;
				}
				//if active point ends at parent - end must move to child
				else if (parent == this->activePoint.end)
				{
					if (matchAfterLongestSufixRemoval)
					{
						WriteMatch();
						matchAfterLongestSufixRemoval = false;
					}

					ChangeMatchPositionToNew(child, parent);
					this->activePoint.end = child;
				}

				parentOfParent->AddChild(child);
				parentOfParent->RemoveChild(parent);
				child->parent = parentOfParent;

				delete(parent);

				CanonizeAP();
			}
		}
	}

	buffer->MoveBackForward();
}

template <typename T>
void SuffixTree<T>::WriteMatch()
{
	short matchSingleCharsCount = matchHelper.GetMatch(match);

	if (matchSingleCharsCount == -1)
	{
		encoder.EncodeMatch(match, matchBytes);

		outputBytesHelper->AppendMatch(matchBytes);
	}
	else
	{
		for (short i = 0; i < matchSingleCharsCount; i++)
		{
			outputBytesHelper->AppendSymbol(shortMatch[i]);
		}
	}
}

template <typename T>
void SuffixTree<T>::ChangeMatchPositionToNew(VertexBase * child, VertexBase * parent)
{
	//update match position only if the parent (i.e. the vertex from which match position is enduced) and the oldest leaf have the same offset
	if (matchHelper.GetMatchLength() > 0)// && activePoint.end->offsetOnBuffer == oldestLeaf->offsetOnBuffer)
	{
		int offsetDifference = child->offsetInBuffer - parent->offsetInBuffer;
		if (offsetDifference < 0) offsetDifference += 2 * windowSize;

		matchHelper.MatchPosition += offsetDifference;
		matchHelper.MatchBufPosition += offsetDifference;
		if (matchHelper.MatchPosition > 2 * windowSize) matchHelper.MatchPosition -= 2 * windowSize;
		if (matchHelper.MatchBufPosition > 2 * windowSize) matchHelper.MatchBufPosition -= 2 * windowSize;
	}
}

template <typename T>
void SuffixTree<T>::UpdateOffsets()
{
	this->root->UpdateOffset();

	if (activePoint.end != NULL)
	{
		//I need refreshed offset + depth of vertex (impl/expl), where the match started - need to get current position of AP (vertex offset+depth, AP length) and substract offset length
		int newMatchBufPosition = activePoint.end->offsetInBuffer + static_cast<Vertex<T>*>(activePoint.start)->depthInTree + activePoint.length - matchHelper.GetMatchLength();

		//and if this value is different from 
		if (matchHelper.GetMatchLength() > 0)
		{
			//if match is nonzero and is longer than is current depth of active point, write out (because the offset would change)
			if (matchHelper.GetMatchLength() > static_cast<Vertex<T>*>(activePoint.start)->depthInTree + activePoint.length)
			{
				WriteMatch();
				matchAfterLongestSufixRemoval = false;
			}
			else if (newMatchBufPosition != matchHelper.MatchBufPosition)
			{
				int offsetDiff = newMatchBufPosition - matchHelper.MatchBufPosition;
				if (offsetDiff < 0) offsetDiff += 2 * windowSize;

				matchHelper.MatchPosition += offsetDiff;
				if (matchHelper.MatchPosition >= 2 * windowSize) matchHelper.MatchPosition -= 2 * windowSize;
				matchHelper.MatchBufPosition = activePoint.end->offsetInBuffer;
			}
		}
	}
}