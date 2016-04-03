#include "SeekID.h"
#include "Global.h"

namespace MkvParser
{
	ParseResult SeekID::ParseFromFile()
	{
		long length = -110;
		GetUIntLength(this->m_pReader, this->GetDataStart, length);
		if (length <= 0 || length > (gEbmlHeader == nullptr ? 4 : gEbmlHeader->EbmlMaxIDLength))
		{
			return E_FILE_FORMAT_INVALID;
		}
		if ((this->RefencedID = ReadID(this->m_pReader, this->GetDataStart, length)) < 0)
		{
			return E_FILE_FORMAT_INVALID;
		}
		return 0;
	}
}