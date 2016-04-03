/*
Some global variables and functions related to them.
*/

#pragma once
#include "EbmlHeader.h"

namespace MkvParser
{
	EbmlHeader* gEbmlHeader = nullptr;

	/*
	Check the range of ID according to EBML header
	params:
		IDSize: ID's size in byte number
	Return:
		true: within ID range
		false: ...
	*/
	inline bool CheckIDSize(long IDSize) // Size in Byte number
	{
		return !(IDSize < 0 || IDSize >(gEbmlHeader == nullptr ? 4 : gEbmlHeader->EbmlMaxIDLength));
	}

	/*
	Check the range of Size field according to EBML header
	Params:
		sizeLen: size's byte number
	Return:
		true: within size range
		false: otherwise
	*/
	inline bool CheckElementSize(long sizeLen)
	{
		return !(sizeLen < 0 || sizeLen >(gEbmlHeader == nullptr ? 8 : gEbmlHeader->EbmlMaxSizeLength));
	}
}