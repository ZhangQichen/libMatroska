#pragma once
#include "MkvReader.hpp"
#include "EbmlHeader.h"
#include "Segment.h"
#include "Types.h"
#include <string>
#include "Webmids.hpp"
#ifndef WEBM_FILE_H
#define WEBM_FILE_H

namespace MkvParser
{
	class WebmFile
	{
	protected:
		ParseResult ParseHeader();

		ParseResult ParseSegment();
	public:
		WebmFile(std::string filename) : FileName(filename)
		{
			pReader = new MkvReader();
		}
		bool Open()
		{
			int res = pReader->Open(FileName.c_str());
			if (res < 0) return false;
			return true;
		}
		void Close()
		{
			pReader->Close();
		}
		MkvReader* pReader;
		std::string FileName;
		EbmlHeader* pHeader;
		SegmentElement* pSegment;
		ParseResult ParseFile()
		{
			if (!Open()) return FAILED;
			ParseResult status;
			status = ParseHeader();
			if (status != SUCCESS) return status;
			status = ParseSegment();
			return status;
		}
		void WriteFile(std::string filename) { }
		~WebmFile()
		{
			if (!pReader) delete pReader;
			if (!pHeader) delete pHeader;
			if (!pSegment) delete pSegment;
		}
	};
}

#endif