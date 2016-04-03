#include "EbmlHeader.h"
#include "mkvreader.hpp"

using namespace MkvParser;

Int64 MkvParser::EbmlHeader::ParseFromFile()
{
	Uint64 stopPos = this->GetDataStart() + this->GetDataSize();
	Uint64 pos = this->GetDataStart();
	while (pos < stopPos)
	{
		long length;
		Int64 size;
		
		GetUIntLength(this->m_pReader, pos, length);
		if (length <= 0) return E_BUFFER_NOT_FULL;
		Uint64 id = ReadID(this->m_pReader, pos, length);
		pos += length; // pos: start of ebml size field
		size = ReadUInt(this->m_pReader, pos, length); // size of the element
		if (size <= 0) return E_FILE_FORMAT_INVALID;
		pos += length; // pos: start of ebml data field
		long status;
		switch (id)
		{
		case MkvId::kMkvEBMLVersion:
			status = (this->EbmlVersion = UnserializeUInt(this->m_pReader, pos, size));
			break;
		case MkvId::kMkvEBMLReadVersion:
			status = (this->EbmlVersion = UnserializeUInt(this->m_pReader, pos, size));
			break;
		case MkvId::kMkvEBMLMaxIDLength:
			status = (this->EbmlMaxIDLength = UnserializeUInt(this->m_pReader, pos, size));
			break;
		case MkvId::kMkvEBMLMaxSizeLength:
			status = (this->EbmlMaxSizeLength = UnserializeUInt(this->m_pReader, pos, size));
			break;
		case MkvId::kMkvDocType:
			status = UnserializeString(this->m_pReader, pos, size, this->DocType);
			break;
		case MkvId::kMkvDocTypeVersion:
			status = (this->DocTypeReadVersion = UnserializeUInt(this->m_pReader, pos, size));
			break;
		case MkvId::kMkvDocTypeReadVersion:
			status = (this->DocTypeReadVersion = UnserializeUInt(this->m_pReader, pos, size));
			break;
		case MkvId::kMkvVoid:
			status = 0;
			break;
		case MkvId::kMkvEbmlCRC32:
			status = 0;
			break;
		default:
			status = -1;
			break;
		}
		if (status < 0) return E_FILE_FORMAT_INVALID;
		pos += size;
	}
	if (pos == stopPos)
		return 0;
	else return E_FILE_FORMAT_INVALID;
}