#include "WebmFile.h"

namespace MkvParser
{
	ParseResult WebmFile::ParseHeader()
	{
		Uint64 e_start = 0, e_size = 0, d_start = 0, d_size = 0;
		long len;
		BytePostion pos = 0;
		Int64 e_id = ReadID(pReader, pos, len);
		if (e_id != MkvId::kMkvEBML) return E_FILE_FORMAT_INVALID;
		d_size = ReadUInt(pReader, pos += len, len);
		d_start = (pos += len);
		e_size = d_size + d_start - e_start;
		EbmlHeader* pHeader = new EbmlHeader(e_start, e_size, d_start, d_size, nullptr, pReader);
		ParseResult status = pHeader->ParseFromFile();
		if (status != SUCCESS) delete pHeader;
		return status;
	}

	ParseResult WebmFile::ParseSegment()
	{
		Uint64 e_start = 0, e_size = 0, d_start = 0, d_size = 0;
		EbmlID e_id;
		ParseResult status;
		BytePostion stop = 0;
		BytePostion pos;
		long len;
		long long total, available;
		if (pReader->Length(&total, &available) == 0) return E_FILE_FORMAT_INVALID;
		if (total < available) return E_BUFFER_NOT_FULL;
		e_start = pHeader->GetElementStart() + pHeader->GetElementSize();
		stop = total;
		while (e_start < stop)
		{
			status = ReadID(pReader, e_start, len);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			if (status != MkvId::kMkvSegment)
			{
				pos = e_start + len; // start of size field
				e_id = status;
			}
			else break;
		}
		if (e_start == stop) return E_FILE_FORMAT_INVALID;
		// read size
		status = GetUIntLength(pReader, pos, len);
		if (status < 0) return E_FILE_FORMAT_INVALID;
		d_size = status;
		pos += len; // start of data field
		e_size = d_size + pos - e_start;
		d_start = pos;
		pSegment = new SegmentElement(e_start, e_size, d_start, d_size, nullptr, pReader);
		status = pSegment->ParseFromFile();
		if (status != SUCCESS) delete pSegment;
		return status;
	}
}