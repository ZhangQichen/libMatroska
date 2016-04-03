#pragma once
#ifndef SEEK_ID
#define SEEK_ID
#include "IEbmlElement.h"

namespace MkvParser
{
	class SeekID : public IEbmlElement
	{
	public:
		virtual EbmlID GetElementID() const { return MkvId::kMkvSeekID; }
		/*
		Parse SeekID from file.
		Return:
			0: successful
			negative number: fail
		*/
		virtual ParseResult ParseFromFile();
		EbmlID RefencedID;
		SeekID(BytePostion elementStart, BytePostion elementSize, BytePostion dataStart, BytePostion dataSize, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement::IEbmlElement(elementStart, elementSize, dataStart, dataSize, father, pReader), RefencedID(0)
		{ }
	};
}

#endif