#pragma once
#ifndef SIMPLE_BLOCK_H
#define SIMPLE_BLOCK_H
#include "Block.h"

namespace MkvParser
{
	class SimpleBlockHeader : public BlockHeader
	{
	public:
		bool IsKeyFrames; // Set when the block contains only key frames
		bool IsDiscardable; // Set when the block is discardable
	};

	class SimpleBlock : public Block, public IBlockEntry
	{
	public:
		virtual EbmlID GetElementID() const { return MkvId::kMkvSimpleBlock; }
		SimpleBlock(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart, Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader)
			: Block(elementStart, elementSize, dataStart, dataSize, father, pReader)
		{}
		virtual BlockHeader& Header() { return m_Header; }
		// IBlockEntry
		virtual Uint64 GetTrackNumber()  const { return m_Header.TrackNumber; }
		virtual Int16 GetTimeCode() const { return m_Header.TimeCode; }
		virtual bool IsInvisible() const { return m_Header.IsInvisible; }
		virtual bool IsKeyFrames() const { return m_Header.IsKeyFrames; }
		virtual bool IsDiscardable() const { return m_Header.IsDiscardable; }
		virtual std::vector<Frame*>& GetFrames() { return Block::Frames; }
		// END IBlockEntry
	protected:
		/*
		Parse Block flag from file.
		Params:
		pReader: reader
		pos: position of the end of Block header. Set to the end of Block header if successful and unchanged if failed.
		Return:
		Result in IMkvReader.h
		*/
		virtual ParseResult ParseFlags(IMkvReader* pReader, BytePostion& pos);
	private:
		SimpleBlockHeader m_Header;
	};
}

#endif