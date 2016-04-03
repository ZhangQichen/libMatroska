#pragma once
#ifndef IMKV_READER_H
#define IMKV_READER_H
#include "Types.h"
namespace MkvParser
{
	class IMkvReader {
	public:
		virtual int Read(long long pos, long len, unsigned char* buf) = 0;
		virtual int Length(long long* total, long long* available) = 0;
	protected:
		virtual ~IMkvReader() {};
	};

	const Int64 E_FILE_FORMAT_INVALID = -3;
	const Int64 E_BUFFER_NOT_FULL = -2;
	const Int64 SUCCESS = 0;
	const Int64 FAILED = -1;
}
#endif