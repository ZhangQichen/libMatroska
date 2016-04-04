#include "EbmlHeader.h"
#include "MkvReader.hpp"
#include "assert.h"
#include "WebmFile.h"
#include <string>
#include <iostream>
using namespace MkvParser;
int main(int argc, char* arg[])
{
	MkvParser::MkvReader reader;
	/*if (reader.Open("C:\\Users\\Qichen Zhang\\Desktop\\1.webm") < 0)
		return 1;
	if (argc < 2) return 1;
	std::string filename(arg[1]);*/
	WebmFile file("C:\\Users\\Qichen Zhang\\Desktop\\1.webm");

	std::cout << std::endl;
	ParseResult status = file.ParseFile();
	std::cout << "Parsing result: " << status;
	if (status == SUCCESS)
	{
		using namespace std;
		cout << "EBML Header\n";
		cout << "Ebml Version:\t\t" << file.pHeader->EbmlVersion << endl;
		cout << "Ebml MaxIdLen:\t\t" << file.pHeader->EbmlMaxIDLength << endl;;
		cout << "Ebml MaxSizeLen:\t\t" << file.pHeader->EbmlMaxSizeLength << endl;;
		cout << "Ebml DocType:\t\t" << file.pHeader->DocType << endl;;
		cout << "Ebml Version:\t\t" << file.pHeader->EbmlVersion << endl;
		cout << "Pos:\t\t" << file.pHeader->GetElementStart() + file.pHeader->GetElementSize() << endl << endl;

		cout << "Segment Info\n";
		cout << "TimeCodeScale:\t\t" << file.pSegment->pSegInfo->TimecodeScale << endl;
		cout << "Duration:\t\t" << file.pSegment->pSegInfo->Duration << endl;
		cout << "Muxing App:\t\t" << file.pSegment->pSegInfo->MuxingApp << endl;
		cout << "Writing App:\t\t" << file.pSegment->pSegInfo->WritingApp << endl;
		cout << "Position(Segment):\t\t" << file.pSegment->GetDataStart() << endl << endl;

		cout << "Track Info\n";
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
		for (Cluster* p : file.pSegment->ClusterCollection)
		{
			cout << "Cluster Timecode\t\t" << p->TimeCode << endl;
			cout << "Cluster Abs Time\t\t" << p->TimeCode * file.pSegment->pSegInfo->TimecodeScale << endl;
			int bn = 0;
			for (IBlockEntry* pp : p->BlockCollection)
			{
				cout << "\t\t" << ++bn << "\tTrackN: " << pp->GetTrackNumber() << "\tTimecode: " << pp->GetTimeCode() << endl;
			}
		}

		cout << endl;

		cout << "Cueing Data Info\n";
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

	return 0;
}
