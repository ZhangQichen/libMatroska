#include "MatroskaIO.h"

MatroskaReader::MatroskaReader(std::string filename) : m_inStream(filename, std::ifstream::binary), m_currentPos(0)
{
	if (!m_inStream.is_open())
		throw std::exception((std::string("Error: fail to read file: ") + filename).c_str());
}


