#pragma once
#ifndef ELEMENT_ID_H
#define ELEMENT_ID_H

#include "VINT.h"
#include "Config.h"

class ElementID
{
public:
	ElementID(const VINT&);

	static UInt64 EBML_MAX_ID_LENGTH;// Max Octet Length of EbmlID
	
private:
	VINT m_data;

	void validate() const;

};
#endif // !ELEMENT_ID_H
