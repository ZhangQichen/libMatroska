#include "Serialization.h"

namespace MkvParser
{
	/*
	Copy src of little-endianness to dst of big-endianness.
	That is the byte in LE_src should have exactly opposite order in BE_dst
	*/
	void CopyBuffer(const Byte* const LE_src, Byte*& BE_dst, Uint64 length)
	{
		//if (!LE_src) return; // if LE_src is all zeors, !LE_src will be true.
		if (BE_dst != nullptr) delete BE_dst;
		BE_dst = new Byte[length + 1]; // padding 0 at the end
		BE_dst[length] = 0;
		size_t i = 0;
		size_t j = length - 1;
		while (i < length)
		{
			*(BE_dst + j) = *(LE_src + i);
			++i;
			--j;
		}
		return;
	}

	/*
	Serialize an Ebml ID.
	Params:
	id: id to serialize
	buffer: store serialized bytes of id
	return:
		length of serialized data in bytes if successfully
		0 if failed
	*/
	Uint64 SerializeEbmlID(EbmlID id, Byte*& buffer)
	{
		Uint64 length = 0;
		Uint64 mask = 0xFF;
		short MaxLen = sizeof(Uint64);
		while (length <= MaxLen)
		{
			Uint64 tmp = id & mask;
			if (tmp == 0) break;
			mask <<= 8; // shift 1 byte
			++length;
		}
		if (length > MaxLen) return 0; // error
		Byte* ptr = reinterpret_cast<Byte*>(&id);
		/*
		It is little-endianness in Windows so ptr now points to the most-least byte.
		*/
		CopyBuffer(ptr, buffer, length);
		return length;
	}

	/*
	Serialize an Ebml data size.
	Params:
	id: id to serialize
	buffer: store serialized bytes of id
	return length of serialized data in bytes
	*/
	Uint64 SerializeEbmlSize(Uint64 size, Byte*& buffer)
	{
		Uint64 vint_width = 0;
		Uint64 maxLen = sizeof(Uint64); //
		Uint64 vint_mark = 0;
		while (vint_width < maxLen)
		{
			vint_mark = 1LL << (vint_width + 1) * 7;
			if (size < vint_mark) break;
			++vint_width;
		}
		if (vint_width >= maxLen) return 0;
		size |= vint_mark;
		CopyBuffer((Byte*)&size, buffer, vint_width + 1);
		return vint_width + 1;
	}

	/*
	Serialize an float.
	Params:
	data: data to serialize	
	buffer: store serialized bytes of data
	return length of serialized data in bytes
	*/
	Uint64 SerializeFloat(double data, Byte*& buffer)
	{
		union
		{
			double f_data;
			Uint64 uint_data;
		};
		f_data = data;
		// 4 bytes
		Uint64 dataSize = 0;
		if ((uint_data & 0x00000000LL) == 0) dataSize = 4;
		else dataSize = 8;
		CopyBuffer((Byte*)&f_data, buffer, dataSize);
		return dataSize;
	}

	/*
	Serialize a Uint.
	Params:
	data: data to serialize
	dataSize: length which the serialized data should have (read from dataSize field of ebml element)
	buffer: store serialized bytes of data
	return length of serialized data in bytes if successful and 0 otherwise
	*/
	Uint64 SerializeUint(Uint64 data, Byte*& buffer)
	{
		Byte* ptr = (Byte*)&data;
		Uint64 dataSize = 1;
		Uint64 maxLen = sizeof(Uint64);
		Uint64 mask = 0xFFLL;
		while (dataSize <= maxLen)
		{
			if ((mask & data) == 0) break;
			mask <<= 8;
			++dataSize;
		}
		if (dataSize > maxLen) return 0;
		CopyBuffer(ptr, buffer, dataSize);
		return dataSize;
	}

	/*
	Serialize a string.
	Params:
	data: data to serialize
	dataSize: length which the serialized data should have (read from dataSize field of ebml element)
	buffer: store serialized bytes of data
	return length of serialized data in bytes if successful and 0 otherwise
	*/
	Uint64 SerializeString(std::string data, Uint64 dataSize, Byte*& buffer)
	{
		CopyBuffer((Byte*)data.c_str(), buffer, dataSize);
		if (buffer || buffer != nullptr) delete buffer;
		buffer = new Byte[dataSize + 1];
		buffer[dataSize] = 0;
		for (int i = 0; i < dataSize; ++i)
		{
			buffer[i] = data[i];
		}
		return dataSize;
	}

	Uint64 GetSerializedEbmlSizeLength(Uint64 data)
	{
		Byte* p = nullptr;
		Uint64 size = SerializeEbmlSize(data, p);
		if (p != nullptr) delete p;
		return size;
	}
	Uint64 GetSerializedEbmlIDSize(EbmlID data)
	{
		Byte* p = nullptr;
		Uint64 size = SerializeEbmlID(data, p);
		if (p != nullptr) delete p;
		return size;
	}
	Uint64 GetSerializedFloatSize(double data)
	{
		Byte* p = nullptr;
		Uint64 size = SerializeFloat(data, p);
		if (p != nullptr) delete p;
		return size;
	}

	Uint64 GetSerializedUintSize(Uint64 data)
	{
		Byte*p = nullptr;
		Uint64 size = SerializeUint(data, p);
		if (p != nullptr || p != NULL) delete p;
		return size;
	}

	Uint64 GetSerializedStringSize(std::string data)
	{
		Byte* p = nullptr;
		Uint64 size = SerializeString(data, data.size(), p);
		if (p != nullptr) delete p;
		return size;
	}
}