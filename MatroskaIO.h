#pragma once
#ifndef IO_H
#define IO_H
#include <fstream>
#include <string>
#include "Config.h"
#include "ElementID.h"

class MatroskaReader
{
public:
	MatroskaReader(std::string filename); // Default open mode is std::ifstream::binary.
	void ReadNextElementID(ElementID* pEID);
	void SeekPosition(UInt64 newPos); // Set the position of the next character to read.
	UInt64 CurrentPosition() const;
private:
	void ReadNextVint(VINT* pVint);
	std::ifstream m_inStream;
	UInt64 m_currentPos; // Absolute byte position with respect to the beginning.
};

#endif