#pragma once
#include "Types.h"
#include <string>
#ifndef SERIALIZATION_H
#define SERIALIZATION_H

namespace MkvParser
{
	/* Describe position of each element after serialized.
	Some elements in segment reference other elements' position, such as Cues and SeekHead,
	so we first generate serialized info for each element to make each element right.
	*/
	struct SerializedInfo
	{
		BytePostion Abs_ElementStart; // Absolute position of elemen		
		BytePostion Abs_DataStart; // Absolute position of element's data
		Uint64 DataSize; // size of data
		Uint64 ElementSize; // ebml ID + size + data
	};

	/*
	Copy src of little-endianness to dst of big-endianness.
	That is the byte in LE_src should have exactly opposite order in BE_dst
	*/
	void CopyBuffer(const Byte* const src, Byte*& dst, Uint64 length);

	/*
	Serialize an Ebml ID.
	Params:
	id: id to serialize
	buffer: store serialized bytes of id
	return:
	length of serialized data in bytes if successfully
	0 if failed
	*/
	Uint64 SerializeEbmlID(EbmlID id, Byte*& buffer);

	/*
	Serialize an Ebml ID.
	Params:
	id: id to serialize
	buffer: store serialized bytes of id
	return length of serialized data in bytes
	*/
	Uint64 SerializeEbmlSize(Uint64 size, Byte*& buffer);

	/*
	Serialize an float.
	Params:
	data: data to serialize
	dataSize: length which the serialized data should have (read from dataSize field of ebml element)
	buffer: store serialized bytes of data
	return length of serialized data in bytes
	*/
	Uint64 SerializeFloat(double data,Byte*& buffer);

	/*
	Serialize a Uint.
	Params:
	data: data to serialize
	dataSize: length which the serialized data should have (read from dataSize field of ebml element)
	buffer: store serialized bytes of data
	return length of serialized data in bytes
	*/
	Uint64 SerializeUint(Uint64 data, Byte*& buffer);

	/*
	Serialize a string.
	Params:
	data: data to serialize
	dataSize: length which the serialized data should have (read from dataSize field of ebml element)
	buffer: store serialized bytes of data
	return length of serialized data in bytes
	*/
	Uint64 SerializeString(std::string data, Uint64 dataSize, Byte*& buffer);

	Uint64 GetSerializedEbmlSizeLength(Uint64);
	Uint64 GetSerializedEbmlIDSize(EbmlID);
	Uint64 GetSerializedFloatSize(double);
	Uint64 GetSerializedUintSize(Uint64);
	Uint64 GetSerializedStringSize(std::string);
}

#endif