#pragma once
#ifndef CLUSTER_H
#define CLUSTER_H
#include "IEbmlElement.h"
#include <string>
#include <vector>
#include "IBlockEntry.h"

namespace MkvParser
{
	class Cluster : public IEbmlElement
	{
	public:
		virtual ParseResult ParseFromFile();
		virtual EbmlID GetElementID() const { return MkvId::kMkvCluster; }
		Cluster(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart, Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(elementStart, elementSize, dataStart, dataSize, father, pReader),
			BlockCollection(),
			TimeCode(0),
			PrevSize(0)
		{ }
		Uint64 TimeCode;
		Uint64 PrevSize;
		std::vector<IBlockEntry*> BlockCollection;
	};
}

#endif