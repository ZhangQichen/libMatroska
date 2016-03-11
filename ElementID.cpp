#include "ElementID.h"

UInt64 ElementID::EBML_MAX_ID_LENGTH = 4; // Default is 4.

ElementID::ElementID(const VINT& vint) : m_data(vint)
{
	validate();
}

void ElementID::validate() const
{
	if (m_data.OctetLength() > EBML_MAX_ID_LENGTH)
		throw std::exception("Error: EbmlID's OctetLength is larger than MAX length");
	if (VINT::isVintDataAllZeros(m_data))
		throw std::exception("Error: All 0s EbmlId is reserved!");
	if (VINT::isVintDataAllOnes(m_data))
		throw std::exception("Error: All 1s EbmlId is reserved!");
	if (m_data.OctetLength() > 1)
	{
		if (m_data.VintData() <= VINT::MaxValueWithOctetLength(m_data.OctetLength() - 1))
		throw std::exception("Error: EbmlId is not shortest!");
	}
}