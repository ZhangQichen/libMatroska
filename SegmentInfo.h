#pragma once
#ifndef SEGMENT_INFO_H
#define SEGMENT_INFO_H
#include "IEbmlElement.h"
#include <string>

namespace MkvParser
{
	class SegmentInfo : public IEbmlElement
	{
	public:
		virtual EbmlID GetElementID() const { return MkvId::kMkvInfo; }
		virtual ParseResult ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id);
		SegmentInfo(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart, Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(elementStart, elementSize, dataStart, dataSize, father, pReader),
			TimecodeScale(1000000),
			Duration(0),
			Title("Unknown"),
			MuxingApp("Unknown"),
			WritingApp("Unknown")
		{ }
		
		// For simlified use, ignore many other
		Uint64 TimecodeScale;
		double Duration;
		// DateUTC??
		std::string Title;
		std::string MuxingApp;
		std::string WritingApp;
	};
}

#endif // !SEGMENT_INFO_H
