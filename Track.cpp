#include "Track.h"
#include "Global.h"
namespace MkvParser
{
	ParseResult ContentEncAESSettings::ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id)
	{
		if (id == MkvId::kMkvAESSettingsCipherMode)
		{
			ParseResult status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status != SUCCESS) return status;
			if (status != 1) return E_FILE_FORMAT_INVALID; // Webm doesn't support cipher mode other than CTR(1).
			this->CipherMode = 1;
			return SUCCESS;
		}
		return SUCCESS;
	}

	ParseResult ContentEncryption::ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id)
	{
		if (id == MkvId::kMkvContentEncAlgo)
		{
			ParseResult status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			if (status != 5) return E_FILE_FORMAT_INVALID; // Webm only supports 5(AES)
			this->ContentEncAlgo = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvContentEncKeyID)
		{
			ParseResult status = UnserializeString(this->m_pReader, d_start, d_size, this->ContentEncKeyID);
			if (status < 0)
			{
				this->ContentEncKeyID = "";
				return E_FILE_FORMAT_INVALID;
			}
			return SUCCESS;
		}
		else if (id == MkvId::kMkvContentEncAESSettings)
		{
			if (this->pAESSettings != nullptr) delete pAESSettings;
			pAESSettings = new ContentEncAESSettings(e_start, e_size, d_start, d_size, this, this->m_pReader);
			ParseResult status = pAESSettings->ParseFromFile();
			if (status != SUCCESS) delete pAESSettings;
			return status;
		}
		return SUCCESS;
	}

	ParseResult ContentEncoding::ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id)
	{
		if (id == MkvId::kMkvContentEncodingOrder)
		{
			ParseResult status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->ContentEncodingOrder = (Uint64)status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvContentEncodingScope)
		{
			ParseResult status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			if (status == 0) return E_FILE_FORMAT_INVALID; // ContentEncodingScope cannot be 0
			this->ContentEncodingScope = (Uint64)status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvContentEncodingType)
		{
			ParseResult status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->ContentEncodingScope = (Uint64)status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvContentEncryption)
		{
			if (this->Encryption != nullptr) delete Encryption;
			this->Encryption = new ContentEncryption(e_start, e_size, d_start, d_size, this, this->m_pReader);
			ParseResult status = this->Encryption->ParseFromFile();
			if (status != SUCCESS)
				delete this->Encryption;
			return status;
		}
		else return SUCCESS;
	}

	ParseResult ContentEncodings::ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id)
	{
		this->ContentEncodingCollection;
		if (id == MkvId::kMkvContentEncoding)
		{
			ContentEncoding* p = new ContentEncoding(e_start, e_size, d_start, d_size, this, this->m_pReader);
			ParseResult status = p->ParseFromFile();
			if (status != SUCCESS) delete p;
			else this->ContentEncodingCollection.push_back(p);
			return status;
		}
		else return SUCCESS;
	}

	ParseResult AudioSetting::ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id)
	{
		ParseResult status;
		if (id == MkvId::kMkvSamplingFrequency)
		{
			double result = 0.0;
			status = UnserializeFloat(this->m_pReader, d_start, d_size, result);
			if (status != 0) return E_FILE_FORMAT_INVALID;
			if (result <= 0) return E_FILE_FORMAT_INVALID;
			this->SamplingFrequency = result;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvOutputSamplingFrequency)
		{
			double result = 0.0;
			status = UnserializeFloat(this->m_pReader, d_start, d_size, result);
			if (status != 0) return E_FILE_FORMAT_INVALID;
			if (result <= 0) return E_FILE_FORMAT_INVALID;
			this->OutputSamplingFrequency = result;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvChannels)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status <= 0) return E_FILE_FORMAT_INVALID;
			this->Channels = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvBitDepth)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status <= 0) return E_FILE_FORMAT_INVALID;
			this->BitDepth = status;
			return SUCCESS;
		}
	}

	ParseResult VideoSetting::ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id)
	{
		ParseResult status = -111;
		if (id == MkvId::kMkvFlagInterlaced)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0 || status > 2) return E_FILE_FORMAT_INVALID;
			this->FlagInterlaced = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvStereoMode)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0 || (status > 3 && status != 11)) return E_FILE_FORMAT_INVALID;
			this->StereoMode = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvAlphaMode)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->AlphaMode = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvPixelWidth)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status <= 0) return E_FILE_FORMAT_INVALID;
			this->PixelWidth = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvPixelHeight)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status <= 0) return E_FILE_FORMAT_INVALID;
			this->PixelHeight = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvPixelCropBottom)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->PixelCropBottom = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvPixelCropTop)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->PixelCropTop = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvPixelCropLeft)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->PixelCropLeft = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvPixelCropRight)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->PixelCropRight = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvDisplayWidth)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status <= 0) return E_FILE_FORMAT_INVALID;
			this->DisplayWidth = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvDisplayHeight)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status <= 0) return E_FILE_FORMAT_INVALID;
			this->DisplayHeigth = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvDisplayUnit)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status != 0) return E_FILE_FORMAT_INVALID; // 0 is only supported
			this->DisplayUnit = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvAspectRatioType)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0 || status > 2) return E_FILE_FORMAT_INVALID;
			this->AspectRatioType = status;
			return SUCCESS;
		}
		else return SUCCESS;
	}

	ParseResult TrackEntry::ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id)
	{
		ParseResult status = -111;
		if (id == MkvId::kMkvTrackNumber)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status <= 0) return E_FILE_FORMAT_INVALID;
			this->TrackNumber = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvTrackUID)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status <= 0) return E_FILE_FORMAT_INVALID;
			this->TrackNumber = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvTrackType)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status <= 0) return E_FILE_FORMAT_INVALID;
			switch (status)
			{
			case 1: Type = Video; break;
			case 2: Type = Audio; break;
			case 3: Type = Complex; break;
			case 0x10: Type = Logo; break;
			case 0x11: Type = Subtitle; break;
			case 0x12: Type = Buttons; break;
			case 0x20: Type = Control; break;
			default: return E_FILE_FORMAT_INVALID;
			}
			return SUCCESS;
		}
		else if (id == MkvId::kMkvFlagEnabled)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status != 0 && status != 1) return E_FILE_FORMAT_INVALID;
			this->FlagEnabled = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvFlagDefault)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status != 0 && status != 1) return E_FILE_FORMAT_INVALID;
			this->FlagDefault = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvFlagForced)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status != 0 && status != 1) return E_FILE_FORMAT_INVALID;
			this->FlagForced = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvFlagLacing)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status != 0 && status != 1) return E_FILE_FORMAT_INVALID;
			this->FlagLacing = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvDefaultDuration)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status <= 0) return E_FILE_FORMAT_INVALID;
			this->DefaultDuration = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvName)
		{
			std::string str;
			status = UnserializeString(this->m_pReader, d_start, d_size, str);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->Name = str;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvLanguage)
		{
			std::string str;
			status = UnserializeString(this->m_pReader, d_start, d_size, str);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->Language = str;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvCodecID)
		{
			std::string str;
			status = UnserializeString(this->m_pReader, d_start, d_size, str);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->CodecID = str;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvCodecPrivate)
		{
			std::string str;
			status = UnserializeString(this->m_pReader, d_start, d_size, str);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->CodecPrivate = str;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvCodecName)
		{
			std::string str;
			status = UnserializeString(this->m_pReader, d_start, d_size, str);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->CodecName = str;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvCodecDelay)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->CodecDelay = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvSeekPreRoll)
		{
			status = UnserializeUInt(this->m_pReader, d_start, d_size);
			if (status < 0) return E_FILE_FORMAT_INVALID;
			this->SeekPreRoll = status;
			return SUCCESS;
		}
		else if (id == MkvId::kMkvVideo)
		{
			if (!pVideo) delete pVideo;
			pVideo = new VideoSetting(e_start, e_size, d_start, d_size, this, this->m_pReader);
			status = pVideo->ParseFromFile();
			if (status != SUCCESS) delete pVideo;
			return status;
		}
		else if (id == MkvId::kMkvAudio)
		{
			if (!pAudio) delete pAudio;
			pAudio = new AudioSetting(e_start, e_size, d_start, d_size, this, this->m_pReader);
			status = pAudio->ParseFromFile();
			if (status != SUCCESS) delete pAudio;
			return status;
		}
		else if (id == MkvId::kMkvContentEncodings)
		{
			if (!pContentEncodings) delete pContentEncodings;
			pContentEncodings = new ContentEncodings(e_start, e_size, d_start, d_size, this, this->m_pReader);
			status = pContentEncodings->ParseFromFile();
			if (status != SUCCESS) delete pContentEncodings;
			return status;
		}
		else return SUCCESS;
	}

	ParseResult Tracks::ParseChild(BytePostion e_start, Uint64 e_size, BytePostion d_start, Uint64 d_size, EbmlID id)
	{
		if (id == MkvId::kMkvTrackEntry)
		{
			TrackEntry* pEntry = new TrackEntry(e_start, e_size, d_start, d_size, this, this->m_pReader);
			ParseResult status = pEntry->ParseFromFile();
			if (status != SUCCESS) delete pEntry;
			else this->TrackEntries.push_back(pEntry);
			return status;
		}
		else return SUCCESS;
	}
}