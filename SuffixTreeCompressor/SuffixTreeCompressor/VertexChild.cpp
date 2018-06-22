
#include "VertexChild.h"

int VertexChild::UpdateOffset()
{
	VertexBase * child = this->firstChild;

	int newestOffset = -1;
	int offset = -1;

	while (child != NULL)
	{
		offset = child->UpdateOffset();

		//at leaves it is sure that all offsets come from that buffer half that is between back and front. Inner vertices can have out-of-date offsets, but leaves not, as when they are created,
		// their current offsetInBuffer is directed by current position of front and thus it is the newest possible
		//so we can always compare simply "offset > newestOffset"
		if (offset > newestOffset)
		{
			newestOffset = offset;
		}

		child = child->nextSibling;
	}

	this->offsetInBuffer = newestOffset;

	return newestOffset;
}

void VertexChild::MoveToFront(VertexBase * prevChild, VertexBase * movedChild)
{
	prevChild->nextSibling = movedChild->nextSibling;
	movedChild->nextSibling = this->firstChild;
	this->firstChild = movedChild;
}

void VertexChild::AddChild(VertexBase * child)
{
	if (this->firstChild != NULL)
	{
		child->nextSibling = this->firstChild;
	}
	else
	{
		child->nextSibling = NULL;
	}

	this->firstChild = child;
}

void VertexChild::ReplaceChild(VertexBase * oldVert, VertexBase * newVert)
{
	//Redirect pointers from/to sibling vertices
	//Find vertex (inner node or leaf) that is previous sibling of the oldest leaf - "sibling" pointer is pointing to it
	VertexBase * prevSibling = GetPrevSiblingOfChild(oldVert);

	if (prevSibling != NULL)
	{
		prevSibling->nextSibling = newVert;
		newVert->nextSibling = oldVert->nextSibling;
	}
	else
	{
		newVert->nextSibling = this->firstChild->nextSibling;
		this->firstChild = newVert;
	}
}

void VertexChild::RemoveChild(VertexBase * vertexToRemove)
{
	VertexBase * prevSibling = GetPrevSiblingOfChild(vertexToRemove);

	if (prevSibling != NULL)
	{
		prevSibling->nextSibling = vertexToRemove->nextSibling;
	}
	else
	{
		this->firstChild = vertexToRemove->nextSibling;
	}
}

VertexBase * VertexChild::GetPrevSiblingOfChild(VertexBase * vertex)
{
	VertexBase * prevSibling = this->firstChild;

	if (prevSibling == vertex)
	{
		return NULL;
	}

	while (prevSibling->nextSibling != vertex)
	{
		prevSibling = prevSibling->nextSibling;
	}

	return prevSibling;
}