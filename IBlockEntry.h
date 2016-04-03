#pragma once
#ifndef I_BLOCK_ENTRY_H
#define I_BLOCK_ENTRY_H
#include "IEbmlElement.h"
#include <vector>
#include "Frame.h"

namespace MkvParser
{
	class IBlockEntry
	{
	public:
		virtual std::vector<Frame*>& GetFrames() = 0;
		virtual Uint64 GetTrackNumber() const = 0;
		virtual Int16 GetTimeCode() const = 0;
		virtual bool IsInvisible() const = 0;
		virtual bool IsKeyFrames() const = 0;
		virtual bool IsDiscardable() const = 0;
	};
}

#endif