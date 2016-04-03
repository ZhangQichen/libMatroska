#pragma once
#ifndef SEEK_HEAD_H
#define SEEK_HEAD_H

#include "IEbmlElement.h"
#include <vector>
#include "Seek.h"

namespace MkvParser
{
	class SeekHead : public IEbmlElement
	{
	public:
		SeekHead(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(e_start, e_size, d_start, d_size, father, pReader)
		{ }
		virtual EbmlID GetElementID() const { return MkvId::kMkvSeekHead; }
		virtual ParseResult ParseFromFile();
		std::vector<Seek*> Seeks;
	};
}

#endif