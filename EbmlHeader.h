#pragma once
#ifndef EBML_HEADER_H
#define EBML_HEADER_H
#include "IEbmlElement.h"
#include "Webmids.hpp"
#include "Types.h"
#include <string>

namespace MkvParser
{
	class EbmlHeader : public IEbmlElement
	{
	public:
		EbmlHeader(long long elementStart, long long elementSize, long long dataStart, long long dataSize, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement::IEbmlElement(elementStart, elementSize, dataStart, dataSize, father, pReader),
			EbmlVersion(1),
			EbmlReadVersion(1),
			EbmlMaxIDLength(4),
			EbmlMaxSizeLength(8),
			DocType("Matroska"),
			DocTypeVersion(1),
			DocTypeReadVersion(1)
		{
		}
		/*
		Parse header from file.
		Return:
			0: successful
			negative number: failed
		*/
		virtual Int64 ParseFromFile();
		virtual Int64 GetElementID() const { return MkvParser::kMkvEBML; }

		Uint64 EbmlVersion;
		Uint64 EbmlReadVersion;
		Uint64 EbmlMaxIDLength;
		Uint64 EbmlMaxSizeLength;
		std::string DocType;
		Uint64 DocTypeVersion;
		Uint64 DocTypeReadVersion;
	protected:
	private:
	};
}
#endif