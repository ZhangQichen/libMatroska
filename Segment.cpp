#include "Segment.h"

namespace MkvParser
{
	ParseResult SegmentElement::ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id)
	{
		ParseResult status = -100;
		if (id == MkvId::kMkvInfo)
		{
			this->pSegInfo = new SegmentInfo(e_start, e_size, d_start, d_size, this, this->m_pReader);
			status = pSegInfo->ParseFromFile();
			if (status != SUCCESS) delete pSegInfo;
			return status;
		}
		else if (id == MkvId::kMkvSeekHead)
		{
			this->pSeekHead = new SeekHead(e_start, e_size, d_start, d_size, this, this->m_pReader);
			status = pSeekHead->ParseFromFile();
			if (status != SUCCESS) delete pSeekHead;
			return status;
		}
		else if (id == MkvId::kMkvCluster)
		{
			Cluster* p = new Cluster(e_start, e_size, d_start, d_size, this, this->m_pReader);
			status = p->ParseFromFile();
			if (status != SUCCESS) delete p;
			else this->ClusterCollection.push_back(p);
			return status;
		}
		else if (id == MkvId::kMkvTracks)
		{
			pTracks = new Tracks(e_start, e_size, d_start, d_size, this, this->m_pReader);
			status = pTracks->ParseFromFile();
			if (status != SUCCESS) delete pTracks;
			return status;
		}
		else if (id == MkvId::kMkvCues)
		{
			pCues = new Cues(e_start, e_size, d_start, d_size, this, this->m_pReader);
			status = pCues->ParseFromFile();
			if (status != SUCCESS) delete pCues;
			return status;
		}
		else return SUCCESS;
	}
}