#include "IEbmlElement.h"
//#include <iostream>

namespace MkvParser
{
	Uint64 IEbmlElement::sMaxIDLength = 0;
	Uint64 IEbmlElement::sMaxSizeLength = 0;

	ParseResult IEbmlElement::ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id)
	{
		return SUCCESS;
	}

	void IEbmlElement::AFewMoreActions() { }

	void IEbmlElement::PreClean() { }

	ParseResult IEbmlElement::ParseFromFile()
	{
		//std::cout << "On parsing IEbmlElement from file\n";
		PreClean();
		if (this->m_pReader == nullptr) return FAILED;
		long long total, available;
		m_pReader->Length(&total, &available);
		if (total > available) return E_FILE_FORMAT_INVALID;
		BytePostion cur_pos = this->GetDataStart();
		BytePostion stop_pos = this->GetDataSize() + cur_pos;
		if (stop_pos > total) return E_FILE_FORMAT_INVALID;
		if (cur_pos > stop_pos) return E_FILE_FORMAT_INVALID;
		while (cur_pos < stop_pos)
		{

			//std::cout << "e_start: " << cur_pos << std::endl;

			Uint64 status;
			long length = -1;
			BytePostion e_start = cur_pos;
			// Read ID
			status = ReadID(this->m_pReader, cur_pos, length);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			if (!CheckIDLength(length)) return E_FILE_FORMAT_INVALID;
			cur_pos += length; // Consumes ID
			EbmlID id = (EbmlID)status;

			//std::cout << "ID: " << id << std::endl;

			// Read DataSize
			status = ReadUInt(this->m_pReader, cur_pos, length);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			if (!CheckElementSize(length)) return E_FILE_FORMAT_INVALID;

			//std::cout << "d_size: " << status << std::endl;

			cur_pos += length; // Consumes Size
			Uint64 d_size = status;
			// Read Data

			//std::cout << "d_start: " << cur_pos << std::endl;
			//std::cout << std::endl;

			status = ParseChild(e_start, cur_pos + d_size - e_start, cur_pos, d_size, id);
			if (status != SUCCESS) return status;
			if (cur_pos > stop_pos) return E_FILE_FORMAT_INVALID;
			cur_pos += d_size;
		}
		if (cur_pos != stop_pos) return E_FILE_FORMAT_INVALID;
		AFewMoreActions();
		return SUCCESS;
	}

	IEbmlElement::~IEbmlElement()
	{

	}


}