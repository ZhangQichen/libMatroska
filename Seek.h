#pragma once
#include "IEbmlElement.h"
#ifndef SEEK
#define SEEK

namespace MkvParser
{
	class Seek : public IEbmlElement
	{
	public:
		virtual EbmlID GetElementID() const { return MkvId::kMkvSeek; }
		virtual ParseResult ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id);
		Seek(BytePostion e_start, BytePostion e_size, BytePostion d_start, BytePostion d_size, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(e_start, e_size, d_start, d_size, father, pReader)
		{ }
		EbmlID SeekID;
		BytePostion SeekPosition; // Byte position relative to the start of the data field of the entire segment
	};
}

#endif // !Seek
