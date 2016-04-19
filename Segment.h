#pragma once
#include "IEbmlElement.h"
#include "SeekHead.h"
#include "SegmentInfo.h"
#include "Cluster.h"
#include "Track.h"
#include "Cues.h"
#include <vector>
#ifndef SEGMENT_H
#define SEGMENT_H

namespace MkvParser
{
	class SegmentElement : public IEbmlElement
	{
	protected:
		virtual ParseResult ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id);
		virtual void AFewMoreActions() { }
		virtual void PreClean()
		{
			if (!pSeekHead) delete pSeekHead;
			if (!pSegInfo) delete pSegInfo;
			ClusterCollection.clear();
			if (!pTracks) delete pTracks;
			if (!pCues) delete pCues;
		}
	public:
		SegmentElement(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart, Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(elementStart, elementSize, dataStart, dataSize, father, pReader),
			pSeekHead(nullptr),
			pSegInfo(nullptr),
			pTracks(nullptr),
			pCues(nullptr)
		{ }
		//virtual void GenerateSerializedInfo(Uint64 start);
		virtual EbmlID GetElementID() const { return MkvParser::MkvId::kMkvSegment; }
		SeekHead* pSeekHead; // Actually SeekHead is useless. Will write a new seekHead during writing.
		SegmentInfo* pSegInfo;
		std::vector<Cluster*> ClusterCollection;
		Tracks* pTracks;
		Cues* pCues;
	};
}

#endif