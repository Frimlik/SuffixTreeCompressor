#pragma once
#include "VertexBase.h"

/**
 * <summary> Class that represents vertex with child(ren).</summary>
 */

class VertexChild :	public VertexBase
{
	public:
		VertexBase * firstChild;	///< The first child of vertex

		VertexChild() = default;
		~VertexChild() = default;

		/**
		* <summary> Updates the offset of the current vertex.</summary>
		*
		* <returns> The freshest offset of all leaves in subtree.</returns>
		*/

		int UpdateOffset();

		/**
		* <summary> Move the movedChild to front of children list.</summary>
		*
		* <param name="prevChild"> Child previous to the moved child.</param>
		* <param name="movedChild"> The child to be moved to the front.</param>
		*/

		void MoveToFront(VertexBase* prevChild, VertexBase* movedChild);

		/**
		* <summary> Adds a child into children list.</summary>
		*
		* <param name="child"> The child to be added.</param>
		*/

		void AddChild(VertexBase* child);

		/**
		* <summary> Replace child in children list.</summary>
		*
		* <param name="oldVert"> Vertex to be changed.</param>
		* <param name="newVert"> New vertex.</param>
		*/

		void ReplaceChild(VertexBase * oldVert, VertexBase * newVert);

		/**
		* <summary> Removes the child given.</summary>
		*
		* <param name="vertexToRemove"> The vertex to be removed.</param>
		*/

		void RemoveChild(VertexBase * vertexToRemove);
	private:

		/**
		* <summary> Search previous sibling of child in list - if it is the first, then return NULL.</summary>
		*
		* <param name="vertex"> Vertex that I search for previous sibling of.</param>
		*
		* <returns> Null if it is self, else the previous sibling of child.</returns>
		*/

		VertexBase * GetPrevSiblingOfChild(VertexBase * vertex);
};