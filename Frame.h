#pragma once
#ifndef FRAME_H
#define FRAME_H

namespace MkvParser
{
	class Frame
	{
	public:
		BytePostion Position; // Position in file
		Uint64 Length; // Length in byte
	};
}

#endif