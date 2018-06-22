#pragma once

#include "VertexBase.h"

/**
 * <summary> Class that represents leaf.</summary>
 */

class Leaf : public VertexBase
{
	public:
		Leaf();
		~Leaf();

		/**
		 * <summary> Writes a vertex.</summary>
		 *
		 * <param name="level">		 The level of the leaf.</param>
		 * <param name="labelStart"> The label start.</param>
		 * <param name="labelEnd">   The label end.</param>
		 * <param name="buffer">	 The buffer.</param>
		 */

		//void WriteVertex(int level, int labelStart, int *labelEnd, Buffer* buffer);

		/**
		 * <summary> Updates the offset - gets current offset from leaf</summary>
		 *
		 * <returns> The leaf offset.</returns>
		 */

		int UpdateOffset();
};