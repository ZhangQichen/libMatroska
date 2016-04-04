#include "BlockGroup.h"

namespace MkvParser
{
	ParseResult BlockGroup::ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id)
	{
		ParseResult status = -100;
		if (id == MkvId::kMkvBlock)
		{
			if (m_pBlock != nullptr) return FAILED;
			Block* pBlock = new Block(e_start, e_size, d_start, d_size, this, this->m_pReader);
			status = pBlock->ParseFromFile();
			if (status != SUCCESS) return status;
		}
		else if (id == MkvId::kMkvBlockDuration)
		{
			if (BlockDuration >= 0) // Has been initialized
				return E_FILE_FORMAT_INVALID;
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->BlockDuration = status;
		}
		else if (id == MkvId::kMkvDiscardPadding)
		{
			if (DiscardPadding >= 0) // Has been initialized
				return E_FILE_FORMAT_INVALID;
			Int64 result = -100;
			status = UnserializeInt(this->m_pReader, d_start, d_size, result);
			if (status != 0) return E_FILE_FORMAT_INVALID;
			this->DiscardPadding = result;
		}
		else // Ignore other elements
		{
			return SUCCESS;
		}
	}
	ParseResult BlockGroup::ParseFromFile()
	{
		BytePostion cur_pos = this->GetDataStart();
		BytePostion stop_pos = this->GetDataSize() + cur_pos;
		if (cur_pos > stop_pos) return E_FILE_FORMAT_INVALID;
		if (this->m_pReader == nullptr) return FAILED;
		if (this->m_pBlock != nullptr)
		{
			delete m_pBlock;
			m_pBlock = nullptr;
		}
		while (cur_pos < stop_pos)
		{
			long length = -1;
			Int64 status = ReadID(this->m_pReader, cur_pos, length);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			EbmlID e_id = status;
			BytePostion e_start = cur_pos;
			cur_pos += length; // consumes id
			if (cur_pos >= stop_pos) return E_FILE_FORMAT_INVALID;
			status = ReadUInt(this->m_pReader, cur_pos, length);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			Uint64 d_size = status;
			cur_pos += length; // consumes data size
			if ((cur_pos >= stop_pos) || (cur_pos + d_size > stop_pos))
				return E_FILE_FORMAT_INVALID;
			if (e_id == MkvId::kMkvBlock)
			{
				if (m_pBlock != nullptr) return FAILED;
				Block* pBlock = new Block(e_start, cur_pos + d_size - e_start, cur_pos, d_size, this, this->m_pReader);
				status = pBlock->ParseFromFile();
				if (status != SUCCESS) return status;
				cur_pos += d_size; // consumes data
			}
			else if (e_id == MkvId::kMkvBlockDuration)
			{
				if (BlockDuration >= 0) // Has been initialized
					return E_FILE_FORMAT_INVALID;
				status = UnserializeUInt(this->m_pReader, cur_pos, d_size);
				if (status < 0) return E_FILE_FORMAT_INVALID;
				this->BlockDuration = status;
				cur_pos += d_size;
			}
			else if (e_id == MkvId::kMkvDiscardPadding)
			{
				if (DiscardPadding >= 0) // Has been initialized
					return E_FILE_FORMAT_INVALID;
				Int64 result = -100;
				status = UnserializeInt(this->m_pReader, cur_pos, d_size, result);
				if (status != 0) return E_FILE_FORMAT_INVALID;
				this->DiscardPadding = result;
				cur_pos += d_size;
			}
			else // Ignore other elements
			{
				cur_pos += d_size;
			}
		}
		if (cur_pos != stop_pos) return E_FILE_FORMAT_INVALID;
		return SUCCESS;
	}
}