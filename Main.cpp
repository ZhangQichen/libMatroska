#include "EbmlHeader.h"
#include "MkvReader.hpp"
#include "assert.h"
using namespace MkvParser;
int main()
{
	MkvParser::MkvReader reader;
	if (reader.Open("C:\\Users\\Qichen Zhang\\Desktop\\video1.webm") < 0)
		return 1;
	Uint64 e_start = 0, e_size = 0, d_start = 0, d_size = 0;
	long len;
	Uint64 pos = 0;
	Uint64 e_id = ReadID(&reader, pos, len);
	assert(e_id == MkvId::kMkvEBML);
	d_size = ReadUInt(&reader, pos += len, len);
	assert(e_size >= 0);
	d_start = (pos += len);
	e_size = d_size + d_start - e_start;
	EbmlHeader h(e_start, e_size, d_start, d_size, nullptr, &reader);
	assert(h.ParseFromFile() == 0);
}
