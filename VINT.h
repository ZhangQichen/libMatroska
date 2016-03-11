#pragma once
#ifndef VINT_H
#define VINT_H

#include <string>
#include "Config.h"
#include <vector>

class VINT
{
public:
	/*
	Param: bytes big endian
	*/
	VINT(const std::vector<BinaryByte>&);

	// m_VINT_width + 1
	UInt16 OctetLength() const { return m_VINT_width + 1; }

	UInt64 VintData() const { return m_VINT_data; }

	UInt16 VintWidth() const { return m_VINT_width; }

	// Bit number of VINT_data = OctetLength * 8 - OctetLength
	UInt64 VintDataBitNumber() const { return OctetLength() * 7; }

	std::string ToString() const;

	static UInt64 MaxValueWithOctetLength(UInt16 octetLength);
	static bool isVintDataAllZeros(const VINT&);
	static bool isVintDataAllOnes(const VINT&);
private:
	UInt16 m_VINT_width;
	UInt64 m_VINT_data; // Max bit number of VINT is 64
};

#endif
