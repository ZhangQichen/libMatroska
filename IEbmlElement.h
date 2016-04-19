#pragma once
#include "MkvReader.hpp"
#include "Webmids.hpp"
#include <set>
#include "Types.h"
#include "Serialization.h"
#ifndef IEBML_ELEMENT_H
#define IEBML_ELEMENT_H

namespace MkvParser
{
	class IEbmlElement
	{
	private:
		static Uint64 sMaxIDLength;
		static Uint64 sMaxSizeLength;

	protected:
		BytePostion m_element_start;
		BytePostion m_element_size;
		BytePostion m_start_data;
		BytePostion m_size_data;
		IMkvReader* m_pReader;
		IEbmlElement* m_pFather;

		/*
		Check the range of ID according to EBML header
		params:
		IDSize: ID's size in byte number
		Return:
		true: within ID range
		false: ...
		*/
		bool CheckIDLength(long IDSize) // Size in Byte number
		{
			return !(IDSize < 0 || IDSize >(!sMaxIDLength ? 4 : sMaxIDLength));
		}
		/*
		Check the range of Size field according to EBML header
		Params:
		sizeLen: size's byte number
		Return:
		true: within size range
		false: otherwise
		*/
		bool CheckElementSize(long sizeLen)
		{
			return !(sizeLen < 0 || sizeLen >(!sMaxSizeLength ? 8 : sMaxSizeLength));
		}
		
		void SetMaxIDLength(Uint64 value)
		{
			sMaxIDLength = value;
		}

		void SetMaxSizeLength(Uint64 value)
		{
			sMaxSizeLength = value;
		}

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
		virtual ParseResult ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id);
		/*
		After parsing this element, some acions may be taken to fill each attribute.
		*/
		virtual void AFewMoreActions();
		virtual void PreClean();
	
	public:
		IEbmlElement(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart, Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader);
		SerializedInfo SerializedInformation;
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
		virtual ParseResult ParseFromFile();

		//virtual void GenerateSerializedInfo(Uint64 start) = 0;

		virtual ~IEbmlElement();
	};
}

#endif