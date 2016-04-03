#include "SegmentInfo.h"
#include "Global.h"

namespace MkvParser
{
	ParseResult SegmentInfo::ParseFromFile()
	{
		BytePostion cur_pos = this->GetDataStart();
		Uint64 stop_pos = this->GetDataSize() + cur_pos;
		while (cur_pos < stop_pos)
		{
			long length = -1;
			EbmlID id = ReadID(this->m_pReader, cur_pos, length);
			if (!CheckIDSize(length)) return E_FILE_FORMAT_INVALID;
			BytePostion e_start = cur_pos;
			cur_pos += length; // consumes id
			Int64 size = ReadUInt(this->m_pReader, cur_pos, length);
			if (!CheckElementSize(length)) return E_FILE_FORMAT_INVALID;
			Uint64 d_size = (Uint64)size;
			cur_pos += length; // consumes data size
			if (id == MkvId::kMkvTimecodeScale)
			{
				Int64 result = UnserializeUInt(this->m_pReader, cur_pos, size);
				if (result < 0) return E_FILE_FORMAT_INVALID;
				this->TimecodeScale = (Uint64)result;
			}
			else if (id == MkvId::kMkvDateUTC) { }
			else if (id == MkvId::kMkvDuration)
			{
				double duration = -1;
				Int64 result = UnserializeFloat(this->m_pReader, cur_pos, size, duration);
				if (result != 0) return E_FILE_FORMAT_INVALID;
				this->Duration = duration;
			}
			else if (id == MkvId::kMkvMuxingApp)
			{
				std::string str = "";
				Int64 result = UnserializeString(this->m_pReader, cur_pos, size, str);
				if (result != 0) return E_FILE_FORMAT_INVALID;
				this->MuxingApp = str;
			}
			else if (id == MkvId::kMkvWritingApp)
			{
				std::string str = "";
				Int64 result = UnserializeString(this->m_pReader, cur_pos, size, str);
				if (result != 0) return E_FILE_FORMAT_INVALID;
				this->WritingApp = str;
			}
			else if (id == MkvId::kMkvVoid) { }
			else return E_FILE_FORMAT_INVALID;
			cur_pos += d_size; // consumes data field
		}
		if (stop_pos != cur_pos)
			return E_FILE_FORMAT_INVALID;
		return SUCCESS;
	}
}