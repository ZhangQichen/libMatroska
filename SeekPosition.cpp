#include "SeekPosition.h"

namespace MkvParser
{
	ParseResult SeekPostion::ParseFromFile()
	{
		long long result = UnserializeUInt(this->m_pReader, this->GetDataStart(), this->GetDataSize());
		if (result < 0) return E_FILE_FORMAT_INVALID;
		this->TargetPosition = (BytePostion)result; // no data loss
		return 0;
	}
}