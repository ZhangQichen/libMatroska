#pragma once
#include "IEbmlElement.h"
#include "Block.h"
#include "IBlockEntry.h"
#ifndef BLOCK_GROUP
#define BLOCK_GROUP

namespace MkvParser
{
	class BlockGroup : public IEbmlElement, public IBlockEntry
	{
	public:
		BlockGroup(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart, Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(elementStart, elementSize, dataStart, dataSize, father, pReader),
			m_pBlock(nullptr),
			BlockDuration(-1000),
			DiscardPadding(-1000)
		{ }
		// IEbmlElement
		/*
		Return:
		E_FILE_FORMAT_INVALID
		E_BUFFER_NOT_FULL
		SUCCESS
		FAILED
		*/
		virtual ParseResult ParseFromFile();
		virtual EbmlID GetElementID() const { return MkvId::kMkvBlockGroup; }
		// END IEbmlElement
		// IBlockEntry
		virtual Uint64 GetTrackNumber() const
		{
			if (this->m_pBlock == nullptr) return -1;
			return this->m_pBlock->Header().TrackNumber;
		}
		virtual Int16 GetTimeCode() const
		{
			if (this->m_pBlock == nullptr) return -1;
			return this->m_pBlock->Header().TimeCode;
		}
		virtual bool IsInvisible() const
		{
			if (this->m_pBlock == nullptr) return false;
			return this->m_pBlock->Header().IsInvisible;
		}
		virtual bool IsKeyFrames() const { return false; }
		virtual bool IsDiscardable() const { return false; }
		// END IBlockEntry
		virtual std::vector<Frame*>& GetFrames()
		{
			if (this->m_pBlock == nullptr) return std::vector<Frame*>();
			else return this->m_pBlock->Frames;
		}
		Uint64 BlockDuration; // init as negative
		Block* BlockContent() { return m_pBlock; }
		// BlockAdditions
		// BlockMore
		// BlockAddID
		// BlockAdditional
		// ReferenceBLock
		Int64 DiscardPadding; // init as negative
	protected:
		Block* m_pBlock;
	};
}

#endif