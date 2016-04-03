#pragma once
#ifndef IEBML_ELEMENT_H
#define IEBML_ELEMENT_H

#include "MkvReader.hpp"
#include "Webmids.hpp"
#include <set>
#include "assert.h"

namespace MkvParser
{
	class IEbmlElement
	{
	protected:
		BytePostion m_element_start;
		BytePostion m_element_size;
		BytePostion m_start_data;
		BytePostion m_size_data;
		IMkvReader* m_pReader;
		IEbmlElement* m_pFather;
		/*
		Parse a child ebml element from file,
		Params:
		e_start: start of the ebml id of the child element
		e_size: size of the entire child
		d_start: start of the data field of the child
		d_size: size of the data of the child
		id: Ebml id of the Child
		Retuen:
		in IMkvReader.h
		*/
		virtual ParseResult ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id) {}
		/*
		After parsing this element, some acions may be taken to fill each attribute.
		*/
		virtual void AFewMoreActions() { }
		virtual void PreClean() { }
	public:
		IEbmlElement(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart,Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader)
			: m_element_start(elementStart), m_element_size(elementSize), m_size_data(dataSize), m_start_data(dataStart), m_pFather(father), m_pReader(pReader)
		{
			assert((elementStart + elementSize) == (dataSize + dataStart));
		}
		long long GetDataSize() const { return m_size_data; }
		long long GetDataStart() const { return m_start_data; }
		long long GetElementSize() const { return m_element_size; }
		long long GetElementStart() const { return m_element_start; }
		virtual EbmlID GetElementID() const = 0;
		/*
		Return:
		E_FILE_FORMAT_INVALID
		E_BUFFER_NOT_FULL
		SUCCESS
		FAILED
		*/
		virtual ParseResult ParseFromFile()
		{
			PreClean();
			if (this->m_pReader == nullptr) return FAILED;
			BytePostion cur_pos = this->GetDataStart();
			BytePostion stop_pos = this->GetDataSize();
			if (cur_pos > stop_pos) return E_FILE_FORMAT_INVALID;
			while (cur_pos < stop_pos)
			{
				Uint64 status;
				long length = -1;
				BytePostion e_start = cur_pos;
				// Read ID
				status = ReadID(this->m_pReader, cur_pos, length);
				if (status < 0) return E_FILE_FORMAT_INVALID;
				if (!CheckIDSize(length)) return E_FILE_FORMAT_INVALID;
				cur_pos += length; // Consumes ID
				EbmlID id = (EbmlID)status;
				// Read DataSize
				status = ReadUInt(this->m_pReader, cur_pos, length);
				if (status < 0) return E_FILE_FORMAT_INVALID;
				if (!CheckElementSize(length)) return E_FILE_FORMAT_INVALID;
				cur_pos += length; // Consumes Size
				Uint64 d_size = status;
				// Read Data
				cur_pos += d_size;// Consumes data
				status = ParseChild(e_start, cur_pos + d_size - e_start, cur_pos, d_size, id);
				if (status != SUCCESS) return status;
				if (cur_pos > stop_pos) return E_FILE_FORMAT_INVALID;
			}
			if (cur_pos != stop_pos) return E_FILE_FORMAT_INVALID;
			AFewMoreActions();
			return SUCCESS;
		}

		virtual ~IEbmlElement() { }
	};
}

#endif