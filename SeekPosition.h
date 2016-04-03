#pragma once
#ifndef SEEK_POSITION_H
#define SEEK_POSITION_H
#include "IEbmlElement.h"

namespace MkvParser
{
	class SeekPostion : public IEbmlElement
	{
	public:
		virtual EbmlID GetElementID() const { return MkvId::kMkvSeekPosition; }
		/*
		Parse.
		Return:
			0: successful
			<0: fail
		*/
		virtual ParseResult ParseFromFile();
		SeekPostion(BytePostion elem_start, BytePostion elem_size, BytePostion data_start, BytePostion data_size, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(elem_start, elem_size, data_start, data_size, father, pReader), TargetPosition(0)
		{

		}
		BytePostion TargetPosition;
	};
}

#endif // !SEEK_POSITION_H
