#include "Block.h"
//#include <iostream>

namespace MkvParser
{
	ParseResult Block::ParseBlockHeader(IMkvReader* pReader, BytePostion& pos)
	{
		//std::cout << "On parsing Block header\n";
		BlockHeader& bHeader = this->Header();
		BytePostion cur_pos = pos;
		if (pReader == nullptr) return FAILED;
		long length = -100;
		ParseResult status = ReadUInt(pReader, cur_pos, length);
		if (status < 0) return E_FILE_FORMAT_INVALID;
		bHeader.TrackNumber = status;
		cur_pos += length; // consums track number
		Int64 result = -100;
		status = UnserializeInt(pReader, cur_pos, 2, result);
		if (status < 0) return E_FILE_FORMAT_INVALID;
		bHeader.TimeCode = static_cast<Int16>(result);
		cur_pos += 2;
		status = ParseFlags(pReader, cur_pos);
		if (status == SUCCESS) pos = cur_pos;
		return status;
	}

	ParseResult Block::ParseLacedData(IMkvReader* pReader, BytePostion& pos, const BytePostion& stop, const LacingType& lacing, std::vector<Frame*>& frames)
	{
		//std::cout << "On parsing Block laced data\n";
		if (frames.size() != 0) frames.clear();
		BytePostion cur_pos = pos;
		Byte buffer = ' ';
		Int64 status = -100;
		if (lacing == LacingType::NoLacing)
		{
			short frame_num = 1;
			Frame* fm = new Frame();
			fm->Position = cur_pos;
			fm->Length = stop - cur_pos;
			cur_pos = stop;
			pos = cur_pos;
			frames.push_back(fm);
			return SUCCESS;
		}
		else
		{
			status = pReader->Read(cur_pos, 1, &buffer);
			if (status != 0) return E_FILE_FORMAT_INVALID;
			Uint16 frame_num = buffer + 1;
			cur_pos += 1; // consumes number of frames bit in lace
			if (lacing == LacingType::FixedSizeLacing)
			{
				if ((stop - cur_pos) % frame_num != 0) return E_FILE_FORMAT_INVALID;
				Uint64 frame_size = (stop - cur_pos) / frame_num;
				for (int i = 0; i < frame_num; ++i)
				{
					Frame* fm = new Frame();
					fm->Position = cur_pos;
					fm->Length = frame_size;
					cur_pos += frame_size;
				}
				if (cur_pos != stop) return E_FILE_FORMAT_INVALID;
				pos = cur_pos;
				return SUCCESS;
			}
			else if (lacing == LacingType::XiphLacing)
			{
				Uint64 accumulatedFrameSize = 0;
				Uint64 accumulatedFrameNum = 0;
				Ubyte readByte = 0;
				Int64 status = -1;
				// Read frame size
				while (accumulatedFrameNum < frame_num - 1)
				{
					if (cur_pos >= stop) return E_FILE_FORMAT_INVALID;
					status = pReader->Read(cur_pos, 1, &readByte);
					if (!status) return E_FILE_FORMAT_INVALID;
					cur_pos += 1;
					if (readByte == 0)
					{
						if (accumulatedFrameSize == 0) return E_FILE_FORMAT_INVALID;
						if (accumulatedFrameSize % 255 != 0) return E_FILE_FORMAT_INVALID;
						Frame* fm = new Frame();
						fm->Length = accumulatedFrameSize;
						frames.push_back(fm);
						accumulatedFrameNum += 1;
						accumulatedFrameSize = 0;
					}
					else
					{
						accumulatedFrameSize += readByte;
						if (readByte < 255)
						{
							Frame* fm = new Frame();
							fm->Length = accumulatedFrameSize;
							frames.push_back(fm);
							accumulatedFrameNum += 1;
							accumulatedFrameSize = 0;
						}
					}
				}
				if (cur_pos >= stop) return E_FILE_FORMAT_INVALID;// cur_pos now points to the start of the first frame
				for (int i = 0; i < frames.size(); ++i)
				{
					if (cur_pos >= stop) return E_FILE_FORMAT_INVALID;
					frames[i]->Position = cur_pos;
					cur_pos += frames[i]->Length;
				}
				// cur_pos now points to the start of the last frame
				Frame* fm = new Frame();
				fm->Position = cur_pos;
				fm->Length = stop - cur_pos;
				frames.push_back(fm);
				cur_pos = stop;
				pos = cur_pos;
				return SUCCESS;
			}
			else // Ebml lacting
			{
				Uint64 accumulatedFrameNum = 0;
				Int64 status = 0;
				long length = 0;
				bool isFirstFm = true;
				Uint64 preLength = 0;
				// Read frame size
				while (accumulatedFrameNum < frame_num - 1)
				{
					if (cur_pos >= stop) return E_FILE_FORMAT_INVALID;
					status = ReadUInt(pReader, cur_pos, length);
					if (cur_pos + length >= stop) return E_FILE_FORMAT_INVALID;
					if (status <= 0) return E_FILE_FORMAT_INVALID;
					if (isFirstFm)
					{
						Frame* fm = new Frame();
						fm->Length = status;
						preLength = fm->Length;
						isFirstFm = false;
						frames.push_back(fm);
					}
					else
					{
						// caculate diff
						// 7 * length - 1: number of bits of VINT_DATA - 1.
						Int64 delta_len = status - ((1LL << (7 * length - 1)) - 1LL);
						preLength += delta_len;
						if (delta_len <= 0) return E_FILE_FORMAT_INVALID;
						Frame* fm = new Frame();
						fm->Length = preLength;
						isFirstFm = false;
						frames.push_back(fm);
					}
					cur_pos += length;
				}
				// cur_pos not points to the start of the first frame
				for (int i = 0; i < frames.size(); ++i)
				{
					if (cur_pos >= stop) return E_FILE_FORMAT_INVALID;
					frames[i]->Position = cur_pos;
					cur_pos += frames[i]->Length;
				}
				if (cur_pos >= stop) return E_FILE_FORMAT_INVALID;// cur_pos now points to the start of the last frame
				Frame* fm = new Frame();
				fm->Position = cur_pos;
				fm->Length = stop - cur_pos;
				frames.push_back(fm);
				cur_pos = stop;
				pos = cur_pos;
				return SUCCESS;
			}
		}
	}

	ParseResult Block::ParseFlags(IMkvReader* pReader, BytePostion& pos)
	{
		//std::cout << "On parsing Block flags\n";
		BytePostion cur_pos = pos;
		Int64 status = -100;
		unsigned char* buffer = new unsigned char;
		status = pReader->Read(cur_pos, 1, buffer);
		if (status != 0) return E_FILE_FORMAT_INVALID;
		this->Header().IsInvisible = (((*buffer & 0x8) >> 3) == 1);
		short lacingResult = ((*buffer & 0x6) >> 1);
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

	ParseResult Block::ParseFromFile()
	{
		//std::cout << "On parsing Block from file\n";
		BytePostion cur_pos = this->GetDataStart();
		BytePostion stop_pos = this->GetDataSize() + cur_pos;
		ParseResult status = ParseBlockHeader(this->m_pReader, cur_pos);
		if (cur_pos >= stop_pos) return E_FILE_FORMAT_INVALID;
		if (status != SUCCESS) return E_FILE_FORMAT_INVALID;
		status = ParseLacedData(this->m_pReader, cur_pos, stop_pos, this->Header().Lacing, this->Frames);
		if (cur_pos != stop_pos) return E_FILE_FORMAT_INVALID;
		if (status != SUCCESS) return E_FILE_FORMAT_INVALID;
		return SUCCESS;
	}
}