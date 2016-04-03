#include "Cluster.h"
#include "Global.h"
#include "SimpleBlock.h"
#include "BlockGroup.h"

namespace MkvParser
{
	ParseResult Cluster::ParseFromFile()
	{
		BytePostion cur_pos = this->GetDataStart();
		BytePostion stop_pos = this->GetDataSize() + cur_pos;
		while (cur_pos < stop_pos)
		{
			BytePostion e_start = cur_pos;
			long length;
			Int64 status = ReadID(this->m_pReader, cur_pos, length);
			if (!CheckIDSize(length)) return E_FILE_FORMAT_INVALID;
			if (status < 0) return E_FILE_FORMAT_INVALID;
			EbmlID id = status;
			cur_pos += length; // consumes ID
			if (cur_pos >= stop_pos) return E_FILE_FORMAT_INVALID;
			status = ReadUInt(this->m_pReader, cur_pos, length);
			if (!CheckElementSize(length)) return E_FILE_FORMAT_INVALID;
			if (status < 0) return E_FILE_FORMAT_INVALID;
			Uint64 d_size = status;
			cur_pos += length; // consumes size
			if (cur_pos + d_size > stop_pos) return E_FILE_FORMAT_INVALID;
			if (cur_pos >= stop_pos) return E_FILE_FORMAT_INVALID;
			if (id == MkvId::kMkvTimecode)
			{
				status = UnserializeUInt(this->m_pReader, cur_pos, d_size);
				if (status < 0) return E_FILE_FORMAT_INVALID;
				this->TimeCode = status;
				cur_pos += d_size; // consumes data
			}
			else if (id == MkvId::kMkvPrevSize)
			{
				status = UnserializeUInt(this->m_pReader, cur_pos, d_size);
				if (status < 0) return E_FILE_FORMAT_INVALID;
				this->PrevSize = status;
				cur_pos += d_size; // consumes data
			}
			else if (id == MkvId::kMkvSimpleBlock)
			{
				SimpleBlock* block = new SimpleBlock(e_start, cur_pos + d_size - e_start, cur_pos, d_size, this, this->m_pReader);
				status =  block->ParseFromFile();
				if (status != SUCCESS) return E_FILE_FORMAT_INVALID;
				IBlockEntry* entry = block;
				this->BlockCollection.push_back(block);
				cur_pos += d_size; // consumes data
			}
			else if (id == MkvId::kMkvBlockGroup)
			{
				BlockGroup* block = new BlockGroup(e_start, cur_pos + d_size - e_start, cur_pos, d_size, this, this->m_pReader);
				status = block->ParseFromFile();
				if (status != SUCCESS) return E_FILE_FORMAT_INVALID;
				IBlockEntry* entry = block;
				this->BlockCollection.push_back(entry);
				cur_pos += d_size;
			}
			else { cur_pos += d_size; }
		}
		if (cur_pos != stop_pos) return E_FILE_FORMAT_INVALID;
		return SUCCESS;
	}
}