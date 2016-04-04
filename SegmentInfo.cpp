#include "SegmentInfo.h"
//#include <iostream>

namespace MkvParser
{
	ParseResult SegmentInfo::ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id)
	{
		//std::cout << "On parsing Seg Info\n";
		if (id == MkvId::kMkvTimecodeScale)
		{
			Int64 result = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (result < 0) return E_FILE_FORMAT_INVALID;
			this->TimecodeScale = (Uint64)result;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvDuration)
		{
			double duration = -1;
			Int64 result = UnserializeFloat(this->m_pReader, d_start, d_size, duration);
			if (result != 0) return E_FILE_FORMAT_INVALID;
			this->Duration = duration;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvMuxingApp)
		{
			std::string str = "";
			Int64 result = UnserializeString(this->m_pReader, d_start, d_size, str);
			if (result != 0) return E_FILE_FORMAT_INVALID;
			this->MuxingApp = str;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvWritingApp)
		{
			std::string str = "";
			Int64 result = UnserializeString(this->m_pReader, d_start, d_size, str);
			if (result != 0) return E_FILE_FORMAT_INVALID;
			this->WritingApp = str;
			return SUCCESS;
		}
		else return SUCCESS;
	}
}