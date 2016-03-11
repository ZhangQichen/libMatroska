#include "VINT.h"

VINT::VINT(const std::vector<BinaryByte>& sequence) : m_VINT_width(sequence.size() - 1)
{
	if (OctetLength() > 8) throw std::exception("Error: Too large for VINT type");
	if (OctetLength == 0) throw std::exception("Error: OctetLength = 0!");
	m_VINT_data = 0;

	// Remove the VINT_mark
	BinaryByte mask = 1;
	mask = (mask << (8- OctetLength()));
	m_VINT_data = (sequence[0] ^ mask); 

	for (int i = 1; i < sequence.size(); ++i)
	{
		m_VINT_data <<= 8;
		m_VINT_data += (uint32_t)(sequence[i]);
	}
}

std::string VINT::ToString() const
{
	UInt64 mask = 1;
	mask <<= (64 - OctetLength()); // Vint_mark bit is 1 and other bits are 0 now.
	UInt64 originData = m_VINT_data ^ mask; // The Vint_mark bit of m_VINT_data is 1 now.
	BinaryByte* ptr = (BinaryByte*)&originData;
	std::string result = "";
	for (int i = 0; i < sizeof(originData); ++i, ++ptr)
	{
		result += *ptr;
	}
	return result;
}

UInt64 VINT::MaxValueWithOctetLength(UInt16 octetLength)
{
	if (octetLength > 8)
		throw std::exception("Error: OctetLength larger than 8!");
	return (UInt64)(std::pow(2.0, double(octetLength * 7)) - 1);
}

bool VINT::isVintDataAllZeros(const VINT& vint)
{
	return (vint.VintData() == 0);
}

bool VINT::isVintDataAllOnes(const VINT& vint)
{
	UInt64 mask = 0xFFFFFFFFFFFFFFFF;
	mask <<= vint.VintDataBitNumber(); // 1000 0000 or 1100 0000 0000 0000 ...
	return ((vint.VintData() | mask) == (UInt64)0xFFFFFFFFFFFFFFFF);
}