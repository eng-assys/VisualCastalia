#ifndef _SINKNODEMANAGER_H_
#define _SINKNODEMANAGER_H_

// VisualCastalia trace structure
#include "ImageTransmissionUtils.h"

struct lastPacketInfo {
	// int nodeID;
	int imageSourceNumber;
	int setNumber;
	double receivementSnapshotTime;
	double currentSnapshotTime;
	int bufferSize;
	int totalImgPackets;
	string originalImagesFolder;
};

using namespace std;

class SinkNodeManager {
	public:
		// Store the header information of the last packet received from each node
		map<int, lastPacketInfo> lastPacketEachNode;
		// When the first image packet from a new node arrives in the sink node
		// the number of correspondent node in the vector turns 1. This allows
		// the sink node creates a corresponder folder to store the received images
		// from that sender node
		// vector<bool> nodeIDList = vector<bool>(10000, false);
		set<int> nodeIDList;

		// Store how many complete images where received from each node
		map<int, int> howManyImgReceived;

		// Number of the node whose packet was received
		int numNode;

		// The simulation time that some function of this sinkNodeManager is called
		double simulationTime;

		// Return true if the packet received is the last valid packet of the same image source
		// When the sink receives another packet from the same node, but from the next image source
		// It is necessary generate the image from the before received image packets
		bool isANewImageSource (int nodeID, int setNumber);

		// When a duplicated packet is received returns false
		void receiveImagePacket (unsigned short nodeID,
									double locX,
									double locY,
									vector<unsigned char> bufferFragment,
									int imgPckNumber,
									int totalImagePkts,
									int setNumber);

		// bool isNotDuplicatedPacket(unsigned short nodeID,
		// 											int setNumber,
		// 											int imgPckNumber);

		// bool isLastPacket ( int totalImgPackets, int imagePktNumber, bool isNotDuplicated);
		bool isLastPacket ( int totalImgPackets, int imagePktNumber);

		vector<unsigned char> getCompleteBuffer(unsigned short nodeID,
										int setNumber, 
										int pckSize, 
										int totalImagePkts);
	private:
		string getImgBufferKey (unsigned short nodeID, int setNumber, int imgPckNumber);

		map< string, vector<unsigned char> > imagePacketBuffer;
		ofstream internalTrace;

		// It performs VisualCastalia traces
		// ImageTransmissionTraces visualCastaliaTrace;

		// Store the keys of image packet, avoid receivement of duplicated packets
		// set<string> imgPckKeys;
};

#endif

