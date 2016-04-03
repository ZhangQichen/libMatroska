#pragma once
#ifndef SEEK
#define SEEK
#include "IEbmlElement.h"
#include "SeekID.h"
#include "SeekPosition.h"

namespace MkvParser
{
	class Seek : public IEbmlElement
	{
	public:
		virtual EbmlID GetElementID() const { return MkvId::kMkvSeek; }
		/*
		Parse a seek's data field from file.
		Return:
			0: successful
			<0: failed
		*/
		virtual Int64 ParseFromFile();
		Seek(BytePostion e_start, BytePostion e_size, BytePostion d_start, BytePostion d_size, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(e_start, e_size, d_start, d_size, father, pReader), pSeekID(nullptr), pSeekPosition(nullptr)
		{ }
		SeekID* pSeekID;
		SeekPostion* pSeekPosition;
		virtual ~Seek();
	};
}

#endif // !Seek
