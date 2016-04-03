#include "Cues.h"

namespace MkvParser
{
	ParseResult CueTrackPositions::ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id)
	{
		ParseResult status = -100;
		if (id == MkvId::kMkvCueTrack)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status <= 0) return E_FILE_FORMAT_INVALID;
			this->CueTrack = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvCueClusterPosition)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->CueClusterPosition = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvCueBlockNumber)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status <= 0) return E_FILE_FORMAT_INVALID;
			this->CueBlockNumber = status;
			return SUCCESS;
		}
		else return SUCCESS;
	}

	ParseResult CuePoint::ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id)
	{
		ParseResult status = -100;
		if (id == MkvId::kMkvCueTime)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->CueTime = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvCueTrackPositions)
		{
			CueTrackPositions* p = new CueTrackPositions(e_start, e_size, d_start, d_size, this, this->m_pReader);
			status = p->ParseFromFile();
			if (status != SUCCESS) delete p;
			else CueTrackPositionCollection.push_back(p);
			return status;
		}
		else return SUCCESS;
	}

	ParseResult Cues::ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id)
	{
		ParseResult status = -100;
		if (id == MkvId::kMkvCuePoint)
		{
			CuePoint* p = new CuePoint(e_start, e_size, d_start, d_size, this, this->m_pReader);
			status = p->ParseFromFile();
			if (status != SUCCESS) delete p;
			else CuePointCollection.push_back(p);
			return status;
		}
		else return SUCCESS;
	}
}