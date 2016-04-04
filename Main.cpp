#include "EbmlHeader.h"
#include "MkvReader.hpp"
#include "assert.h"
using namespace MkvParser;
int main()
{
	MkvParser::MkvReader reader;
	if (reader.Open("C:\\Users\\Qichen Zhang\\Desktop\\1.webm") < 0)
		return 1;
	
	return 0;
}
