#pragma once
#include "IEbmlElement.h"
#include <vector>
#ifndef CUES_H
#define CUES_H

namespace MkvParser
{
	class CueTrackPositions : public IEbmlElement
	{
	protected:
		virtual ParseResult ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id);
	public:
		CueTrackPositions(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart, Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(elementStart, elementSize, dataStart, dataSize, father, pReader),
			CueBlockNumber(1)
		{ }
		virtual EbmlID GetElementID() const { return MkvId::kMkvCues; }
		Uint64 CueTrack; // The track for which a position is given
		Uint64 CueClusterPosition; // The position of the Cluster containing the required Block relative to the start of the data field of Segment
		Uint64 CueBlockNumber; // Number of the Block in the specified Cluster. Starts from 1.
	};

	class CuePoint : public IEbmlElement
	{
	protected:
		virtual ParseResult ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id);
		virtual void PreClean() { CueTrackPositionCollection.clear(); }
	public:
		CuePoint(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart, Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(elementStart, elementSize, dataStart, dataSize, father, pReader)
		{ }
		virtual EbmlID GetElementID() const { return MkvId::kMkvCues; }
		Uint64 CueTime;
		std::vector<CueTrackPositions*> CueTrackPositionCollection;
	};

	class Cues : public IEbmlElement
	{
	protected:
		virtual ParseResult ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id);
		virtual void PreClean() { CuePointCollection.clear(); }
	public:
		Cues(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart, Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(elementStart, elementSize, dataStart, dataSize, father, pReader)
		{ }
		virtual EbmlID GetElementID() const { return MkvId::kMkvCues; }
		std::vector<CuePoint*> CuePointCollection;
	};
}

#endif