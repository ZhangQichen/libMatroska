#include "Seek.h"
#include "Global.h"

namespace MkvParser
{
	ParseResult Seek::ParseFromFile()
	{
		BytePostion pos = this->GetDataStart();
		BytePostion stop = pos + this->GetDataSize();
		long size = 0;
		EbmlID result = -1;
		Int64 d_size = -1;
		BytePostion e_start = -1;
		ParseResult parseRes = -1;
		while (pos < stop)
		{
			e_start = pos;
			// Read ID
			result = ReadID(this->m_pReader, pos, size);
			if (result <= 0) return E_FILE_FORMAT_INVALID;
			if (size <= 0 || size >(gEbmlHeader == nullptr ? 4 : gEbmlHeader->EbmlMaxIDLength))
				return E_FILE_FORMAT_INVALID;
			pos += size; // consume ID
			// Read data size
			d_size = GetUIntLength(this->m_pReader, pos, size);
			if (d_size <= 0 || d_size > (gEbmlHeader == nullptr ? 8 : gEbmlHeader->EbmlMaxSizeLength))
			{
				return E_FILE_FORMAT_INVALID;
			}
			d_size = ReadUInt(this->m_pReader, pos, size);
			pos += size; // consume data size. now points to the start of the data field.
			// Parse data
			switch (result)
			{
			case MkvId::kMkvSeekID:
				if (this->pSeekID != nullptr)
					return E_FILE_FORMAT_INVALID; // One seek can only have one seekID
				this->pSeekID = new SeekID(e_start, pos + d_size - e_start, pos, d_size, this, this->m_pReader);
				parseRes = this->pSeekID->ParseFromFile();
				pos += d_size; // consume seekID element. now points to the next element ID.
				break;
			case MkvId::kMkvSeekPosition:
				if (this->pSeekPosition != nullptr)
					return E_FILE_FORMAT_INVALID; // One seek can only have one seekPosition
				this->pSeekPosition = new SeekPostion(e_start, pos + d_size - e_start, pos, d_size, this, this->m_pReader);
				parseRes = this->pSeekPosition->ParseFromFile();
				pos += d_size; // consume seekPosition element. now points to the next element ID.
				break;
			case MkvId::kMkvVoid :
				parseRes = 0;
				pos += d_size;
				break;
			default:
				parseRes = -1;
				break;
			}
			if (parseRes != 0)
				return E_FILE_FORMAT_INVALID;
		}
		if (pos != stop)
			return E_FILE_FORMAT_INVALID;
		return SUCCESS;
	}

	Seek::~Seek()
	{
		if (this->pSeekID != nullptr) delete pSeekID;
		if (pSeekPosition != nullptr) delete pSeekPosition;
	}
}