#pragma once
#include "IEbmlElement.h"
#include <vector>
#include <string>
#ifndef TRACK_H
#define TRACK_H

namespace MkvParser
{
	class ContentEncAESSettings : public IEbmlElement
	{
	protected:
		virtual ParseResult ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id);
	public:
		virtual EbmlID GetElementID() const { return kMkvContentEncAESSettings; }
		ContentEncAESSettings(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart, Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(elementStart, elementSize, dataStart, dataSize, father, pReader),
			CipherMode(1)
		{ }
		Uint64 CipherMode; // Pre defined values: 1 - CTR
		//virtual void GenerateSerializedInfo(Uint64 start);
	};

	class ContentEncryption : public IEbmlElement
	{
	protected:
		virtual ParseResult ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id);
	public:
		ContentEncryption(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart, Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(elementStart, elementSize, dataStart, dataSize, father, pReader),
			ContentEncAlgo(0),
			pAESSettings(nullptr)
		{ }
		virtual EbmlID GetElementID() const { return MkvId::kMkvContentEncryption; }
		Uint64 ContentEncAlgo;
		std::string ContentEncKeyID;
		ContentEncAESSettings* pAESSettings;
		//virtual void GenerateSerializedInfo(Uint64 start);
	};

	class ContentEncoding : public IEbmlElement
	{
	protected:
		virtual ParseResult ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id);
	public:
		ContentEncoding(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart, Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(elementStart, elementSize, dataStart, dataSize, father, pReader),
			ContentEncodingOrder(0),
			ContentEncodingScope(1),
			ContentEncodingType(0),
			Encryption(nullptr)
		{}
		virtual EbmlID GetElementID() const { return MkvId::kMkvContentEncoding; }
		Uint64 ContentEncodingOrder;
		Uint64 ContentEncodingScope; // 0 for compression and 1 for encryption. Since Webm doesn't support ContentCompression, this should be 1.
		Uint64 ContentEncodingType;
		ContentEncryption* Encryption;
		virtual ~ContentEncoding()
		{
			if (Encryption != nullptr) delete Encryption;
		}
		//virtual void GenerateSerializedInfo(Uint64 start);
	};

	class ContentEncodings : public IEbmlElement
	{
	protected:
		virtual ParseResult ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id);
		virtual void PreClean() { ContentEncodingCollection.clear(); }
	public:
		ContentEncodings(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart, Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(elementStart, elementSize, dataStart, dataSize, father, pReader)
		{ }
		virtual EbmlID GetElementID() const { return MkvId::kMkvContentEncodings; }
		std::vector<ContentEncoding*> ContentEncodingCollection;
		//virtual void GenerateSerializedInfo(Uint64 start);
	};

	class AudioSetting : public IEbmlElement
	{
	protected:
		virtual ParseResult ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id);
		virtual void PreClean()
		{
			SamplingFrequency = 8000;
			OutputSamplingFrequency = 0.0;
			Channels = 1;
		}
		virtual void AFewMoreActions()
		{
			if (OutputSamplingFrequency <= 0.0) OutputSamplingFrequency = SamplingFrequency;
		}
	public:
		AudioSetting(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart, Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(elementStart, elementSize, dataStart, dataSize, father, pReader),
			SamplingFrequency(8000.0),
			OutputSamplingFrequency(0),
			Channels(1)
		{ }
		virtual EbmlID GetElementID() const { return MkvId::kMkvAudio; }
		double SamplingFrequency;
		double OutputSamplingFrequency; // Default value is SamplingFrequency. Init as 0 to show that this is not assigned.
		Uint64 Channels;
		Uint64 BitDepth;
		//virtual void GenerateSerializedInfo(Uint64 start);
	};

	class VideoSetting : public IEbmlElement
	{
	protected:
		virtual ParseResult ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id);
		virtual void AFewMoreActions()
		{
			if (DisplayWidth == 0)
			{
				DisplayWidth = PixelWidth - PixelCropLeft - PixelCropRight;
				if (DisplayWidth <= 0) throw std::exception();
			}
			if (DisplayHeigth == 0)
			{
				DisplayHeigth = PixelHeight - PixelCropTop - PixelCropBottom;
				if (DisplayWidth <= 0) throw std::exception();
			}
		}
	public:
		VideoSetting(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart, Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(elementStart, elementSize, dataStart, dataSize, father, pReader),
			FlagInterlaced(0),
			StereoMode(0),
			AlphaMode(0),
			PixelWidth(0),
			PixelHeight(0),
			PixelCropBottom(0),
			PixelCropTop(0),
			PixelCropLeft(0),
			PixelCropRight(0),
			DisplayWidth(0),
			DisplayHeigth(0),
			DisplayUnit(0),
			AspectRatioType(0)
		{ }
		virtual EbmlID GetElementID() const { return MkvId::kMkvVideo; }
		Uint64 FlagInterlaced; // Range: 0 - 2. Default: 0
		Uint64 StereoMode;
		Uint64 AlphaMode;
		Uint64 PixelWidth; // init as 0 to show this is unknown
		Uint64 PixelHeight; // init as 0 to show this is unknown
		Uint64 PixelCropBottom;
		Uint64 PixelCropTop;
		Uint64 PixelCropLeft;
		Uint64 PixelCropRight;
		Uint64 DisplayWidth; // init as 0 to show this is unknown
		Uint64 DisplayHeigth; // init as 0 to show this is unknown
		Uint64 DisplayUnit; // 0(pixels) is only supported
		Uint64 AspectRatioType; // 0: free resizing; 1: keep aspect ratio; 2: fixed
		//virtual void GenerateSerializedInfo(Uint64 start);
	};

	enum TrackType // 8 bits
	{
		Video = 0x01,
		Audio = 0x02,
		Complex = 0x03,
		Logo = 0x10,
		Subtitle = 0x11,
		Buttons = 0x12,
		Control = 0x20
	};

	namespace Codec_ID
	{
		const std::string VP8 = "V_VP8";
		const std::string VP9 = "V_VP9";
	}

	class TrackEntry : public IEbmlElement
	{
	protected:
		virtual ParseResult ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id);
	public:
		virtual EbmlID GetElementID() const { return MkvId::kMkvTracks; }
		TrackEntry(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart, Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(elementStart, elementSize, dataStart, dataSize, father, pReader),
			FlagEnabled(true),
			FlagDefault(true),
			FlagForced(true),
			FlagLacing(true),
			DefaultDuration(0),
			Name(""),
			Language("eng"),
			CodecID(Codec_ID::VP8),
			CodecName("VP8"),
			CodecDelay(0),
			SeekPreRoll(0),
			pVideo(nullptr),
			pAudio(nullptr),
			pContentEncodings(nullptr)
		{ }
		Uint64 TrackNumber;
		Uint64 TrackUID;
		TrackType Type; // 8 bits
		bool FlagEnabled; // if the track is used
		bool FlagDefault; // if the track SHOULD be used if no language found matched the user preference
		bool FlagForced; // if the track MUST be used during playback
		bool FlagLacing; // if the track may contain blocks using lacing
		Uint64 DefaultDuration; // number of nanoseconds(not scaled) per frame
		std::string Name; // a human readable name of this track
		std::string Language;
		std::string CodecID;
		std::string CodecPrivate;
		std::string CodecName;
		Uint64 CodecDelay;
		Uint64 SeekPreRoll; // In nanoseconds
		VideoSetting* pVideo;
		AudioSetting* pAudio;
		ContentEncodings* pContentEncodings;
		//virtual void GenerateSerializedInfo(Uint64 start);
		virtual ~TrackEntry()
		{
			if (!pVideo) delete pVideo;
			if (!pAudio) delete pAudio;
			if (!pContentEncodings) delete pContentEncodings;
		}
	};

	class Tracks : public IEbmlElement
	{
	protected:
		virtual void PreClean() { TrackEntries.clear(); }
		virtual ParseResult ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id);
	public:
		virtual EbmlID GetElementID() const { return MkvId::kMkvTracks; }
		Tracks(BytePostion elementStart, Uint64 elementSize, BytePostion dataStart, Uint64 dataSize, IEbmlElement* father, IMkvReader* pReader)
			: IEbmlElement(elementStart, elementSize, dataStart, dataSize, father, pReader)
		{ }
		std::vector<TrackEntry*> TrackEntries;
	};
}

#endif