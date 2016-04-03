// Copyright (c) 2010 The WebM project authors. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS.  All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.

#ifndef MKVREADER_HPP
#define MKVREADER_HPP

#include "IMkvReader.h" 
#include <cstdio>
#include <string>

namespace MkvParser {

class MkvReader : public IMkvReader {
 public:
  MkvReader();
  explicit MkvReader(FILE* fp);
  virtual ~MkvReader();

  int Open(const char*);
  void Close();

  // Return 0 if successful
  virtual int Read(long long position, long length, unsigned char* buffer);
  virtual int Length(long long* total, long long* available);

 private:
  MkvReader(const MkvReader&);
  MkvReader& operator=(const MkvReader&);

  // Determines the size of the file. This is called either by the constructor
  // or by the Open function depending on file ownership. Returns true on
  // success.
  // Return true if successful and file size is stored in m_length
  bool GetFileSize();

  long long m_length;
  FILE* m_file;
  bool reader_owns_file_;
};

/*
Get the length of VINT given pos in file
Params:
pos: byte position in file
len: store length in byte of Uint number
Return:
UInt number from file: successful.
negative number: fail
*/
Int64 GetUIntLength(IMkvReader*, long long, long&);

/*
Read an Integer of VINT format of EBML
Params:
pos: position in byte in file
len: if success, len is the len in byte of UInt
Return:
UInt read from file: success
< 0: error
*/
Int64 ReadUInt(IMkvReader*, long long, long&);

// Reads an EBML ID and returns it.
// An ID must at least 1 byte long, cannot exceed 4, and its value must be
// greater than 0.
// See known EBML values and EBMLMaxIDLength:
// http://www.matroska.org/technical/specs/index.html
// Returns the ID, or a value less than 0 to report an error while reading the
// ID
Int64 ReadID(IMkvReader*, long long pos, long& len);

/*
Unserialize a Uint number from file. An ebml element may contains Uint numbers in its data field(not VINT size).
Params:
pos: byte position in file
size: length in byte of the UInt number
Return:
negative number: error
Uint number: successful
*/
long long UnserializeUInt(IMkvReader*, long long pos, long long size);

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
long UnserializeFloat(IMkvReader*, long long pos, long long size, double&);

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
long UnserializeInt(IMkvReader*, long long pos, long long size,
	long long& result);

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
long UnserializeString(IMkvReader*, long long pos, long long size, std::string& str);

}  // end namespace mkvparser

#endif  // MKVREADER_HPP
