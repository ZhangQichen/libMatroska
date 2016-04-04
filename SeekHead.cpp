#include "SeekHead.h"
namespace MkvParser
{
	ParseResult SeekHead::ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id)
	{
		if (id == MkvId::kMkvSeek)
		{
			Seek* pSeek = new Seek(e_start, d_start + d_size - e_start, d_start, d_size, this, this->m_pReader);
			ParseResult parseSeekRes = pSeek->ParseFromFile();
			if (parseSeekRes != 0) return E_FILE_FORMAT_INVALID;
			this->Seeks.push_back(pSeek);
			return SUCCESS;
		}
		else if (id == MkvId::kMkvVoid) { return SUCCESS; }
		return SUCCESS;
	}
}