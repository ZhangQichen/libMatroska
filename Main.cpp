#include "EbmlHeader.h"
#include "MkvReader.hpp"
#include "assert.h"
#include "WebmFile.h"
#include <string>
#include <iostream>
#include "Serialization.h"
using namespace MkvParser;
using namespace std;

int testParsing(int argc, char* arg[])
{
	MkvParser::MkvReader reader;
	/*if (reader.Open("C:\\Users\\Qichen Zhang\\Desktop\\1.webm") < 0)
	return 1;*/
	//std::string filename("C:\\Users\\Qichen Zhang\\Documents\\Wondershare Video Converter Ultimate\\Output\\1280_720p\\got2.webm");
	if (argc < 2) return 1;
	std::string filename(arg[1]);
	WebmFile file(filename);

	std::cout << std::endl;
	ParseResult status = file.ParseFile();
	std::cout << "Parsing result: " << status << std::endl;
	{
		using namespace std;
		if (file.pHeader)
		{
			cout << "EBML Header\n";
			cout << "Ebml Version:\t\t" << file.pHeader->EbmlVersion << endl;
			cout << "Ebml MaxIdLen:\t\t" << file.pHeader->EbmlMaxIDLength << endl;;
			cout << "Ebml MaxSizeLen:\t\t" << file.pHeader->EbmlMaxSizeLength << endl;;
			cout << "Ebml DocType:\t\t" << file.pHeader->DocType << endl;;
			cout << "Ebml Version:\t\t" << file.pHeader->EbmlVersion << endl;
			cout << "Pos:\t\t" << file.pHeader->GetElementStart() + file.pHeader->GetElementSize() << endl << endl;
		}

		if (file.pSegment)
		{
			cout << "Segment Info\n";
			cout << "TimeCodeScale:\t\t" << file.pSegment->pSegInfo->TimecodeScale << endl;
			cout << "Duration:\t\t" << file.pSegment->pSegInfo->Duration << endl;
			cout << "Muxing App:\t\t" << file.pSegment->pSegInfo->MuxingApp << endl;
			cout << "Writing App:\t\t" << file.pSegment->pSegInfo->WritingApp << endl;
			cout << "Position(Segment):\t\t" << file.pSegment->GetDataStart() << endl << endl;

			cout << "Track Info\n";
			cout << "Track Number: " << file.pSegment->pTracks->TrackEntries.size() << endl;
			cout << "Track TimeCodeScale:\t\t" << file.pSegment->pSegInfo->TimecodeScale << endl;
			for (TrackEntry* p : file.pSegment->pTracks->TrackEntries)
			{
				if (p == nullptr) continue;
				cout << "Track Type\t\t" << p->Type << endl;
				cout << "Track Number\t\t" << p->TrackNumber << endl;
				cout << "Track UID\t\t" << p->TrackUID << endl;
				cout << "Track Name\t\t" << p->Name << endl;
				cout << "Track Codec ID\t\t" << p->CodecID << endl;
				cout << "Track Codec Name\t\t" << p->CodecName << endl;
				cout << "Track Codec Private\t\t" << p->CodecPrivate << endl;
				cout << "Track DefaultDuration\t\t" << p->DefaultDuration << endl;
				cout << "Track CodecDelay\t\t" << p->CodecDelay << endl;
				cout << "Track Language\t\t" << p->Language << endl;
				if (p->Type == TrackType::Video)
				{
					cout << "Video Dis Width\t\t" << p->pVideo->DisplayWidth << endl;
					cout << "Video Pixel Width\t\t" << p->pVideo->PixelWidth << endl;
					cout << "Video Dis Height\t\t" << p->pVideo->DisplayHeigth << endl;
					cout << "Video Pixel Height\t\t" << p->pVideo->PixelHeight << endl;
					cout << "Video Rate\t\t" << p->pVideo->AspectRatioType << endl;
				}
				else if (p->Type == TrackType::Audio)
				{
					cout << "Audio Dis Channels\t\t" << p->pAudio->Channels << endl;
					cout << "Audio BitDepth\t\t" << p->pAudio->BitDepth << endl;
					cout << "Audio SamplingFrequency\t\t" << p->pAudio->SamplingFrequency << endl;
					cout << "Audio SeekPreRoll\t\t" << p->SeekPreRoll << endl;
				}
			}

			cout << endl;

			cout << "Cluster Info\n";
			cout << "Cluster Count: " << file.pSegment->ClusterCollection.size() << endl;
			for (Cluster* p : file.pSegment->ClusterCollection)
			{
				cout << "Cluster Timecode\t\t" << p->TimeCode << endl;
				cout << "Cluster Abs Time\t\t" << p->TimeCode * file.pSegment->pSegInfo->TimecodeScale << endl;
				int bn = 0;
				for (IBlockEntry* pp : p->BlockCollection)
				{
					cout << "\t\t" << ++bn << "\tTrackN: " << pp->GetTrackNumber() << "\tTimecode: " << pp->GetTimeCode() << "\tIsKeyFrames: " << pp->IsKeyFrames() << endl;
				}
			}

			cout << endl;

			cout << "Cueing Data Info\n";
			cout << "CuePoint Number; " << file.pSegment->pCues->CuePointCollection.size() << endl;
			int cn = 0;
			for (CuePoint* p : file.pSegment->pCues->CuePointCollection)
			{
				cout << "\t\t" << ++cn << "\tCueTime" << p->CueTime;
				for (CueTrackPositions* ctp : p->CueTrackPositionCollection)
				{
					cout << "\tCueTrack: " << ctp->CueTrack << "\tCluesterPos: " << ctp->CueClusterPosition << "\tBlockN: " << ctp->CueBlockNumber << endl;
				}
			}
		}

	}
}

int testSerializeID()
{
	// ebml id
	Uint64 size;
	Byte* buffer = nullptr;
	EbmlID eid;

	size = SerializeEbmlID(MkvId::kMkvSegment, buffer);
	if (size != 4) throw std::exception();
	eid = 0;
	for (int i = 0; i < size; ++i)
	{
		eid <<= 8;
		eid += buffer[i];
	}
	cout << kMkvSegment << " " << eid << endl;

	size = SerializeEbmlID(MkvId::kMkvTracks, buffer);
	if (size != 4) throw std::exception();
	eid = 0;
	for (int i = 0; i < size; ++i)
	{
		eid <<= 8;
		eid += buffer[i];
	}
	cout << kMkvTracks << " " << eid << endl;

	size = SerializeEbmlID(MkvId::kMkvCues, buffer);
	if (size != 4) throw std::exception();
	eid = 0;
	for (int i = 0; i < size; ++i)
	{
		eid <<= 8;
		eid += buffer[i];
	}
	cout << kMkvCues << " " << eid << endl;

	size = SerializeEbmlID(MkvId::kMkvSeek, buffer);
	if (size != 2) throw std::exception();
	eid = 0;
	for (int i = 0; i < size; ++i)
	{
		eid <<= 8;
		eid += buffer[i];
	}
	cout << kMkvSeek << " " << eid << endl;

	size = SerializeEbmlID(MkvId::kMkvEBMLMaxIDLength, buffer);
	if (size != 2) throw std::exception();
	eid = 0;
	for (int i = 0; i < size; ++i)
	{
		eid <<= 8;
		eid += buffer[i];
	}
	cout << kMkvEBMLMaxIDLength << " " << eid << endl;

	size = SerializeEbmlID(MkvId::kMkvVoid, buffer);
	if (size != 1) throw std::exception();
	eid = 0;
	for (int i = 0; i < size; ++i)
	{
		eid <<= 8;
		eid += buffer[i];
	}
	cout << kMkvVoid << " " << eid << endl;
}

int testSerializeSize()
{
	Byte* buffer = nullptr;
	Byte* a;
	Uint64 len = 0;
	len = SerializeEbmlSize(0x8ALL, buffer);
	len = SerializeEbmlSize(0xAC8ALL, buffer);
	len = SerializeEbmlSize(0xFFFD8ALL, buffer);
	len = SerializeEbmlSize(0xFFFFFEFFAFFFDDLL, buffer);
	len = 0;
	return len;
}

int testSerializeUint()
{
	Byte* buffer = new Byte[10];
	Uint64 l;
	l = SerializeUint(0xFFFFFFFFFFFFFFFFLL, buffer);
	l = SerializeUint(0xEFA1658DCBB978LL, buffer);
	l = SerializeUint(0xEFDCB900124d, buffer);
	l = SerializeUint(0x0000000000, buffer);
	l = SerializeUint(0x11334455, buffer);
	l = SerializeUint(0x78BDCD, buffer);
	l = SerializeUint(0xABAB, buffer);
	l = SerializeUint(0x1, buffer);
	l = SerializeUint(0x0, buffer);
	return l;
}

int testSerializeFloat()
{
	Byte* buffer = nullptr;
	Uint64 l;
	Byte* ptr;
	double d = 1.29834738297;
	l = SerializeFloat(d, buffer);
	
	ptr = (Byte*)&d;
	for (int i = 0; i < l; ++i)
		cout << (int)*(ptr + i);
	cout << endl;
	for (int i = l - 1; i >= 0; --i)
		cout << (int)*(buffer + i);
	cout << endl;
	cout << endl;

	d = 123.3245;
	l = SerializeFloat(d, buffer);
	ptr = (Byte*)&d;
	for (int i = 0; i < l; ++i)
		cout << (int)*(ptr + i);
	cout << endl;
	for (int i = l - 1; i >= 0; --i)
		cout << (int)*(buffer + i);
	cout << endl;
	cout << endl;

	d = 3e19;
	l = SerializeFloat(d, buffer);
	ptr = (Byte*)&d;
	for (int i = 0; i < l; ++i)
		cout << (int)*(ptr + i);
	cout << endl;
	for (int i = l - 1; i >= 0; --i)
		cout << (int)*(buffer + i);
	cout << endl;
	cout << endl;
	return l;
}

int testSerializeString()
{
	Uint64 l;
	Byte* buf = nullptr;
	string s = "helakjdfsal;skdfj";
	l = SerializeString(s, s.size(), buf);
	for (int i = 0; i < l; ++i)
	{
		cout << (*(buf + i) == s[i]) << endl;
	}
	return l;
}

int main(int argc, char* arg[])
{
	testSerializeFloat();
	return 0;
}
