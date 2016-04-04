#include "Cluster.h"
#include "SimpleBlock.h"
#include "BlockGroup.h"
#include "IEbmlElement.h"

namespace MkvParser
{
	ParseResult Cluster::ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id)
	{
		ParseResult status = -100;
		if (id == MkvId::kMkvTimecode)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->TimeCode = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvPrevSize)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->PrevSize = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvSimpleBlock)
		{
			SimpleBlock* block = new SimpleBlock(e_start, e_size, d_start, d_size, this, this->m_pReader);
			status = block->ParseFromFile();
			if (status != SUCCESS) return E_FILE_FORMAT_INVALID;
			IBlockEntry* entry = block;
			this->BlockCollection.push_back(block);
			return SUCCESS;
		}
		else if (id == MkvId::kMkvBlockGroup)
		{
			BlockGroup* block = new BlockGroup(e_start, e_size, d_start, d_size, this, this->m_pReader);
			status = block->ParseFromFile();
			if (status != SUCCESS) return E_FILE_FORMAT_INVALID;
			IBlockEntry* entry = block;
			this->BlockCollection.push_back(entry);
			return SUCCESS;
		}
		else { return SUCCESS; }
	}
}