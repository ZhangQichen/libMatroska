#include "Seek.h"
#include "Serialization.h"
//#include <iostream>

namespace MkvParser
{
	ParseResult Seek::ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id)
	{
		//std::cout << "On parsing Seek\n";
		ParseResult status = -101;
		if (id == MkvId::kMkvSeekID)
		{
			long len = -1;
			status = ReadID(m_pReader, d_start, len);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			if (len != d_size) return E_FILE_FORMAT_INVALID;
			this->SeekID = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvSeekPosition)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->SeekPosition = status;
			return SUCCESS;
		}
		else return SUCCESS;
	}
}