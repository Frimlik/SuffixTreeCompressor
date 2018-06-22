#pragma once

/**
 * <summary> A class to encapsulate handling with buffer itself.</summary>
 */

typedef unsigned char uchar;

//Declaration----------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class Buffer
{
	public:
		Buffer() = default;
		Buffer(short windowSizeBits);
		~Buffer();

		/**
		 * <summary> Gets the sliding window front.</summary>
		 *
		 * <returns> The front.</returns>
		 */

		int GetSlidingWindowFront();

		/**
		 * <summary> Gets the sliding window back.</summary>
		 * 
		 * <returns> The back.</returns>
		 */

		int GetSlidingWindowBack();

		/**
		 * <summary> Gets the sliding window length.</summary>
		 *
		 * <returns> The length.</returns>
		 */

		int GetSlidingWindowLength();

		/**
		 * <summary> Check if the sliding window is full (distance between front and end is wider than this->bufferLength).</summary>
		 *
		 * <returns> True if full, false if not.</returns>
		 */

		bool SlidingWindowIsFull();

		/**
		 * <summary> Check if the sliding window is empty while decoding.</summary>
		 *
		 * <returns> True if the decoded is empty, false if not.</returns>
		 */

		bool SlidingWindowIsEmptyWhileDecoding();

		/**
		 * <summary> Gets symbol from buffer.</summary>
		 *
		 * <param name="index"> Index of symbol to be returned.</param>
		 *
		 * <returns> The symbol at given index in buffer.</returns>
		 */

		T GetSymbolFromBuffer(int index);

		/**
		 * <summary> Appends a symbol and moves front index.</summary>
		 *
		 * <param name="symbol"> The symbol.</param>
		 */

		void AppendSymbolToSlidingWindow(T symbol);
		
		/**
		 * <summary> Check if 'off1' is higher than 'off2'.</summary>
		 *
		 * <param name="off1"> The first offset.</param>
		 * <param name="off2"> The second offset.</param>
		 *
		 * <returns> True if offset1 is higher, false if not.</returns>
		 */

		bool IsOffsetHigher(int off1, int off2);

		/**
		 * <summary> Gets the last symbol while decoding.</summary>
		 *
		 * <returns> The last symbol.</returns>
		 */

		T GetLastSymbol();

		/**
		 * <summary> Appends the match symbol into sliding window.</summary>
		 */

		void AppendMatchSymbol();

		/**
		 * <summary> Sets index, where match starts - for no more need to ask for it.</summary>
		 *
		 * <param name="matchIndex"> Index of previous location of current symbol.</param>
		 */

		void SetMatchIndex(int matchIndex);

		/**
		 * <summary> Move back forward.</summary>
		 */

		void MoveBackForward();

		/**
		* <summary> Check if front is on the edge - at index 0 or in the middle of buffer.</summary>
		*
		* <returns> True if yes, false if not.</returns>
		*/

		bool FrontIsOnEdge();

	private:
		T * buffer;				///< The buffer itself
		int front;					///< The front of sliding window - index of next symbol placing
		int back;					///< The back of sliding window - index of next symbol deleting
		short bufferSizeBits;		///< The size of sliding window in bits
		int slidingWindowLength;	///< Length of the sliding window
		int bufferLength;			///< The length of the buffer
		int bufferHalfLength;		///< Half length of the buffer (used for checking if front is on updating point)
		int matchIndex;				///< The index in buffer that shows where matched symbols are located; valid while appending match symbols to buffer during decoding
};

//Definition----------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T> 
Buffer<T>::Buffer(short windowSizeBits)
{
	this->front = 0;
	this->back = 0;
	this->slidingWindowLength = 1 << windowSizeBits;
	this->bufferSizeBits = windowSizeBits + 1;
	this->bufferHalfLength = slidingWindowLength;
	this->buffer = new T[1 << bufferSizeBits];
}

template <typename T> 
Buffer<T>::~Buffer()
{
	delete[] buffer;
}

template <typename T> 
int Buffer<T>::GetSlidingWindowFront()
{
	return this->front;
}

template <typename T> 
int Buffer<T>::GetSlidingWindowBack()
{
	return this->back;
}

template <typename T> 
int Buffer<T>::GetSlidingWindowLength()
{
	return this->slidingWindowLength;
}

template <typename T> 
bool Buffer<T>::SlidingWindowIsFull()
{
	return abs(this->front - this->back) >= this->slidingWindowLength;
}

template <typename T> 
bool Buffer<T>::SlidingWindowIsEmptyWhileDecoding()
{
	return this->back == this->front;
}

template <typename T> 
T Buffer<T>::GetSymbolFromBuffer(int index)
{
	return this->buffer[index & ((1 << bufferSizeBits) - 1)];
}

template <typename T> 
void Buffer<T>::AppendSymbolToSlidingWindow(T symbol)
{
	this->buffer[this->front] = symbol;

	this->front++;
	this->front &= ((1 << bufferSizeBits) - 1);
}

template <typename T> 
bool Buffer<T>::IsOffsetHigher(int off1, int off2)
{
	if (front > back && off2 > off1)
	{
		return true;
	}

	if (back > front)
	{
		if (off2 < front && off1 < off2 ||
			off1 > back && off2 > off1 ||
			off2 < front && off1 > back)
		{
			return true;
		}
	}

	return false;
}

template <typename T> 
T Buffer<T>::GetLastSymbol()
{
	T symbol = this->buffer[this->back];

	this->back++;
	this->back &= ((1 << bufferSizeBits) - 1);

	return symbol;
}

template <typename T> 
void Buffer<T>::AppendMatchSymbol()
{
	AppendSymbolToSlidingWindow(this->buffer[(this->front + matchIndex) & ((1 << bufferSizeBits) - 1)]);
}

template <typename T> 
void Buffer<T>::SetMatchIndex(int matchIndex)
{
	this->matchIndex = matchIndex;
}

template <typename T> 
void Buffer<T>::MoveBackForward()
{
	this->back++;
	this->back &= ((1 << bufferSizeBits) - 1);
}

template <typename T>
bool Buffer<T>::FrontIsOnEdge()
{
	return this->front == 0 || this->front == this->bufferHalfLength;
}