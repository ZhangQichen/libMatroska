#pragma once
#ifndef BLOCK_H
#define BLOCK_H
#include "IEbmlElement.h"
#include <vector>
#include "Frame.h"

namespace MkvParser
{
	enum LacingType
	{
		NoLacing = 0, XiphLacing = 1, EbmlLacing = 3, FixedSizeLacing = 2
	};

	class BlockHeader
	{
	public:
		Uint64 TrackNumber;
		Int16 TimeCode; // Relative to cluster's timecode
		bool IsInvisible;
		LacingType Lacing;
	};

	class Block : public IEbmlElement
	{
	public:
		/*
		Return:
			E_FILE_FORMAT_INVALID
			E_BUFFER_NOT_FULL
			SUCCESS
			FAILED
		*/
		virtual ParseResult ParseFromFile();
		virtual EbmlID GetElementID() const { return MkvId::kMkvBlock; }
		Block(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart, Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(elementStart, elementSize, dataStart, dataSize, father, pReader)
		{}
		virtual BlockHeader& Header() { return m_Header; }
		virtual std::vector<Frame*>& GetFrames() { return Frames; }
		std::vector<Frame*> Frames;
	protected:
		/*
		Parse Block header from file.
		Params:
			pReader: reader
			pos: position of the starting of Block header. Set to the end of Block header if successful and unchanged if failed.
		Return:
			Result in IMkvReader.h
		*/
		virtual ParseResult ParseBlockHeader(IMkvReader* pReader, BytePostion& pos);
		
		/*
		Parse Block header from file.
		Params:
		pReader: reader
		pos: position of the end of Block header. Set to the end of Block header if successful and unchanged if failed.
		bHeader: store parsed header if successful
		frames: store parsed frames
		Return:
		Result in IMkvReader.h
		*/
		virtual ParseResult ParseLacedData(IMkvReader* pReader, BytePostion& pos, const BytePostion& stop, const LacingType& lacing, std::vector<Frame*>& frames);

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
		BlockHeader m_Header;
	};
}

#endif