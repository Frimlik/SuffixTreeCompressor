#pragma once
#include "Vertex.hh"

/**
 * <summary> An active point that moves through graph.</summary>
 */

class ActivePoint
{
	public:
		ActivePoint();
		~ActivePoint();
		VertexChild * start;	///< The AP starting vertex (bot/root/inner node)
		VertexBase * end;		///< The AP terminal vertex	(inner node, leaf)
		int length;				///< The length of AP - how far it is from starting vertex
		int offsetOnBuffer;		///< The offset on buffer
};