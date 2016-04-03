#include "SeekHead.h"
#include "Global.h"
namespace MkvParser
{
	ParseResult SeekHead::ParseFromFile()
	{
		BytePostion cur_pos = this->GetDataStart();
		BytePostion stop_pos = cur_pos + this->GetDataSize();
		while (cur_pos < stop_pos)
		{
			long len = -1;
			EbmlID e_ID = -1;
			e_ID = ReadID(this->m_pReader, cur_pos, len);
			if (!CheckIDSize(len)) return E_FILE_FORMAT_INVALID;
			if (e_ID < 0)  return E_FILE_FORMAT_INVALID;
			BytePostion e_start = cur_pos;
			cur_pos += len; // consume ID
			Int64 d_size = -1;
			d_size = GetUIntLength(this->m_pReader, cur_pos, len);
			if (!CheckElementSize(len)) return E_FILE_FORMAT_INVALID;
			if (d_size < 0) return E_FILE_FORMAT_INVALID;
			cur_pos += len; // consume size
			if (e_ID == MkvId::kMkvSeek)
			{
				Seek* pSeek = new Seek(e_start, cur_pos + d_size - e_start, cur_pos, d_size, this, this->m_pReader);
				ParseResult parseSeekRes = pSeek->ParseFromFile();
				if (ParseFromFile != 0) return E_FILE_FORMAT_INVALID;
				this->Seeks.push_back(pSeek);
			}
			else if (e_ID == MkvId::kMkvVoid) { }
			else if (e_ID == MkvId::kMkvEbmlCRC32) { }
			else return E_FILE_FORMAT_INVALID;
			cur_pos += d_size; // consume payload
		}
		if (cur_pos != stop_pos) return E_FILE_FORMAT_INVALID;
		return 0;
	}
}