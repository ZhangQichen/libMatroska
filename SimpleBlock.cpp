#include "SimpleBlock.h"

namespace MkvParser
{
	ParseResult SimpleBlock::ParseFlags(IMkvReader* pReader, BytePostion& pos)
	{
		BytePostion cur_pos = pos;
		Int64 status = -100;
		unsigned char* buffer = new unsigned char;
		status = pReader->Read(cur_pos, 1, buffer);
		if (status != 0) return E_FILE_FORMAT_INVALID;
		this->m_Header.IsKeyFrames = (((*buffer & 0x80) >> 7) == 1);
		this->Header().IsInvisible = (((*buffer & 0x8) >> 3) == 1);
		short lacingResult = ((*buffer & 0x6) >> 1);
		this->m_Header.IsDiscardable = ((*buffer & 0x01) == 1);
		delete buffer;
		switch (lacingResult)
		{
		case LacingType::NoLacing:
			this->Header().Lacing = NoLacing; break;
		case LacingType::XiphLacing:
			this->Header().Lacing = XiphLacing; break;
		case LacingType::EbmlLacing:
			this->Header().Lacing = EbmlLacing; break;
		case LacingType::FixedSizeLacing:
			this->Header().Lacing = FixedSizeLacing; break;
		default:
			return E_FILE_FORMAT_INVALID;
		}
		cur_pos += 1;
		pos = cur_pos;
		return SUCCESS;
	}
}