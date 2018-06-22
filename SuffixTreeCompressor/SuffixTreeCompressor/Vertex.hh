#pragma once
#include <string>

#include "VertexChild.h"
#include "Buffer.hh"
#include "Leaf.h"

using namespace std;

/**
 * <summary> Class that represents inner nodes in graph.</summary>
 */

 //Declaration----------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
class Vertex : public VertexChild
{
	public:
		int depthInTree;
		Vertex(int offset);
		~Vertex() = default;

		/**
		 * <summary> Gets vertex suffix link.</summary>
		 *
		 * <returns> Suffix link of the vertex.</returns>
		 */

		VertexChild * GetSuffixLink();

		/**
		 * <summary> Sets suffix link.</summary>
		 *
		 * <param name="suffixLinkVertex"> Vertex, that is set to be terminal vertex of the suffix link.</param>
		 */

		void SetSuffixLink(VertexChild* suffixLinkVertex);

		//void WriteVertex(int level, int *frontOrig, int offsetOnBuffer, int prevDepthInSymbols, Buffer* buffer);
		
		/**
		 * <summary> Deletes all children.</summary>
		 */

		void DeleteChildren();

		/**
		 * <summary> Searches for the end of edge, that starts with given symbol.</summary>
		 *
		 * <param name="symbol"> The symbol.</param>
		 * <param name="buffer"> Buffer object, to be able to search in symbol array.</param>
		 *
		 * <returns> Vertex that is at the end of edge.</returns>
		 */

		VertexBase * FindOutEdgeEnd(int symbol, Buffer<T> * buffer);
		
	private:
		VertexChild * suffixLink;   ///< The suffix link
};

//Definition----------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T>
Vertex<T>::Vertex(int offset) : VertexChild()
{
	this->offsetInBuffer = offset;
	this->firstChild = NULL;
}

template <typename T>
VertexChild* Vertex<T>::GetSuffixLink()
{
	return this->suffixLink;
}

template <typename T>
void Vertex<T>::SetSuffixLink(VertexChild * suffixLinkVertex)
{
	this->suffixLink = suffixLinkVertex;
}

//template <typename T>
//void Vertex<T>::WriteVertex(int level, int *frontOrig, int offsetOnBuffer, int prevDepthInSymbols, Buffer* buffer)
//{
//	VertexBase::WriteVertex(level, offsetOnBuffer + prevDepthInSymbols, offsetOnBuffer + this->depthInTree, buffer);
//
//	VertexBase* child = this->firstChild;
//	while (child != NULL)
//	{
//		if (typeid(child) == typeid(Vertex))
//			dynamic_cast<Vertex*>(child)->WriteVertex(
//				level + 1,
//				frontOrig,
//				child->offsetInBuffer,
//				this->depthInTree,
//				buffer);
//		else if (typeid(child) == typeid(Leaf))
//			dynamic_cast<Leaf*>(child)->WriteVertex(
//				level + 1,
//				this->depthInTree,
//				frontOrig,
//				buffer);
//	}
//}

template <typename T>
void Vertex<T>::DeleteChildren()
{
	VertexBase * child = this->firstChild;
	while (child)
	{
		if (typeid(child) == typeid(Vertex))
		{
			Vertex * vert = static_cast<Vertex*>(child);
			vert->DeleteChildren();
			child = child->nextSibling;
			delete(vert);
			continue;
		}
		child = child->nextSibling;
	}
}

template <typename T>
VertexBase * Vertex<T>::FindOutEdgeEnd(int symbol, Buffer<T> * buffer)
{
	VertexBase * child = this->firstChild;

	do
	{
		// Check first symbol
		if (buffer->GetSymbolFromBuffer(child->offsetInBuffer + this->depthInTree) == symbol)
			break;

		// Get next child
		child = child->nextSibling;
	} while (child != NULL);

	return child;
}