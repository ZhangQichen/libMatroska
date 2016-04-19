#include "WebmFile.h"
#include <iostream>

namespace MkvParser
{
	ParseResult WebmFile::ParseHeader()
	{
		std::cout << "Start Parsing Header\n";
		Uint64 e_start = 0, e_size = 0, d_start = 0, d_size = 0;
		long len;
		BytePostion pos = 0;
		Int64 e_id = ReadID(pReader, pos, len);
		if (e_id != MkvId::kMkvEBML) return E_FILE_FORMAT_INVALID;
		d_size = ReadUInt(pReader, pos += len, len);
		d_start = (pos += len);
		e_size = d_size + d_start - e_start;
		pHeader = new EbmlHeader(e_start, e_size, d_start, d_size, nullptr, pReader);
		ParseResult status = pHeader->ParseFromFile();
		//if (status != SUCCESS) delete pHeader;
		return status;
	}

	ParseResult WebmFile::ParseSegment()
	{
		std::cout << "Start Parsing Segment\n";
		Uint64 e_start = 0, e_size = 0, d_start = 0, d_size = 0;
		EbmlID e_id;
		ParseResult status;
		BytePostion stop = 0;
		BytePostion pos;
		long len = -1;
		long long total, available;
		if (pReader->Length(&total, &available) != 0)
			return E_FILE_FORMAT_INVALID;
		if (total < available) return E_BUFFER_NOT_FULL;
		e_start = pHeader->GetElementStart() + pHeader->GetElementSize();
		stop = total;
		while (e_start < stop)
		{
			status = ReadID(pReader, e_start, len);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			if (status != MkvId::kMkvSegment)
			{
				e_id = status;
			}
			else break;
		}
		// e_start : starting of segmeng
		if (e_start == stop) return E_FILE_FORMAT_INVALID;
		pos = e_start + len; // start of size field
		// read size
		status = ReadUInt(pReader, pos, len);
		if (status < 0) return E_FILE_FORMAT_INVALID;
		d_size = status;
		pos += len; // start of data field
		e_size = d_size + pos - e_start;
		d_start = pos;
		pSegment = new SegmentElement(e_start, e_size, d_start, d_size, nullptr, pReader);
		std::cout << "On parsing segment\n\n";
		status = pSegment->ParseFromFile();
		//if (status != SUCCESS) delete pSegment;
		return status;
	}
}