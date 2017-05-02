cplusplus {{
#include "ApplicationPacket_m.h"
}}

class ApplicationPacket;

struct ImagePacketHeader {
	// Node informations
	// -----------------
	unsigned short nodeID;		// the ID of the Node
	double locX; 				// x-coordinate of the node
	double locY; 				// y-coordinate of the node
	
	// Image PKT informations
	// ----------------------
	// Number of the current packet
	int imagePktNumber;
	// Total number of packets generated from the same image
	int totalImgPackets;
	// The set number indicates how many images
	// the node sent until the creation of this pkt
	int setNumber;
	// Time of the current snapshot
	simtime_t currentSnapshotTime;
	// The current size of the fragment buffer
	int bufferSize;
	// The number of image source that generated the image of the packet
	int imageSourceNumber;
	// The source folder of original images - used to find the original image to compute PSNR
	string originalImagesFolder;
}

packet ImagePacket extends ApplicationPacket {
	ImagePacketHeader imgPktHeader;
	unsigned char bufferFragment[];
}
