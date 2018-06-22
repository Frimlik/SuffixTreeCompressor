
#include "VertexBase.h"

#include <iostream>

using namespace std;

//void VertexBase::WriteVertex(int level, int labelStart, int labelEnd, Buffer* buffer)
//{
//	cout << this << ", ";
//	if (level == 0)
//		cout << "root";
//	for (int i = 0; i < level; i++)
//	{
//		cout << "-";
//	}
//
//	if (labelEnd >= labelStart)
//	{
//		for (int i = labelStart; i < labelEnd; i++)
//		{
//			cout << buffer->GetSymbolFromBuffer(i);
//		}
//	}
//	else
//	{
//		for (int i = labelStart; i < 2 * buffer->GetSlidingWindowLength(); i++)
//		{
//			cout << buffer->GetSymbolFromBuffer(i);
//		}
//		for (int i = 0; i < labelEnd; i++)
//		{
//			cout << buffer->GetSymbolFromBuffer(i);
//		}
//	}
//	
//	cout << endl;
//}