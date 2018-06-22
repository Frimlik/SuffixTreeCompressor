#pragma once
#include <string>
#include <map>

#include "Buffer.hh"

using namespace std;

/**
 * <summary> Base class for all vertices in graph.</summary>
 */

class VertexBase
{
	public:
		VertexBase * parent;		///< The parent
		VertexBase * nextSibling;   ///< The next sibling
		int offsetInBuffer;			///< Offset of string that is represented by this vertex in buffer (i.e. currently I am on position that is counted as offset + depth in three (depthInSymbols))

		VertexBase() = default;
		virtual ~VertexBase() = default;
		//virtual void WriteVertex(int level, int labelStart, int labelEnd, Buffer* buffer);

		/**
		 * <summary> Updates the vertex offset.</summary>
		 *
		 * <returns> Offset of the string that this vertex represents.</returns>
		 */

		virtual int UpdateOffset() = 0;
};