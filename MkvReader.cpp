// Copyright (c) 2010 The WebM project authors. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS.  All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.

#include "MkvReader.hpp"
#include <cmath>
#include <cassert>

namespace MkvParser {

MkvReader::MkvReader() : m_file(NULL), reader_owns_file_(true) {}

MkvReader::MkvReader(FILE* fp) : m_file(fp), reader_owns_file_(false) {
  GetFileSize();
}

MkvReader::~MkvReader() {
  if (reader_owns_file_)
    Close();
  m_file = NULL;
}

int MkvReader::Open(const char* fileName) {
  if (fileName == NULL)
    return -1;

  if (m_file)
    return -1;

#ifdef _MSC_VER
  const errno_t e = fopen_s(&m_file, fileName, "rb");

  if (e)
    return -1;  // error
#else
  m_file = fopen(fileName, "rb");

  if (m_file == NULL)
    return -1;
#endif
  return !GetFileSize();
}

bool MkvReader::GetFileSize() {
  if (m_file == NULL)
    return false;
#ifdef _MSC_VER
  int status = _fseeki64(m_file, 0L, SEEK_END); // Returns 0 if successful

  if (status)
    return false;  // error

  m_length = _ftelli64(m_file);
#else
  fseek(m_file, 0L, SEEK_END);
  m_length = ftell(m_file);
#endif
  assert(m_length >= 0);

  if (m_length < 0)
    return false;

#ifdef _MSC_VER
  status = _fseeki64(m_file, 0L, SEEK_SET);

  if (status)
    return false;  // error
#else
  fseek(m_file, 0L, SEEK_SET);
#endif

  return true;
}

void MkvReader::Close() {
  if (m_file != NULL) {
    fclose(m_file);
    m_file = NULL;
  }
}

/*
Get the length of file
Params:
total: total size of file
available: available size of file
Return:
0 if successful
negative number if failed
*/
int MkvReader::Length(long long* total, long long* available) {
  if (m_file == NULL)
    return -1;

  if (total)
    *total = m_length;

  if (available)
    *available = m_length;

  return 0;
}

/*
Return 0 if successful
*/
int MkvReader::Read(long long offset, long len, unsigned char* buffer) {
  if (m_file == NULL)
    return -1;

  if (offset < 0)
    return -1;

  if (len < 0)
    return -1;

  if (len == 0)
    return 0;

  if (offset >= m_length)
    return -1;

#ifdef _MSC_VER
  const int status = _fseeki64(m_file, offset, SEEK_SET);

  if (status)
    return -1;  // error
#else
  fseek(m_file, offset, SEEK_SET);
#endif

  const size_t size = fread(buffer, 1, len, m_file);

  if (size < size_t(len))
    return -1;  // error

  return 0;  // success
}
/*
Read an Integer of VINT format of EBML
Params:
	pos: position in byte in file
	len: if success, len is the len in byte of UInt
Return:
	UInt read from file: success
	< 0: error
*/
long long ReadUInt(IMkvReader* pReader, long long pos, long& len) {
  if (!pReader || pos < 0)
    return E_FILE_FORMAT_INVALID;

  len = 1;
  unsigned char b;
  int status = pReader->Read(pos, 1, &b);

  if (status < 0)  // error or underflow
    return status;

  if (status > 0)  // interpreted as "underflow"
    return E_BUFFER_NOT_FULL;

  if (b == 0)  // we can't handle u-int values larger than 8 bytes
    return E_FILE_FORMAT_INVALID;

  unsigned char m = 0x80; // 0x80 = 1000 0000

  while (!(b & m)) {
    m >>= 1;
    ++len;
  }

  long long result = b & (~m);
  ++pos;

  for (int i = 1; i < len; ++i) {
    status = pReader->Read(pos, 1, &b);

    if (status < 0) { // error or underflow
      len = 1;
      return status;
    }

    if (status > 0) {
      len = 1;
      return E_BUFFER_NOT_FULL;
    }

    result <<= 8;
    result |= b;

    ++pos;
  }

  return result;
}

// Reads an EBML ID and returns it.
// An ID must at least 1 byte long, cannot exceed 4, and its value must be
// greater than 0.
// See known EBML values and EBMLMaxIDLength:
// http://www.matroska.org/technical/specs/index.html
// Returns the ID, or a value less than 0 to report an error while reading the
// ID.
long long ReadID(IMkvReader* pReader, long long pos, long& len) {
  if (pReader == NULL || pos < 0)
    return E_FILE_FORMAT_INVALID;

  // Read the first byte. The length in bytes of the ID is determined by
  // finding the first set bit in the first byte of the ID.
  unsigned char temp_byte = 0;
  int read_status = pReader->Read(pos, 1, &temp_byte);

  if (read_status < 0)
    return E_FILE_FORMAT_INVALID;
  else if (read_status > 0)  // No data to read.
    return E_BUFFER_NOT_FULL;

  if (temp_byte == 0)  // ID length > 8 bytes; invalid file.
    return E_FILE_FORMAT_INVALID;

  int bit_pos = 0;
  const int kMaxIdLengthInBytes = 4;
  const int kCheckByte = 0x80;

  // Find the first bit that's set.
  bool found_bit = false;
  for (; bit_pos < kMaxIdLengthInBytes; ++bit_pos) {
    if ((kCheckByte >> bit_pos) & temp_byte) {
      found_bit = true;
      break;
    }
  }

  if (!found_bit) {
    // The value is too large to be a valid ID.
    return E_FILE_FORMAT_INVALID;
  }

  // Read the remaining bytes of the ID (if any).
  const int id_length = bit_pos + 1;
  long long ebml_id = temp_byte;
  for (int i = 1; i < id_length; ++i) {
    ebml_id <<= 8;
    read_status = pReader->Read(pos + i, 1, &temp_byte);

    if (read_status < 0)
      return E_FILE_FORMAT_INVALID;
    else if (read_status > 0)
      return E_BUFFER_NOT_FULL;

    ebml_id |= temp_byte;
  }

  len = id_length;
  return ebml_id;
}

/*
Get the length of VINT given pos in file
Params:
	pReader: 
	pos: byte position in file
	len: store length in byte of Uint number
Return:
	UInt number from file: successful.
	negative number: fail
*/
long long GetUIntLength(IMkvReader* pReader, long long pos, long& len) {
  if (!pReader || pos < 0)
    return E_FILE_FORMAT_INVALID;

  long long total, available;

  int status = pReader->Length(&total, &available);
  if (status < 0 || (total >= 0 && available > total))
    return E_FILE_FORMAT_INVALID;

  len = 1;

  if (pos >= available)
    return pos;  // too few bytes available

  unsigned char b;

  status = pReader->Read(pos, 1, &b);

  if (status != 0)
    return status;

  if (b == 0)  // we can't handle u-int values larger than 8 bytes
    return E_FILE_FORMAT_INVALID;

  unsigned char m = 0x80;

  while (!(b & m)) {
    m >>= 1;
    ++len;
  }

  return 0;  // success
}

/*
Unserialize a Uint number from file. An ebml element may contains Uint numbers in its data field(not VINT size).
Params:
pos: byte position in file
size: length in byte of the UInt number
Return:
negative number: error
Uint number: successful
*/
long long UnserializeUInt(IMkvReader* pReader, long long pos, long long size) {
	if (!pReader || pos < 0 || (size <= 0) || (size > 8))
		return E_FILE_FORMAT_INVALID;

	long long result = 0;

	for (long long i = 0; i < size; ++i) {
		unsigned char b;

		const long status = pReader->Read(pos, 1, &b);

		if (status < 0)
			return status;

		result <<= 8;
		result |= b;

		++pos;
	}

	return result;
}

/*
Unserialize a float number from file. An ebml element may contains float numbers in its data field(not VINT size).
Params:
pos: byte position in file
size_: length in byte of the float number. can only be 0, 4 or 8.
result: store float number read from file if successful
Return:
negative number: error
0: successful
*/
long UnserializeFloat(IMkvReader* pReader, long long pos, long long size_,
	double& result) {
	if (!pReader || pos < 0 || ((size_ != 4) && (size_ != 8)))
		return E_FILE_FORMAT_INVALID;

	const long size = static_cast<long>(size_);

	unsigned char buf[8];

	const int status = pReader->Read(pos, size, buf);

	if (status < 0)  // error
		return status;

	if (size == 4) { // float in EBML can be 0 octets, 4 octets or 8 octets
		union {
			float f;
			unsigned long ff;
		};

		ff = 0;

		for (int i = 0;;) {
			ff |= buf[i];

			if (++i >= 4)
				break;

			ff <<= 8; // Endianness of Float is Big-endian
		}

		result = f;
	}
	else {
		union {
			double d;
			unsigned long long dd;
		};

		dd = 0;

		for (int i = 0;;) {
			dd |= buf[i];

			if (++i >= 8)
				break;

			dd <<= 8;
		}

		result = d;
	}

	if (std::isinf(result) || std::isnan(result))
		return E_FILE_FORMAT_INVALID;

	return 0;
}

/*
Unserialize a int number from file. An ebml element may contains int numbers in its data field(not VINT size).
Params:
pos: byte position in file
size: length in byte of the Int number
result_ref: store int read from file if successful
Return:
negative number: error
0: successful
*/
long UnserializeInt(IMkvReader* pReader, long long pos, long long size,
	long long& result_ref) {
	if (!pReader || pos < 0 || size < 1 || size > 8)
		return E_FILE_FORMAT_INVALID;

	signed char first_byte = 0;
	const long status = pReader->Read(pos, 1, (unsigned char*)&first_byte);

	if (status < 0)
		return status;

	unsigned long long result = first_byte;
	++pos;

	for (long i = 1; i < size; ++i) {
		unsigned char b;

		const long status = pReader->Read(pos, 1, &b);

		if (status < 0)
			return status;

		result <<= 8;
		result |= b;

		++pos;
	}

	result_ref = static_cast<long long>(result);
	return 0;
}

/*
Unserialize a string from file. An ebml element may contains string in its data field(not VINT size).
Params:
pos: byte position in file
size: length in byte of the string
str: pointer to the string read from file
Return:
negative number: error
0: successful
*/
long UnserializeString(IMkvReader* pReader, long long pos, long long size,
	std::string& str) {
	str = "";

	if (size >= LONG_MAX || size < 0)
		return E_FILE_FORMAT_INVALID;

	// +1 for '\0' terminator
	const long required_size = static_cast<long>(size) + 1;

	Byte* const buffer = new Byte[required_size];

	const long status = pReader->Read(pos, static_cast<long>(size), buffer);

	if (status) { // Fail
		delete[] buffer;
		str = "";
		return status;
	}
	buffer[size] = 0;
	str = std::string(reinterpret_cast<char*>(buffer));
	delete[] buffer;
	return 0;
}

}  // end namespace mkvparser